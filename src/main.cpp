// main.cpp; Copyright (c) 22/09/2012;  D.A.Tiger; GNU GPL 3
#include<main.h>
#include<Application.h>
#include<Runner.h>

int main( int argc, char **argv )
{
  Application app;
  app.init( argc, argv );
  new Runner( &app );
  app.create( );
  return app.run( );

}

/*** END ******************************************************************************************/
