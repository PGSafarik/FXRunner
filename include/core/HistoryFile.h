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
#ifndef FXRUNNER_HISTORYFILE_H
#define FXRUNNER_HISTORYFILE_H

/*************************************************************************
* HistoryFile.h                                                          *
*                                                                        *
* The class, represent a file for recording the commands history         *
* Copyright (c) 2025 D.A.Tiger <drakarax@seznam.cz>                      *
*************************************************************************/
#include "defs.h"
#include "History.h"

class SubstrStream  {
	FXString m_str;      // CSV line String 
	FXString m_delim;    // Delimiter
	FXint    m_num;      // Number of substrings (only for reading?)
	FXint    m_index;    // Index of actual substring (only for reading?)
	
	FXString GetSection( );
	
public:
  SubstrStream( const FXString &delimiter );  
  SubstrStream( const FXString &str, const FXString &delimiter );
	~SubstrStream( ); 

  /* Access methods */	
	FXString get_str( ) { return m_str; }
	
	/* operators */
	SubstrStream& operator >> ( FXString &str );
	SubstrStream& operator >> ( FXbool &value );
	SubstrStream& operator << ( const FXString &str );
	SubstrStream& operator << ( FXbool value ); 
};


class HistoryFile : protected FXFile {
public:
  explicit HistoryFile( const FXString &filename, FXuint mode = FXIO::Reading );

	using FXFile::isOpen;
	using FXFile::close;

  FXint readHistory( History *history );
  FXint writeHistory( History *history );
};

#endif /* FXRUNNER_HISTORYFILE_H */
