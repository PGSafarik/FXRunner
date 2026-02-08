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
#include "core/Task.h"
FXIMPLEMENT( Task, FXObject, NULL, 0 )

Task::Task( const FXString &cmd_str )
{
  m_cmd        = cmd_str;
  prm          = FXString::null;
  m_wpth       = FXString::null;
  reset_properties( );
  
  prop = new Properties;
}

Task::~Task( )
{ 
  delete prop;
}

void Task::dump( )
{
  std::cout << "Task: " << m_cmd.text( ) << " " << prm.text( ) << std::endl;
  std::cout << "Work dir: " << m_wpth.text( ) << std::endl;
  std::cout << "Properties: " << prop->suaccess << " " << prop->unblock << " " << prop->term << " " << prop->nocloseterm << std::endl;
  std::cout << "PROPERTIES: " << check_property( PRIVILAGE ) << check_property( UNBLOCK ) << check_property( TERMINAL ) << check_property( UNCLOSED ) << std::endl;
}


/*** END ******************************************************************************************/
