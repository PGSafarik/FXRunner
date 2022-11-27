/*************************************************************************
* History.cpp Copyright (c) 2019 - 2022 by  D.A.Tiger                     *
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
#include<History.h>

History_b::History_b( FXint num, const FXString name ) : h_name( name ), h_num( num ), h_hash( 0 ), h_change( false )
{ }

History_b::~History_b( )
{ clear( ); }

/**************************************************************************************************/

void History_b::dump( )
{
  std::cout << "Dumping array: " << h_name.text( ) << " with " << no( ) << " items" << std::endl;
  for( FXint i = 0; i != no( ); i++ ) { std::cout << at( i ).text( ) << std::endl; }
}

void History_b::insert( const FXString &data )
{
  FXint size_buff = no( );
  for( FXint i = 0; i != size_buff; i++ ) { if( data == at( i ) ) { erase( i ); } }

  push( data );
  h_change = true;

}

void History_b::_clear( )
{
  h_change = true;
  this->clear( );
}

FXbool History_b::write( const FXString &cache_dir, FXbool mk )
{
  FXString tmp = "";

  if( h_change == false ) { return true; }
  if( !cache_dir.empty( ) ) {
    FXString store = cache_dir + "/" + h_name; 
    if( FXStat::exists( store ) == false ) {
      if( mk ) { FXFile::create ( store ); }
      else {
        std::cout << "Cache store file " << store.text( ) << " not found!" << std::endl;
        return false;
      }
    }

    for( FXint i = 0; i != no( ); i++ ) { tmp += at( i ) + "\n"; }

    FXFile fd;
    if( tmp.empty( ) ) { std::cout << "Cache is empty!" << std::endl; }
    if( fd.open( store, FXIO::Writing ) == true ) {
     // std::cout << "save cache " << h_name.text( ) << ": " << store.text( ) << std::endl;
      DEBUG_OUT( "SAVE cache " << h_name.text( ) << ": " << store.text( ) )
      FXint n = fd.writeBlock( tmp.text( ), tmp.length( ) );
      //std::cout << "write data of size: " << n << std::endl;
      DEBUG_OUT( "write data of size: " << n )
      fd.close( );
    }
    else {
      std::cout << "Cache store file " << store.text( ) << " not open!" << std::endl;
      return false;
    }
  }

  h_change = false;
  return true;
}

FXbool History_b::read( const FXString &cache_dir )
{
  
  if( !cache_dir.empty( ) ) {
    FXString store = cache_dir + "/" + h_name;
    FXint    i;
    FXString tmp, data;
    FXFile   fd;

    if( no( ) > 0 ) {
      clear( );
      h_hash = 0;
    }

    if( FXStat::exists( store ) == true ) { 
      if( fd.open( store ) != true ) { std::cout << "History store file is not open!" << std::endl; return false; }
      //std::cout << "READ cache \'" << h_name.text( ) << "\': " << store.text( ) << std::endl;
      DEBUG_OUT( "READ cache \'" << h_name.text( ) << "\': " << store.text( ) )
      tmp.length( fd.size( ) );
      if( fd.readBlock( tmp.text( ), fd.size( ) ) > 0 ) {
        for( i = 0; i != tmp.length( ); i++ ) {
          char c = tmp[ i ];
          if( c == '\n' ) { FXArray<FXString>::push( data ); data = ""; }
          else { data.insert( data.length( ), c ); }
        }
        if( !data.empty( ) ) { FXArray<FXString>::push( data ); }
      }
      else { std::cout << "History store file is not read!" << std::endl; return false; }
      fd.close( );
    }
    else { std::cout << "History store file not exist!" << std::endl; return false; }
  }
  return true;
}

/*** END ******************************************************************************************/
