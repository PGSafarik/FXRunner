/*************************************************************************
* Utils.cpp Copyright (c) 2022 by  D.A.Tiger                             *
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
#include "core/StringUtils.h"

/*** String list class ****************************************************************************/
FXString FXStringList::join( const FXString &sep, FXint n )
{
  FXString str;

  if( n <= 0 ) { n = no( ); }
  else if ( n > no( ) ) { n = no( ); }

  for( int i = 0; i != n; i++ ) {
    if( i > 0 ) { str += sep; }
    str += at( i );
  }

  return str;
}

FXint FXStringList::split( const FXString &str, const FXString &sep )
{
  if( str.empty( ) ) { return 0; }

  int num, count;
  count = num = 0;

  if( !sep.empty( ) ) {
    num = str.contains( sep );
    if( str.right( 1 ) != sep ) { num += 1; }

    for( int i = 0; i != num; i++ ) {
      FXString substr = str.section( sep, i );

      if( i == 0 && substr.empty( ) ) { continue; }
      if( !substr.empty( ) ) { substr.trim( ); }

      push( substr );
      count++;
    }
  }
  else {
    push( str );
    count = 1;
  }

  return count;
}

void FXStringList::sort( )
{
  ::qsort( data( ), no( ), sizeof( FXString ), [](const void* a, const void* b)
  {
    const FXString _a = *static_cast<const FXString*>( a );
    const FXString _b = *static_cast<const FXString*>( b );

    if( _a != _b ) {
      if( _a < _b ) { return -1; }
      else { return 1; }
    }

    return 0;
  });

}

/*** String stream operators **********************************************************************/
FX::FXString& operator <<( FX::FXString &dest, const std::string &source ) {
  if( !source.empty( ) ) {
    int len = source.size( );
    dest.length( len );
    source.copy( dest.text( ), len ); 
  }   
  else { dest = FXString::null; }

  return dest;
}

std::string& operator <<( std::string &dest, const FX::FXString &source ) {
  if( !source.empty( ) ) { dest.assign( source.text( ) ); } 
  else { dest = ""; }
  
  return dest;
}

std::ostream& operator <<( std::ostream &store, const FX::FXString &str  ) {
  return store << str.text( );
}

std::istream& operator >>( std::istream &store, FX::FXString &str  ) {
  std::string s;

  store >> s;
  str << s;

  return store;
}

/*** END ******************************************************************************************/
