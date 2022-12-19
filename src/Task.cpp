/*************************************************************************
* Task.cpp Copyright (c) 2015 - 2022 by  D.A.Tiger                       *
*                                                                        *
* This program is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU General Public License as published by   *
* the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                    *
*                                                                        *
* This program is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU General Public License      *
* along with this program.  If not, see <https://www.gnu.org/licenses/>. *
*************************************************************************/
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

void Task::load( Storage &store )
{
  //std::cout << "Task Entry Load: " << store.getName( ).text( ) ;

  FXArray<FXString> list;
  FXlong r = 0;

  if( ( r = store.readEntry( list ) ) >= 1 ) { 
    cmd  = list[ 0 ]; 
    prm  = list[ 1 ];
    wpth = list[ 2 ]; 
    su   = list[ 3 ].toInt( );
    ow   = list[ 4 ].toInt( );
    te   = list[ 5 ].toInt( );
    lt   = list[ 6 ].toInt( );
    cl   = list[ 7 ].toInt( );
    
  }  
  else { std::cerr << "[ERROR] Task entry is not read!" << std::endl; } 
   
  //std::cout << "( " << r << " ) \n";
}

void Task::save( Storage &store ) 
{


}

/**************************************************************************************************/
FXString Task::ConvOnText( const FXString &str )
{
  FXString text = "\"";
  if( !str.empty( ) ) { text += str; }
  text += "\"";
  
  return text;
}


/*** END ******************************************************************************************/
