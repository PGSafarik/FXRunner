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

/* CSV STREAMER */
class SubstrStream  {
	FXString m_str;      // CSV line String
	FXString m_delim;    // Delimiter
	FXint    m_num;      // Number of substrings (only for reading?)
	FXint    m_index;    // Index of working record in a source of data
	FXint    m_state;    // Status of data transfer between client and storage ( 0 - OK, < 0 - ERROR, 1 - END, > 1 - Other message )
	FXString GetSection( );

public:
  explicit SubstrStream( const FXString &delimiter = ";" );
  SubstrStream( const FXString &str, const FXString &delimiter );
	~SubstrStream( );

  /* Access methods */
	FXString get_str( ) { return m_str; }
  FXint get_num( ) { return m_num; }
	FXint get_index( ) { return m_index; }
	void set_index( FXint index ) { m_index = index; }
	FXint get_state( ) { return m_state; }
	void set_state( FXint state ) { m_state = state; }

  /* Operations Methods */
	void clear( ) { m_str.clear( ); }

	/* operators */
	SubstrStream& operator >> ( FXString &str );
	SubstrStream& operator >> ( FXbool &value );
	SubstrStream& operator << ( const FXString &str );
	SubstrStream& operator << ( FXbool value );
};


class HistoryFile : protected FXFile {
public:
  explicit HistoryFile( const FXString &filename, FXuint mode = FXIO::Reading );

	/* Access */
	using FXFile::isOpen;

	/* Operations */
	using FXFile::close;
  FXint read( History *history );
  FXint write( History *history );
};

#endif /* FXRUNNER_HISTORYFILE_H */
