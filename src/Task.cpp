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

void Task::load( const FXString &store )
{
  if( !store.empty( ) ) {
		SubstrStream sit( store, ";" );
		sit >> cmd;
		sit >> prm;
		sit >> wpth;
		sit >> su;
		sit >> ow;
		sit >> te;
		sit >> lt;
  } 
}

void Task::save( FXString &store )
{
		SubstrStream sit( ";" );
		sit << cmd;
		sit << prm;
		sit << wpth;
		sit << su;
		sit << ow;
		sit << te;
		sit << lt;
		
		store = sit.get_str( );
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
