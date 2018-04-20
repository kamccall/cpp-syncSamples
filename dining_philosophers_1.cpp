#include <pthread.h>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <unistd.h>
#define MAX 5
#define LOOP 5000

using namespace std;

class DiningPhilosophers {
private:
  pthread_mutex_t lock;
  enum State { THINKING, HUNGRY, EATING } state[MAX];
  pthread_cond_t self[MAX];




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
    // if left and right neighbors are eating, wait
	while (state[(i + MAX - 1) % MAX] == EATING ||
		state[(i + 1) % MAX] == EATING)
	{
		pthread_cond_wait(&self[i], &lock);
	}
	state[i] = EATING;
    cout << "philosopher[" << i << "] picked up chopsticks -- " << i << " Eating..."<< endl;

    pthread_mutex_unlock( &lock );
  }
  
  
  void putDown( int i ) 
  {
    pthread_mutex_lock( &lock );

    //cout << "philosopher[" << i << "] put down chopsticks."<< endl;
    state[ i ] = THINKING; // I'm stuffed and now thinking.
    // check if left and right neighbors are hungry and signal them
	if (state[(i + MAX - 1) % MAX] == HUNGRY)
		pthread_cond_signal(&self[(i + MAX - 1) % MAX]);
	if (state[(i + 1) % MAX] == HUNGRY)
		pthread_cond_signal(&self[(i + 1) % MAX]);

    pthread_mutex_unlock( &lock );
  }
} table;


void *philosopher( void *arg ) 
{
  int id = *(int *)arg;
  int st;

  for ( int i = 0; i < LOOP; i++ ) 
  {
	  st = (rand() % 5) + 1;
	  //cout << "philosopher[" << id << "] thinking for " << st << " sec.\n";
	  usleep(st * 10);
	  table.pickUp(id);
	  // usleep( 500 );

	  st = (rand() % 5) + 1;
	  //cout << "philosopher[" << id << "] eating for " << st << "/2 sec.\n";
	  usleep(st * 10);
	  table.putDown(id);
	  //usleep( 500 );
  }
  //cout << "philosopher[" << id << "] left the table."<< endl;
}


//
// M A I N
//
int main( ) 
{
  srand((unsigned)time(0));
  
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
