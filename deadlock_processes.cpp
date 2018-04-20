#include <iostream> // cout, cerr
#include <sys/types.h> // fork, wait
#include <sys/wait.h>  // wait
#include <unistd.h>    // fork
#define LOOP 100

using namespace std;

int main( int argc, char *argv[] ) {
  if ( argc != 2 ) {
    cerr << "usage: deadlock_processes bufsize" << endl;
    return -1;
  }
  int size = atoi( argv[1] );
  char *buf = new char[size];

  int toChild[2], toParent[2];  // two pipes
  pipe( toChild );
  pipe( toParent );

  if ( fork( ) != 0 ) {
    // parent
    close( toChild[0] );
    close( toParent[1] );

    for ( int i = 0; i < LOOP; i++ ) {
      cout << "Round " << i << ": parent will send a message." << endl;
      // write a message
      write( toChild[1], buf, size );
      // read a message
      read( toParent[0], buf, size );
      cout << "Round " << i << ": parent got a message." << endl;
    }
    wait( NULL );
    cout << "Parent synchronized with child." << endl;
  } else {
    // child
    close( toChild[1] );
    close( toParent[0] );    

    for ( int i = 0; i < LOOP; i++ ) {
      cout << "Round " << i << ": child will send a message." << endl;
      // write a message
      write( toParent[1], buf, size );
      // read a message
      read( toChild[0], buf, size );
      cout << "Round " << i << ": child got a message." << endl;
    }
    exit( 0 );
  }
}
