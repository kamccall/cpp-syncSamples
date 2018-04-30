
//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
#include <iostream> // cout
#include <stdlib.h> // atoi
#include <unistd.h> // sleep

using namespace std;

typedef struct 
{
  int   id; 
  int   size;
  int   iterations;
  char* s;
} Thread_struct;

void* infloop(void* x)
{
  int i, j;
  Thread_struct* t;
 
  t = (Thread_struct*) x;

  for (i = 0; i < t->iterations; i++) 
  {
    for (j = 0; j < t->size-1; j++)  
    {
      t->s[j] = 'a'+t->id;
    }
    t->s[j] = '\0';
    cout << "Thread " << t->id << ": " << t->s << "\n";
  }
}

int main(int argc, char **argv)
{
  pthread_t*      tid;
  pthread_attr_t* attr;
  Thread_struct*  t;
  void*           retval;
  char*           s;
  int nthreads, size, iterations, i;
  
  if (argc != 4) 
  {
	  cout << "usage: race nthreads stringsize iterations\n";
    exit(1);
  }

  nthreads = atoi(argv[1]);
  size = atoi(argv[2]);
  iterations = atoi(argv[3]);

  tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
  attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t) * nthreads);
  t = (Thread_struct*) malloc(sizeof(Thread_struct) * nthreads);
  s = (char*) malloc(sizeof(char*) * size);

  for (i = 0; i < nthreads; i++) 
  {
    t[i].id = i;
    t[i].size = size;
    t[i].iterations = iterations;
    t[i].s = s;
    pthread_attr_init(attr+i);
    pthread_attr_setscope(attr+i, PTHREAD_SCOPE_SYSTEM);
    pthread_create(tid+i, attr+i, infloop, t+i);
  }
  for (i = 0; i < nthreads; i++) pthread_join(tid[i], &retval);
  // sleep(1);
  return 0;
}
