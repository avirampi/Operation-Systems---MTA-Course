
#define RAND_LO         100    // low bound for random numbers
#define RAND_HI         200    // hi  bound for random numbers
// #define RAND_LO      1000 * 1000 * 1000     
// #define RAND_HI      1100 * 1000 * 1000     

#define N_PROD          2   // number of producer threads
#define N_CONS          3   // number of consumer threads
#define BUF_SIZE        4   // size of the buffer
#define TOTAL_MSG       7   // total number of messages to create

#define SLEEP_FACTOR    200  // used by my_sleep


typedef long long unsigned lluint;

/* the following is to keep backward compatability after the change + rename
   of the functions write_msg_and_add_to_buf and write_msg_and_remove_from_buf
*/
#define write_msg_and_add_to_buf        write_add_to_buf_msg
#define write_msg_and_remove_from_buf   write_remove_from_buf_msg

// function prototypes
lluint get_random_in_range();
int is_prime(lluint n);
int find_two_factors(lluint, lluint*, lluint*);
void write_add_to_buf_msg(int thread_num, lluint n1, lluint n2, lluint prod);
void write_remove_from_buf_msg(int thread_num, lluint* prod);
void write_product(char* who, lluint n1, lluint n2, lluint prod);
void add_to_buf (lluint prod);
void remove_from_buf(lluint* prod);
void write_producer_is_done(int thread_num);
void write_consumer_is_done(int thread_num);
void my_sleep();
