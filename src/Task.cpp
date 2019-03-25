// Task.cpp; Copyright (c) 23/03/2019;  D.A.Tiger; GNU GPL 3
#include<Task.h>
FXIMPLEMENT( Task, FXObject, NULL, 0 );

/**************************************************************************************************/
Task::Task( )
{
  cmd  = FXString::null;
  prm  = FXString::null;
  wpth = FXString::null;
  su   = false;
  ow   = true;
  te   = false;
  lt   = false;
  cl   = false;
}

Task::~Task( )
{ }

/**************************************************************************************************/
void Task::load( FXStream &store )
{
  store >> cmd;
  store >> prm;
  store >> wpth;
  store >> su;
  store >> ow;
  store >> te;
  store >> lt;
  store >> cl;
}

void Task::save( FXStream &store )
{
  std::cout << "Saving the " << this->cmd.text( ) << " of file stream." << std::endl;
  std::cout.flush( );
  store << cmd;
  store << prm;
  store << wpth;
  store << su;
  store << ow;
  store << te;
  store << lt;
  store << cl;
}

/*** END ******************************************************************************************/
