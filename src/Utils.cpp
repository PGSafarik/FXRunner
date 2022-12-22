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
#include<main.h>
#include<string>


/*** String utils *********************************************************************************/
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
