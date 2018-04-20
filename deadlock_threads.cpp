#include <pthread.h>
#include <iostream>
#define LOOP 100

using namespace std;

pthread_mutex_t resource[3];

void *threadA( void *arg ) {
  cout << "A starts" << LOOP << endl;
  for ( int i = 0; i < LOOP; i++ ) {
    pthread_mutex_lock( &resource[1] );
    cout << "Round " << i << ": A got rsc 1" << endl;
    pthread_mutex_lock( &resource[0] );
    cout << "Round " << i << ": A got rsc 0" << endl;
    pthread_mutex_unlock( &resource[0] );
    pthread_mutex_unlock( &resource[1] );
  }
  cout << "A finished" << endl;
}

void *threadB( void *arg ) {
  cout << "B starts" << LOOP << endl;
  for ( int i = 0; i < LOOP; i++ ) {
    pthread_mutex_lock( &resource[3] );
    cout << "Round " << i << ": B got rsc 3" << endl;
    pthread_mutex_lock( &resource[0] );
    cout << "Round " << i << ": B got rsc 0" << endl;
    pthread_mutex_lock( &resource[2] );
    cout << "Round " << i << ": B got rsc 2" << endl;
    pthread_mutex_unlock( &resource[2] );
    pthread_mutex_unlock( &resource[0] );
    pthread_mutex_unlock( &resource[3] );
  }
  cout << "B finished" << endl;
}

void *threadC( void *arg ) {
  cout << "C starts" << endl;
  for ( int i = 0; i < LOOP; i++ ) {
    pthread_mutex_lock( &resource[2] );
    cout << "Round " << i << ": C got rsc 2" << endl;
    pthread_mutex_lock( &resource[1] );
    cout << "Round " << i << ": C got rsc 1" << endl;
    pthread_mutex_unlock( &resource[1] );
    pthread_mutex_unlock( &resource[2] );
  }
  cout << "C finished" << endl;
}

int main( int argc, char *argv[] ) {

  pthread_t tid[3];

  pthread_create( &tid[0], NULL, threadA, NULL );
  pthread_create( &tid[1], NULL, threadB, NULL );
  pthread_create( &tid[2], NULL, threadC, NULL );


  for ( int i = 0; i < 3; i++ )
    pthread_join( tid[i], NULL );

  return 0;
}
