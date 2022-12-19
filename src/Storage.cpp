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
 
  if( !name.empty( ) ) {
    m_name = name;
    FXIO::access = m; 
    FXIO::pointer = 0L;
    res = true;
  }

  return res;
}

FXbool Storage::changeUri( const FXString &URI )
{
  FXbool resh = false;
  
  if( !URI.empty( ) ) {
    flush( );
    if( close( ) ) {
      m_uri = URI;
      resh = true;
    }
  }
  
  return resh;  
}

FXbool Storage::close( )
{
  FXbool res = false;
  
  if( FXIO::close( ) ) {
    m_name = FXString::null;
    res    = true;
  }  
 
  return res;
}


/*** SimpleFileStorage ****************************************************************************/

SimpleFileStorage::SimpleFileStorage( const FXString &path )
                 : Storage( "SimpleFileStorage" ), m_ready( false )
{ 
  Storage::changeUri( path );
}

SimpleFileStorage::~SimpleFileStorage( )
{ }

FXbool SimpleFileStorage::open( const FXString &name, const FXString &delim, const FXString &fill, FXint minel, FXuint m )
{
  FXbool res = false;
  
  close( );
  if( Storage::hasUri( ) && Storage::open( name, m ) ) {
    FXFile store( filename( ), FXIO::ReadOnly );
    if( store.isOpen( ) ) {
      FXString buff;
      FXlong size = store.size( );
      buff.length( size );
      if( store.readBlock( buff.text( ), size ) == size ) {
        FXint num = buff.contains( '\n' );
        for( FXint i = 0; i != num; i++ ) { 
          FXString line = buff.section( '\n', i );
          line.trim( );
          if( !line.empty( ) ) { 
            m_entries.push( line ); 
          }           
        }
        res = m_ready = true;
        m_delimiter = delim;
        m_filler    = fill;
        m_min       = minel; 
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
  
  if( eof( ) == 1 && m_entries.no( ) > 0 ) {
    FXString data = m_entries[ FXIO::pointer ];

    if( !data.empty( ) ) {
      FXint num = data.contains( m_delimiter );
      if( num > 0 ) {
        FXint i; 
        for( i = 0; i != num; i++ ) {
          buff[ i ] = data.section( m_delimiter, i );   
          buff[ i ].trim( );
        }
        res = i;
      }
      else { 
        FXString s = data;
        s.trim( );
        buff.push( s );
        res = 1;
      }
      
      for( FXint j = res; j < m_min ; j++ ) { buff.push( m_filler ); }
    }
    else { res = FXIO::Again; }
    
    m_entries.erase( FXIO::pointer );
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
  FXbool   res = false;
  FXString fn_str = filename( );

  if( m_ready && !fn_str.empty( ) ) {
    FXFile store( fn_str, FXIO::WriteOnly | FXIO::Create );
    if( store.isOpen( ) ) {
      FXString buff = "";
      FXString sep = "%";
      FXint i, num = m_entries.no( );    
      
      for( i = 0; i != num; i++ ) { 
        FXString entry = m_entries[ i ];
        entry.trim( );
        if( !entry.empty( ) ) { buff += entry + "\n"; } 
      }
    
      FXint size = buff.length( );
      if( num > 0 ) {
        store.truncate( 0 );
        store.writeBlock( buff.text( ), size );
        res = store.flush( );
      }
      else { res = false; }

      store.close( );    
    }
    else { std::cout << "[ ERROR ] Unable to open store file " << fn_str.text( ) << " for writing." << std::endl; }   
  }
  

  return res;
}

FXbool SimpleFileStorage::close( )
{
  FXbool res = false;

  if( Storage::close( ) ) {
    m_entries.clear( );
    m_ready = false;
    res = true;
  } 
  
  return res;
}

const FXString SimpleFileStorage::filename( ) 
{
  FXString fn = FXString::null;

  if( Storage::hasUri( ) && Storage::hasName( ) ) { 
    fn = Storage::getUri( ); 
    if( fn[ fn.length( ) ] != PATHSEP ) { fn += PATHSEPSTRING; }
    fn += Storage::getName( );
  }
  return fn;
}

/*** END ******************************************************************************************/
