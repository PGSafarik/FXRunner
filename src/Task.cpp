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
Task::Task( const FXString &cmd_str )
{
  cmd  = cmd_str;
  prm  = FXString::null;
  wpth = FXString::null;
  su   = false;
  ow   = true;
  te   = false;
  lt   = false;
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
}

void Task::load( Storage &store )
{
  FXArray<FXString> list;

  if( store.readEntry( list ) > 0 ) { 
    cmd  = list[ 0 ]; 
    prm  = list[ 1 ];
    wpth = list[ 2 ]; 
    su   = list[ 3 ].toInt( );
    ow   = list[ 4 ].toInt( );
    te   = list[ 5 ].toInt( );
    lt   = list[ 6 ].toInt( );    
  }  
  else { std::cerr << "[ERROR] Task entry is not read!" << std::endl; } 
}

void Task::save( Storage &store ) 
{
  FXArray<FXString> list;

  list.insert( 0, cmd );
  list.insert( 1, prm );
  list.insert( 2, wpth );
  list.insert( 3, FXString::value( su ) );
  list.insert( 4, FXString::value( ow ) );
  list.insert( 5, FXString::value( te ) );
  list.insert( 6, FXString::value( lt ) );

  if( store.writeEntry( list ) <= 0 ) { std::cerr << "[ERROR] Task entry is not write!" << std::endl; }
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
