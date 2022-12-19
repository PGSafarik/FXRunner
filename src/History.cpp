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

History_b::History_b( FXint num, FXuint opts, const FXString &cache_dir, const FXString &name ) : h_name( name ), h_num( num ), h_hash( 0 ), h_change( false )
{ 
  h_storage = new SimpleFileStorage( cache_dir );
  if( !h_storage->open( name, ";", "", 1 ) ) { std::cerr << "[ERROR] Unable to open a storage " << name.text( ) << std::endl; } 
}

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

FXbool History_b::write( )
{
  FXArray<FXString> buff; 

  if( h_storage->isOpen( ) ) {
    for( FXint i = 0; i != this->no( ); i++ ) {
      buff.clear( );
      buff.push( this->at( i ) );  
      h_storage->writeEntry( buff );
    } 
    h_storage->flush( );
  }    

  h_change = false;
  return true;
}

FXbool History_b::read( )
{
  if( h_storage->isOpen( ) ) {
   while( h_storage->eof( ) > 0 ) { h_storage->readEntry( *this ); } 
  }

  return true;
}

/*** END ******************************************************************************************/
