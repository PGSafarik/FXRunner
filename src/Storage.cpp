/*************************************************************************
* Storage.cpp Copyright (c) 2022 by  D.A.Tiger                           *
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
#include<Storage.h>

// Bad handle value
#ifdef  WIN32
#define BadHandle INVALID_HANDLE_VALUE
#else
#define BadHandle -1
#endif


/*** GENERAL STORAGE INTERFACE ********************************************************************/
Storage::Storage( const FXString &type )
{  
   FXIODevice::device = BadHandle;
   m_type = type;
}

Storage::~Storage( )
{


}

FXbool Storage::open( const FXString &name, FXuint m )
{
  FXbool res = false;
 
  if( !m_name.empty( ) ) {
    m_name = name;
    FXIO::access = m; 
    FXIO::pointer = 0L;
    res = true;
  }

  return res;
}


/*** END ******************************************************************************************/
