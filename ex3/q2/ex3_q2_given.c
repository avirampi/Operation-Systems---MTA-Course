
#include "ex3_q2.h"
#include "ex3_q2_given.h"

//------------------------------------------------------------------------
lluint get_random_in_range()
{
/*
  static int first = 1;
  if (first) {
    my_sleep();
    srand(getpid() * time(NULL));
    first = 0;
  }
*/
  int num = (rand() % (RAND_HI - RAND_LO + 1)) + RAND_LO;
  my_sleep();
  return (lluint) num;
}

//------------------------------------------------------------------------
int is_prime(lluint n)
{
  const lluint sqrt_n = sqrt(n);
  for (lluint j=2; j <= sqrt_n; j++)
    if (n%j == 0) return 0;
  my_sleep();
  return 1;
}

//------------------------------------------------------------------------
int find_two_factors(lluint num, lluint* f1, lluint* f2)
{
  // brute force method
  // there are somewhat better algorithms, but that's not the point in this exercise
  if(num < 4) 
    return 0;
  lluint limit = (lluint) sqrt(num);
  lluint m = 2;
  while (m <= limit)
  {
    if ((num % m ) == 0)
    {
      *f1 = m;
      *f2 = num/m;
      return 0;    // success
    }
    m++;
  }
  // if we got here, it means we did not find_two_factors
  return 1;  // failure
}



//------------------------------------------------------------------------
// previous name: void write_msg_and_add_to_buf
void write_add_to_buf_msg(int thread_num, lluint n1, lluint n2, lluint prod)
{
  printf("producer #%d ", thread_num); fflush(stdout);
  my_sleep();
  write_product("going to add", n1, n2, prod);
  // add_to_buf(prod);
  my_sleep(); 
}

//------------------------------------------------------------------------
// previous name: void write_msg_and_remove_from_buf
void write_remove_from_buf_msg(int thread_num, lluint* prod)
{
  my_sleep();
  // remove_from_buf(prod);
  my_sleep(); 
  printf("consumer #%d just removed: %llu\n", thread_num, *prod);
}

//------------------------------------------------------------------------
void write_product(char* who, lluint n1, lluint n2, lluint prod)
{
  printf("%s ", who); fflush(stdout);
  my_sleep();
  printf("product: "); fflush(stdout);
  my_sleep();

  if (n2 < n1)
  {
    lluint temp = n1;
    n1 = n2;
    n2 = temp;
  }
  printf("%llu = %llu x %llu\n", prod, n1, n2); fflush(stdout);
  my_sleep();  
}


//------------------------------------------------------------------------
void write_is_done(char* who, int thread_num)
{
  printf("%s #%d ", who, thread_num);
  fflush(stdout);
  my_sleep();
  printf("is done\n");
}

//------------------------------------------------------------------------
void write_producer_is_done(int thread_num)
{
  write_is_done("producer", thread_num);
}

//------------------------------------------------------------------------
void write_consumer_is_done(int thread_num)
{
  write_is_done("consumer", thread_num);
}


//------------------------------------------------------------------------
void my_sleep()
{
  if (SLEEP_FACTOR == 0) 
    return;
  static int fact = 0;
  fact = (fact + 20) % SLEEP_FACTOR;
  int delay = (int)pthread_self() %  10 + 1 + fact;
  usleep(SLEEP_FACTOR * delay);
}
