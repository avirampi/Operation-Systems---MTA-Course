#include "ex3_q2_given.h"
#include "ex3_q2.h"

String SEM_NAMES[] = { SEM_PRINT_SEM, SEM_MESSAGES_CONS, SEM_MESSAGES_PRO, SEM_PHYSICAL_BUFFER_SIZE };
sem_t **SEM_ARRAY[] = { &global_print_semaphore, &global_messages_consumed_semaphore, 
                        &global_messages_produced_semaphore, &global_buffer_physical_size_semaphore };

void program_start() {
    creator = pthread_self();
    program_buffer.bufferArray = (lluint*)calloc(BUF_SIZE, sizeof(lluint));
    program_buffer.logical_size = BUF_SIZE;

    // INITIALIZING THE GLOBAL PTHREAD_X VARIABLES
    global_signal_buffer_free       = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    global_signal_can_consume       = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    global_wait_fake                = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    global_buffer_lock              = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    global_wait_fake_lock           = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

    initializing_semaphores();

    messages_consumed = 0;
    messages_produced = 0;

    pthread_mutex_lock(&global_wait_fake_lock);
    worker_creator(&producers, producer_function, N_PROD, MESSAGE_ALL_PRODUCERS_CREATED);
    worker_creator(&consumers, consumer_function, N_CONS, MESSAGE_ALL_CONSUMERS_CREATED);

    pthread_mutex_unlock(&global_wait_fake_lock);
    pthread_cond_broadcast(&global_wait_fake);
    pthread_exit(NULL); // let workers do there jobs
}

void worker_creator(worker **workers, void *function_to_active, int size, char* msg_to_print) {
    (*workers) = (worker*)malloc(size*sizeof(worker));

    for (int i = 0; i < size; ++i) {
        (*workers)[i].system_id = i + 1;
        pthread_create(&(*workers)[i].thread, NULL, function_to_active, &(*workers)[i]);
    }

    printf("%s",msg_to_print);
}

void *producer_function(void *producer_input) {
    Producer producer = (Producer)(producer_input);

    wait_for_starting();
    DEBUG printf("producer #%d is up\n", producer->system_id);

    while(true) {
        if (get_produced_messages()) break;

        lluint number_a = get_prime_number();
        lluint number_b = get_prime_number();
        lluint multiply_numbers = number_a * number_b;

        DEBUG printf("producer #%d: try getting buffer lock\n", producer->system_id);
        pthread_mutex_lock(&global_buffer_lock);
        DEBUG printf("producer #%d: got the buffer lock\n", producer->system_id);
        if (program_buffer.physical_size == program_buffer.logical_size) {
            DEBUG printf("producer #%d: buffer is full, going wait\n", producer->system_id);
            pthread_cond_broadcast(&global_signal_can_consume);
            pthread_cond_wait(&global_signal_buffer_free, &global_buffer_lock);
            DEBUG printf("producer #%d: woked up, can produce more\n", producer->system_id);
            if (program_buffer.physical_size == program_buffer.logical_size || get_produced_messages()) {
                DEBUG printf("producer #%d: release buffer lock\n", producer->system_id);
                pthread_mutex_unlock(&global_buffer_lock);
                pthread_cond_broadcast(&global_signal_can_consume);
                continue;
            }
        }

        add_to_buf(multiply_numbers);
        SEM_PRINT(write_add_to_buf_msg(producer->system_id, number_a, number_b, multiply_numbers))
        pthread_mutex_unlock(&global_buffer_lock);
        pthread_cond_broadcast(&global_signal_can_consume);

        DEBUG printf("producer #%d: release buffer lock\n", producer->system_id);
        produced_message();
    }

    SEM_PRINT(write_producer_is_done(producer->system_id))
    pthread_exit(NULL);
}

void *consumer_function(void *consumer_input) {
    Consumer consumer = (Consumer)(consumer_input);

    wait_for_starting(); // consumers no need to wait because if produces wait then there is nothing to consume
    DEBUG printf("consumer #%d is up\n", consumer->system_id);

    while(true) {
        if (get_consumed_messages()) break;

        lluint multiply, number_a, number_b;
        DEBUG printf("consumer #%d: try getting buffer lock\n", consumer->system_id);
        pthread_mutex_lock(&global_buffer_lock);
        DEBUG printf("consumer #%d: got the buffer lock\n", consumer->system_id);
        if (program_buffer.physical_size == 0) {
            DEBUG printf("consumer #%d: buffer is empty, going wait\n", consumer->system_id);
            pthread_cond_broadcast(&global_signal_buffer_free);
            pthread_cond_wait(&global_signal_can_consume, &global_buffer_lock);
            DEBUG printf("consumer #%d: woked up, can consume more\n", consumer->system_id);
            if (program_buffer.physical_size  == 0 || get_consumed_messages()) {
                DEBUG printf("consumer #%d: release buffer lock\n", consumer->system_id);
                pthread_mutex_unlock(&global_buffer_lock);
                pthread_cond_broadcast(&global_signal_buffer_free);
                continue;
            }
        }

        remove_from_buf(&multiply);         
        SEM_PRINT(write_remove_from_buf_msg(consumer->system_id, &multiply))
        pthread_mutex_unlock(&global_buffer_lock);
        pthread_cond_broadcast(&global_signal_buffer_free);

        DEBUG printf("consumer #%d: release buffer lock\n", consumer->system_id);
        if (find_two_factors(multiply, &number_a, &number_b) < 0) {
            printf(ERROR_FIND_TWO_FACTORS);
        }
        consumed_message();
    }

    SEM_PRINT(write_consumer_is_done(consumer->system_id))
    pthread_exit(NULL);
}

void wait_for_starting() {
    pthread_cond_wait(&global_wait_fake, &global_wait_fake_lock);
    pthread_mutex_unlock(&global_wait_fake_lock);
}

void add_to_buf(lluint number) {
    sem_wait(global_buffer_physical_size_semaphore);
    program_buffer.bufferArray[program_buffer.physical_size] = number;
    program_buffer.physical_size += 1;
    sem_post(global_buffer_physical_size_semaphore);
}

void remove_from_buf(lluint* prod) {
    sem_wait(global_buffer_physical_size_semaphore);
    program_buffer.physical_size -= 1;
    *prod = program_buffer.bufferArray[program_buffer.physical_size];
    sem_post(global_buffer_physical_size_semaphore);
}

lluint get_prime_number() {
    lluint number = get_random_in_range();
    while(!is_prime(number)) { 
        number = get_random_in_range();
    }

    return number;
}

void produced_message() {
    sem_wait(global_messages_produced_semaphore);
    messages_produced += 1;
    sem_post(global_messages_produced_semaphore);
}

void consumed_message() {
    sem_wait(global_messages_consumed_semaphore);
    messages_consumed += 1;
    sem_post(global_messages_consumed_semaphore);
}

bool get_consumed_messages() {
    int result;
    DEBUG printf("test sem_wait() in get_consumed_messages()\n");
    sem_wait(global_messages_consumed_semaphore);
    DEBUG printf("success sem_wait() in get_consumed_messages()\n");
    result = messages_consumed;
    sem_post(global_messages_consumed_semaphore);
    DEBUG printf("release sem_post() in get_consumed_messages()\n");

    return result == TOTAL_MSG;
}

bool get_produced_messages() {
    int result;
    DEBUG printf("test sem_wait() in get_produced_messages()\n");
    sem_wait(global_messages_produced_semaphore);
    DEBUG printf("success sem_wait() in get_produced_messages()\n");
    result = messages_produced;
    sem_post(global_messages_produced_semaphore);
    DEBUG printf("release sem_post() in get_produced_messages()\n");

    return result == TOTAL_MSG;
}

void initializing_semaphores() {
    for (int i = 0; i < SEM_ARRAY_SIZE; ++i) {
        if (sem_unlink(SEM_NAMES[i]) == 0) {
		    fprintf(stderr, "successul unlink of %s\n", SEM_NAMES[i]);
        }

        *(SEM_ARRAY[i]) = sem_open(SEM_NAMES[i], O_CREAT, S_IRWXU, 1); 
        if (*(SEM_ARRAY[i]) == SEM_FAILED) {
            fprintf(stderr, "failed to open semaphore %s\n", SEM_NAMES[i]);
            exit(EXIT_FAILURE);
        }
    }
}