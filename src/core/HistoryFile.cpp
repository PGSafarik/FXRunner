/*************************************************************************
* HistoryFile.cpp Copyright (c) 2017 - 2024 by  D.A.Tiger                *
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
#include "core/HistoryFile.h"

/**************************************************************************************************/
FXString SubstrStream::GetSection( )
{
	FXString substr;
	substr = m_str.section( m_delim, m_index );
	m_index++;
		
	return substr;
}

SubstrStream::SubstrStream( const FXString &delimiter ) : m_delim( delimiter )  
{ 
  m_index = 0;
  m_num   = 0;
	m_state = 0;
}	
	
SubstrStream::SubstrStream( const FXString &str, const FXString &delimiter ) : m_str( str ), m_delim( delimiter )
{
	m_state = 0;
	m_index = 0;
	m_num   = str.contains( m_delim );
}
	
SubstrStream::~SubstrStream( ) 
{ }
	
SubstrStream& SubstrStream::operator >> ( FXString &str ) 
{
	str = GetSection( );
	return *this;
}
	
SubstrStream& SubstrStream::operator >> (  FXbool &value ) 
{
	FXString str = GetSection( );
	value = ( ( str == "1" || str == "true" ) ? true : false );
	return *this;
}
	
SubstrStream& SubstrStream::operator << ( const FXString &str ) 
{
	if( !m_str.empty( ) ) { m_str += m_delim; }
	m_str += str;
	return *this;
}
	
SubstrStream& SubstrStream::operator << ( FXbool value ) 
{
	if( !m_str.empty( ) ) { m_str += m_delim; }
	m_str += ( ( value ) ?  "true": "false" );
	return *this;
}

/*** END ******************************************************************************************/
