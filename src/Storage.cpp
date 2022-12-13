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
  FXbool res = false;
  if( !m_path.empty( ) && Storage::open( name, m ) ) {
    close( );
    m_entries.clear( );
    
    FXFile store( m_path + PATHSEPSTRING + getName( ), FXIO::ReadOnly );
    if( store.isOpen( ) ) {
      FXString buff;
      FXlong size = store.size( );
      buff.length( size );
      if( store.readBlock( buff.text( ), size ) == size ) {
        FXint num = buff.contains( '\n' );
        for( FXint i = 0; i != num; i++ ) { 
          FXString line = buff.section( '\n', i );
          line.trim( );
          if( !line.empty( ) ) { m_entries.push( line ); }           
        }
        res = m_ready = true; 
      } 
      store.close( );   
    }  
  }

  return res;
}

FXlong SimpleFileStorage::position( FXlong offset,FXuint from )
{
  FXlong value;  
  
  if( m_ready && m_entries.no( ) >= 0 ) {
    switch( from ) {
      case FXIO::Begin : 
      {
        value = offset - 1; 
        break;
      } 
      case FXIO::Current : 
      {
        value = FXIO::pointer + offset;
        break;
      }
      case FXIO::End : 
      {
        value = m_entries.no( ) - offset;
        break;
      }
    }

    if( value >= 0 && value <= m_entries.no( ) ) { 
      FXIO::pointer = value; 
      return value;
    }
  }
  
  return FXIO::Error;
}

FXival SimpleFileStorage::readEntry( FXArray<FXString> &buff )
{
  FXlong res = FXIO::Error;
  
  if( eof( ) == 0 && m_entries.no( ) > 0 ) {
    FXString sep = ";";
    FXString data = m_entries[ FXIO::pointer ];

    if( !data.empty( ) ) {
      FXint num = data.contains( sep );
      if( num > 1 ) {
        FXint i; 
        for( i = 0; i != num; i++ ) {
          buff[ i ] = data.section( sep, i );   
        }
        res = i;
      }
      else { buff[ 0 ] = data; }
    }
    else { res = FXIO::Again; }
    FXIO::pointer++; 
  }

  return res;
}

FXival SimpleFileStorage::writeEntry( const FXArray<FXString> &buff )
{
  FXlong res = FXIO::Error;
  FXlong num = buff.no( );
  
  if( m_ready && num > 0 ) {
    FXString data = "";
    FXString sep = ";";
    FXint i;
    for( i = 0; i != num; i++ ) { 
      if( i > 0 ) { data += sep; }
      data += buff[ i ];
    }      
    m_entries.push( data );
    FXIO::pointer = m_entries.no( ) - 1;
    res = i;
  }

  return res; 
} 

FXint SimpleFileStorage::eof( )
{
  FXint res = FXIO::Error;

  if( m_ready ) { res = ( FXIO::pointer < m_entries.no( ) ); }  

  return res;
}

FXbool SimpleFileStorage::flush( )
{
  FXbool res = false;
  
  FXFile store( m_path + PATHSEPSTRING + getName( ), FXIO::WriteOnly );
  if( store.isOpen( ) ) {
    FXString buff = "";
    FXint i, num = m_entries.no( );    

    for( i = 0; i != num; i++ ) { buff += m_entries[ i ] + "\n"; }
    
    num = buff.length( );
    if( num > 0 ) {
      store.truncate( 0 );
      store.writeBlock( buff.text( ), num );
      res = store.flush( );
    }
    else { res = false; }

    store.close( );    
  } 

  return res;
}

FXbool SimpleFileStorage::close( )
{
  FXbool res = false;

  if( m_ready ) {
    m_entries.clear( );
    setName( FXString::null );
    m_ready = false;
    res = true;
  } 
  
  return res;
}

/*** END ******************************************************************************************/
