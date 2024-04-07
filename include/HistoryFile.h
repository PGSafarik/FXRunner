/*************************************************************************
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
#ifndef __HISTORYFILE_H
#define __HISTORYFILE_H
/*************************************************************************
* HistoryFile.h                                                          *
*                                                                        *
* The class, represent a file for recording the commands history         *
* Copyright (c) 05/04/2024 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<History.h>

class SubstrStream  {
	FXString m_str;      // Retezec
	FXString m_delim;    // Oddelovac
	FXint    m_num;      // Pocet subretezcu
	FXint    m_index;    // Aktualni pozice
	
	FXString GetSection( );
	
public:
  SubstrStream( const FXString &delimiter ) : m_delim( delimiter )  
	{ 
	  m_index = 0;
    m_num   = 0;  	
	}	
	
  SubstrStream( const FXString &str, const FXString &delimiter ) : m_str( str ), m_delim( delimiter )
	{ 
		m_index = 0;
		m_num   = str.contains( m_delim );
	}
	
	~SubstrStream( ) 
	{ }
	
	FXString get_str( ) { return m_str; }
	
	SubstrStream& operator >> ( FXString &str ) 
	{
		str = GetSection( );
		return *this;
	}
	
	SubstrStream& operator >> (  FXbool &value ) 
	{
		FXString str = GetSection( );
		value = ( ( str == "1" || str == "true" ) ? true : false );
		return *this;
	}
	
	SubstrStream& operator << ( const FXString &str ) 
	{
		if( !m_str.empty( ) ) { m_str += m_delim; }
		m_str += str;
		return *this;
	}
	
	SubstrStream& operator << ( FXbool value ) 
	{
		if( !m_str.empty( ) ) { m_str += m_delim; }
		m_str += ( ( value ) ?  "true": "false" );
		return *this;
	}	
};


class HistoryFile : public FXFile {	
public:
  HistoryFile(const FXString &filename, FXuint m = FXIO::Reading, FXuint perm = FXIO::AllReadWrite );
  FXint readHistory( History *history );
  FXint writeHistory( History *history );
};

#endif /* __HISTORYFILE_H */
