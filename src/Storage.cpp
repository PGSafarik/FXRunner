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


/*** Storage **************************************************************************************/
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


/*** SimpleFileStorage ****************************************************************************
  FXArray<FXString> m_entries;   // Entries (file lines) list 
  FXString          m_path;      // Path of file storage
  FXbool            m_ready;     // Storage is ready (opened)
*/


SimpleFileStorage::SimpleFileStorage( const FXString &path )
                 : Storage( "SimpleFileStorage" ), m_path( path ), m_ready( false )
{ }

SimpleFileStorage::~SimpleFileStorage( )
{ }

FXbool SimpleFileStorage::open( const FXString &name, FXuint m )
{


}

FXuint SimpleFileStorage::position( FXlong offset,FXuint from )
{

}

FXival SimpleFileStorage::readEntry( FXArray<FXString> &buff )
{

}

FXival SimpleFileStorage::writeEntry( const FXArray<FXString> &buff )
{


} 

FXint SimpleFileStorage::eof( )
{
  FXint res = FXIO::Error;

  if( m_ready ) { resh = ( FXIO::pointer < m_entries.no( ) ); }  

  return resh;
}
/*** END ******************************************************************************************/
