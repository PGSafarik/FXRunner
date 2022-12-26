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
#include<History_b.h>

History_b::History_b( FXint num, FXuint opts ) : h_limit( num ), h_change( false )
{ }

History_b::~History_b( )
{ clear( ); }

/**************************************************************************************************/
void History_b::dump( )
{
  std::cout << "Dumping history array: " << " with " << no( ) << " items" << std::endl;
  for( FXint i = 0; i != no( ); i++ ) { std::cout << at( i ) << std::endl; }
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

FXbool History_b::save( Storage &storage )
{
  FXArray<FXString> buff; 
  
  if( storage.isOpen( ) ) {
    for( FXint i = 0; i != this->no( ); i++ ) {
      buff.clear( );
      buff.push( this->at( i ) );  
      storage.writeEntry( buff );
    } 
    storage.flush( );
    h_change = false;
  }
  else { std::cerr << "[ERROR] Unable to open a history storage " << ( storage.getUri( ) + "/" + storage.getName( ) ) << std::endl; }    

  return true;
}

FXbool History_b::load( Storage &storage )
{
  if( storage.isOpen( ) ) {
    while( storage.eof( ) > 0 ) { 
     storage.readEntry( *this ); 
    }
    h_change = false;
  }
  else { std::cerr << "[ERROR] Unable to open a history storage " << ( storage.getUri( ) + "/" + storage.getName( ) ) << std::endl; }

  return true;
}

/*** END ******************************************************************************************/
