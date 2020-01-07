#include <string.h>
#include <stdio.h>
#include <stdlib.h>         // for exit()
#include <sys/types.h>
#include <unistd.h>         // for execvp(), pipe()
#include <sys/wait.h>       // for wait()
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>        
#include <math.h>               
#include <pthread.h>                               
#include <semaphore.h>
#include <errno.h>

typedef unsigned long long int lluint;

// can be producer and consumer, when initializing the struct need to set the correct
// function for the worker.
typedef struct worker {
    int system_id;
    pthread_t thread;
} worker;

typedef struct buffer {
    int physical_size;  // The number of objects in the array
    int logical_size;   // The size of the array
    lluint *bufferArray;
} buffer;

typedef char* String;
typedef worker* Producer;
typedef worker* Consumer;

#define DEBUG                                   if (false)

#define MESSAGE_ALL_PRODUCERS_CREATED           "main thread created all producer threads\n"
#define MESSAGE_ALL_CONSUMERS_CREATED           "main thread created all consumer threads\n"
#define MESSAGE_ALL_PRODUCERS_FINISH            "all producers terminated\n"
#define MESSAGE_ALL_CONSUMERS_FINISH            "all consumers terminated\n"
#define ERROR_FIND_TWO_FACTORS                  "find_two_factors() function failed\n"

// semaphores names
#define SEM_MESSAGES_CONS                       "/global_messages_consumed_semaphore"
#define SEM_MESSAGES_PRO                        "/global_messages_produced_semaphore"
#define SEM_PRINT_SEM                           "/global_print_semaphore"
#define SEM_PHYSICAL_BUFFER_SIZE                "/global_buffer_physical_size_semaphore"
#define SEM_FAKE_WAIT                           "/global_wait_fake_lock"
#define SEM_ARRAY_SIZE                          5

#define SEM_PRINT(x)                            sem_wait(global_print_semaphore); x; sem_post(global_print_semaphore);

// globals varibales
pthread_t creator;
buffer program_buffer;

pthread_cond_t  global_signal_buffer_free;       // == can produce more
pthread_cond_t  global_signal_can_consume;

pthread_mutex_t global_buffer_lock;

sem_t *global_wait_fake;
sem_t *global_print_semaphore;
sem_t *global_messages_consumed_semaphore;
sem_t *global_messages_produced_semaphore;
sem_t *global_buffer_physical_size_semaphore;

worker *producers, *consumers;

int messages_consumed;
int messages_produced;

void program_start();
void* producer_function(void *producer);
void* consumer_function(void *consumer);
void worker_creator(worker **workers, void *function_to_active, int size, char* msg_to_print);
void wait_for_starting();
lluint get_prime_number();
void initializing_semaphores(bool with_creation);
void wait_all_threads(worker* worker, int size, String msg_to_print);