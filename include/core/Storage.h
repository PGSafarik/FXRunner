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
#ifndef FXRUNNER_Storage_H
#define FXRUNNER_Storage_H

/*************************************************************************
* Storage.h                                                          *
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
	void set_str( const FXString &str ) { m_str = str; m_num = m_str.contains( m_delim ); }

  FXint get_num( ) { return m_num; }
	FXint get_index( ) { return m_index; }
	void set_index( FXint index ) { m_index = index; }
	FXint get_state( ) { return m_state; }
	void set_state( FXint state ) { m_state = state; }

  /* Operations Methods */
	void clear( ) { m_str.clear( ); m_num = 0; m_pos = 0; }

	/* operators */
	SubstrStream& operator++( ) { m_index++; return *this; }

	SubstrStream& operator >> ( FXString &str );
	SubstrStream& operator >> ( FXbool &value );
	SubstrStream& operator << ( const FXString &str );
	SubstrStream& operator << ( FXbool value );
};

template<typename STREAMER, typename CLIENT > class Storage : protected FXFile {
public:
	explicit Storage( const FXString &filename, FXuint mode = FXIO::Reading );

	/* Access */
	using FXFile::isOpen;

	/* Operations */
	using FXFile::close;
	FXint read( CLIENT *history );
	FXint write( CLIENT *history );
};

template<typename STREAMER, typename CLIENT> Storage<STREAMER, CLIENT>::Storage( const FXString &filename, FXuint mode )
					 : FXFile( filename, mode, FXIO::AllReadWrite )
{ }

template<typename STREAMER, typename CLIENT> FXint Storage<STREAMER, CLIENT>::read( CLIENT *history )
{
	if( !isOpen( ) ) { return -1; }
	DEBUG_OUT( "Read history list" )

	FXlong fsize = size( );
	if( fsize == 0 ) { return -2; }

	FXint count = 0;
	FXString buffer;
	buffer.length( fsize );

	history->clear( );

	if( readBlock( buffer.text( ), fsize ) == fsize ) {
		FXint num   = buffer.contains( '\n' );
		for( FXint i = 0; i != num; i++ ) {
			FXString line = buffer.section( '\n', i );
			line.trim( );
			if( !line.empty( ) ) {
				STREAMER substrs( line, ";" );
				history->load_data( substrs );
				count++;
			}
		}
	}
	else { return -3; }

	return count;
}

template<typename STREAMER, typename CLIENT> FXint Storage<STREAMER, CLIENT>::write( CLIENT *client )
{
	if( !isOpen( ) ) { return -1; }
	if( !client->isChange( ) ) { return -4; }

	FXString buffer = "";
	STREAMER stream;

  while( stream.get_state( ) == 0 ) {
    stream.clear( );
  	if( client->save_data( stream ) ) {
  		buffer += stream.get_str( ) + "\n";
  	}
  	++stream;
  }

	FXint size  = 0;
 	if( ( size = buffer.length( ) ) > 0 ) {
		DEBUG_OUT( "Write history list" )
		truncate( 0 );
		if( writeBlock( buffer.text( ), size ) != size ) { return -5; }
		flush( );
	}

	return 0;
}


#endif /* FXRUNNER_Storage_H */
