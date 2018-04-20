#include <pthread.h>
#include <iostream>
#define MAX 5
#define LOOP 10

using namespace std;

class DiningPhilosophers {
private:
  pthread_mutex_t lock;
  enum State { THINKING, HUNGRY, EATING } state[MAX];
  pthread_cond_t self[MAX];

  void test( int i ) {
    // if phi_i's leftis not eating, and phi_i is hungry, and 
    // phi_i's right is not eating, then phi_i can eat!
    // Wake up phi-i
    if ( ( state[ ( i + MAX - 1 ) % MAX ] != EATING ) &&
	 ( state[i] == HUNGRY ) &&
	 ( state[ ( i + 1 ) % MAX ] != EATING ) ) {
      state[ i ] = EATING;
      pthread_cond_signal( &self[ i ] );
    }
  }

public:
  DiningPhilosophers( ) 
  {
    pthread_mutex_init( &lock, NULL );
    for ( int i = 0; i < MAX; i++ ) {
      pthread_cond_init( &self[i], NULL );
      state[i] = THINKING;
    }
  }
  
  
  void pickUp( int i ) 
  {
    pthread_mutex_lock( &lock );

    state[ i ] = HUNGRY;   // I got hungry
    test( i );             // Can I have my left and right chopsticks?
    if ( state[ i ] != EATING ) // No, I can't, then I should wait
      pthread_cond_wait(  &self[ i ], &lock );
    cout << "philosopher[" << i << "] picked up chopsticks."<< endl;

    pthread_mutex_unlock( &lock );
  }
  
  
  void putDown( int i ) 
  {
    pthread_mutex_lock( &lock );

    cout << "philosopher[" << i << "] put down chopsticks."<< endl;
    state[ i ] = THINKING; // I'm stuffed and now thinking.
    // test left and right neighbors
    test( ( i + MAX - 1 ) % MAX );  // if possible, wake up my left.
    test( ( i + 1 ) % MAX );        // if possible, wake up my right.

    pthread_mutex_unlock( &lock );
  }
} table;


void *philosopher( void *arg ) 
{
  int id = *(int *)arg;

  for ( int i = 0; i < LOOP; i++ ) {
    table.pickUp( id );
    usleep( 500 );
    table.putDown( id );
    usleep( 500 );
  }
  cout << "philosopher[" << id << "] left the table."<< endl;
}


//
// M A I N
//
int main( ) {

  pthread_t thread_id[MAX]; 
  int logical_id[MAX];

  for ( int i = 0; i < MAX; i++ ) {
    logical_id[i] = i;
    pthread_create( &thread_id[i], NULL, philosopher, (void *)&logical_id[i] );
  }

  for ( int i = 0; i < MAX; i++ )
    pthread_join( thread_id[i], NULL );
  
  return 0;
}
