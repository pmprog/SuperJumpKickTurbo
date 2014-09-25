
#include "framework.h"

int main ( int argc, char* argv[] )
{
  Framework* fw = new Framework( 800, 480, 100, false );
  fw->Run();
  delete fw;
  return 0;
}
