#include "ex3_q2_given.h"
#include "ex3_q2.h"

String SEM_NAMES[] = { SEM_PRINT_SEM, SEM_MESSAGES_CONS, SEM_MESSAGES_PRO, SEM_PHYSICAL_BUFFER_SIZE, SEM_FAKE_WAIT };
sem_t **SEM_ARRAY[] = { &global_print_semaphore, &global_messages_consumed_semaphore, 
                        &global_messages_produced_semaphore, &global_buffer_physical_size_semaphore, &global_wait_fake };

void program_start() {
    creator = pthread_self();
    program_buffer.bufferArray = (lluint*)calloc(BUF_SIZE, sizeof(lluint));
    program_buffer.logical_size = BUF_SIZE;

    global_buffer_lock              = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    global_signal_buffer_free       = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    global_signal_can_consume       = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    initializing_semaphores(true);

    messages_consumed = 0;
    messages_produced = 0;

    sem_wait(global_wait_fake); // let all threads wait until finish
    worker_creator(&producers, producer_function, N_PROD, MESSAGE_ALL_PRODUCERS_CREATED);
    worker_creator(&consumers, consumer_function, N_CONS, MESSAGE_ALL_CONSUMERS_CREATED);
    sem_post(global_wait_fake); // now they can start working

    wait_all_threads(producers, N_PROD, MESSAGE_ALL_PRODUCERS_FINISH);
    wait_all_threads(consumers, N_CONS, MESSAGE_ALL_CONSUMERS_FINISH);
    initializing_semaphores(false);
}

void wait_all_threads(worker* worker, int size, String msg_to_print) {
    for (int i = 0; i < size; ++i) {
        pthread_join(worker[i].thread, NULL);
    }

    SEM_PRINT(printf("%s", msg_to_print))
}

void worker_creator(worker **workers, void *function_to_active, int size, char* msg_to_print) {
    (*workers) = (worker*)malloc(size*sizeof(worker));

    for (int i = 0; i < size; ++i) {
        (*workers)[i].system_id = i + 1;
        pthread_create(&(*workers)[i].thread, NULL, function_to_active, &(*workers)[i]);
    }

    SEM_PRINT(printf("%s", msg_to_print))
}

void *producer_function(void *producer_input) {
    Producer producer = (Producer)(producer_input);
    wait_for_starting();

    for ( ; ; ) {
        lluint  number_a = get_prime_number(),  number_b = get_prime_number(), 
                multiply_numbers = number_a * number_b; 

        pthread_mutex_lock(&global_buffer_lock);
        if (messages_produced >= TOTAL_MSG)  {
            pthread_mutex_unlock(&global_buffer_lock);
            pthread_cond_broadcast(&global_signal_can_consume);
            break;
        } else if (program_buffer.physical_size < program_buffer.logical_size) {
            add_to_buf(multiply_numbers);
            messages_produced++;
            SEM_PRINT(write_add_to_buf_msg(producer->system_id, number_a, number_b, multiply_numbers))
        }
        pthread_mutex_unlock(&global_buffer_lock);
        pthread_cond_broadcast(&global_signal_can_consume);
    }

    SEM_PRINT(write_producer_is_done(producer->system_id))
    pthread_exit(NULL);
}

void *consumer_function(void *consumer_input) {
    Consumer consumer = (Consumer)(consumer_input);
    wait_for_starting();

    for (lluint multiply = 0, number_a, number_b; ; multiply = 0) {

        pthread_mutex_lock(&global_buffer_lock);
        if (messages_consumed < TOTAL_MSG) {
            if (program_buffer.physical_size <= 0) {
                pthread_cond_wait(&global_signal_can_consume, &global_buffer_lock);
            } 
            
            if (program_buffer.physical_size > 0) {
                remove_from_buf(&multiply);
                messages_consumed++;
                SEM_PRINT(write_remove_from_buf_msg(consumer->system_id, &multiply))
            }
            pthread_mutex_unlock(&global_buffer_lock);
        } else {
            pthread_mutex_unlock(&global_buffer_lock);
            break;
        }
        
        if (multiply != 0) {
            if (find_two_factors(multiply, &number_a, &number_b) < 0) {
                SEM_PRINT(printf(ERROR_FIND_TWO_FACTORS))
            }
        }
    }

    SEM_PRINT(write_consumer_is_done(consumer->system_id))
    pthread_exit(NULL);
}

void wait_for_starting() {
    sem_wait(global_wait_fake);
    sem_post(global_wait_fake);
}

void add_to_buf(lluint number) {
    program_buffer.bufferArray[program_buffer.physical_size] = number;
    program_buffer.physical_size += 1;
}

void remove_from_buf(lluint* prod) {
    program_buffer.physical_size -= 1;
    *prod = program_buffer.bufferArray[program_buffer.physical_size];
}

lluint get_prime_number() {
    lluint number = get_random_in_range();
    while(!is_prime(number)) { 
        number = get_random_in_range();
    }

    return number;
}

void initializing_semaphores(bool with_creation) {
    for (int i = 0; i < SEM_ARRAY_SIZE; ++i) {
        if (sem_unlink(SEM_NAMES[i]) == 0) {
		    DEBUG fprintf(stderr, "successul unlink of %s\n", SEM_NAMES[i]);
        }

        if (with_creation) {
            *(SEM_ARRAY[i]) = sem_open(SEM_NAMES[i], O_CREAT, S_IRWXU, 1); 
            if (*(SEM_ARRAY[i]) == SEM_FAILED) {
                fprintf(stderr, "failed to open semaphore %s\n", SEM_NAMES[i]);
                exit(EXIT_FAILURE);
            }
        }
    }
}