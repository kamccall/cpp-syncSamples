#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

class Monitor 
{
private:
  pthread_mutex_t lock;
  pthread_cond_t x;
  pthread_cond_t y;
public:
  Monitor( ) 
  {
    pthread_mutex_init( &lock, NULL );
    pthread_cond_init( &x, NULL );
    pthread_cond_init( &y, NULL );
  }
  void methodA( ) 
  {
    pthread_mutex_lock( &lock );
	  cout<< "In methodA  1... note how no one else is active for 10 seconds\n";
	  sleep(3);
	  cout<< "In methodA  2... about to wait on x, this 'releases' lock, someone else can be active:\n";
    pthread_cond_wait( &x, &lock );
	  cout<< "In methodA  3...  \n";
    pthread_cond_signal( &y );
    cout << "woke up from method A" << endl;
    pthread_mutex_unlock( &lock );
  }

  void methodB( ) 
  {
    pthread_mutex_lock( &lock );
	  cout<< "In methodB  1...\n";
    pthread_cond_wait( &y, &lock );
    // pthread_cond_signal( &x );
    cout << "woke up from method B" << endl;
    pthread_mutex_unlock( &lock );
  }
  
  void methodC( ) 
  {
    pthread_mutex_lock( &lock );
		cout<< "In methodC...\n";
    cout << "signal to x on method A" << endl;
    pthread_cond_signal( &x );
    pthread_mutex_unlock( &lock );
  }
} monitor;

void* thread_func(void* arg) 
{
  int id = *(int*)arg;

  if ( id == 1 ) monitor.methodA( );              // thread 1 waits in methodA 
  else if ( id == 2 ) monitor.methodB( );         // thread 2 waits in methodB
  //else while ( true ) cout << "hello" << endl;    // thread 3 keeps mumbling
}

int main( ) 
{
  pthread_t thread_id[3]; 
  int logical_id[3] = {1, 2, 3};

  pthread_create( &thread_id[0], NULL, thread_func, (void *)&logical_id[0] );
  pthread_create( &thread_id[1], NULL, thread_func, (void *)&logical_id[1] );
  pthread_create( &thread_id[2], NULL, thread_func, (void *)&logical_id[2] );

  sleep( 1 );
  monitor.methodC( );                              // main thread wakes up thread 1

  pthread_join( thread_id[0], NULL );
  pthread_join( thread_id[1], NULL );
  
  return 0;
}
