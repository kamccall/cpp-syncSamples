
//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
#include <iostream> // cout
#include <stdlib.h> // atoi

using namespace std;

typedef struct {
  pthread_mutex_t *lock;
  int id; 
  int size;
  int iterations;
  char *s;
} Thread_struct;

void *infloop(void *x)
{
  int i, j;
  Thread_struct *t;
 
  t = (Thread_struct *) x;

  for (i = 0; i < t->iterations; i++) {
    pthread_mutex_lock(t->lock);
    for (j = 0; j < t->size-1; j++) {
      t->s[j] = 'A'+t->id;
    }
    t->s[j] = '\0';
    //printf("Thread %d: %s\n", t->id, t->s);
	cout << "Thread " << t->id << ": " << t->s;
    pthread_mutex_unlock(t->lock);
  }
}

main(int argc, char **argv)
{
  pthread_mutex_t lock;
  pthread_t *tid;
  pthread_attr_t *attr;
  Thread_struct *t;
  void *retval;
  int nthreads, size, iterations, i;
  char *s;

  if (argc != 4) {
    //fprintf(stderr, "usage: race nthreads stringsize iterations\n");
	cout << "usage: race nthreads stringsize iterations\n";
    exit(1);
  }

  pthread_mutex_init(&lock, NULL);
  nthreads = atoi(argv[1]);
  size = atoi(argv[2]);
  iterations = atoi(argv[3]);

  tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  attr = (pthread_attr_t *) malloc(sizeof(pthread_attr_t) * nthreads);
  t = (Thread_struct *) malloc(sizeof(Thread_struct) * nthreads);
  s = (char *) malloc(sizeof(char *) * size);

  for (i = 0; i < nthreads; i++) {
    t[i].id = i;
    t[i].size = size;
    t[i].iterations = iterations;
    t[i].s = s;
    t[i].lock = &lock;
    pthread_attr_init(attr+i);
    pthread_attr_setscope(attr+i, PTHREAD_SCOPE_SYSTEM);
    pthread_create(tid+i, attr+i, infloop, t+i);
  }
  for (i = 0; i < nthreads; i++) {
    pthread_join(tid[i], &retval);
  }
}
