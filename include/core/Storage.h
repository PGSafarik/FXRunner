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
* Storage.h                                                              *
*                                                                        *
* The class, represent a file for recording the commands history         *
* Copyright (c) 2025 D.A.Tiger <drakarax@seznam.cz>                      *
*************************************************************************/
#include "defs.h"

/* CSV STREAMER */
class SubstrStream  {
	FXString m_str;      // CSV line String
	FXString m_delim;    // Delimiter
	FXint    m_num;      // Number of substrings
  FXint    m_pos;      // Indicate actual substring position
	FXint    m_index;    // Index of working record in a source of data
	FXint    m_state;    // Status of data transfer between client and storage ( 0 - OK, < 0 - ERROR, 1 - END, > 1 - Other message )

	FXString GetSection( );

public:
  explicit SubstrStream( const FXString &delimiter = ";" );
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

	/*
	void Dump( ) {
    std::cout << m_str << "[ ";
		std::cout << m_num << ";";
    std::cout << m_index << ";";
		std::cout << m_state << ";";
		std::cout << " ]" << std::endl;
	}
	*/
};

/*** Storage **************************************************************************************/
template<typename STREAMER, typename CLIENT > class Storage : protected FXFile {
	FXString m_filename;	// Path of the file of the storage
	FXbool   m_ready;			// Indication that all conditions for proper storage function are met

public:
	explicit Storage( const FXString &filename = FXString::null );
  ~Storage( ) override = default;

	/* Access */
	using FXFile::isOpen;
	FXbool ready( ) const { return m_ready; }
  FXbool set_filename( const FXString &filename );
	FXString get_filename( ) { return m_filename; }

	/* Operations */
	void operator <<( CLIENT &client );
	void operator >>( CLIENT &client );
};

template<typename STREAMER, typename CLIENT> Storage<STREAMER, CLIENT>::Storage( const FXString &filename )
{
  set_filename( filename );
}

template<typename STREAMER, typename CLIENT> FXbool Storage<STREAMER, CLIENT>::set_filename( const FXString &filename )
{
	m_filename = filename;
  m_ready = false;

	// Checking the conditions for proper storage function
	if( !m_filename.empty( ) ) {
		if( FXStat::exists( m_filename ) ) {
		  if( FXStat::isFile( m_filename ) ) {
			  m_ready = ( FXStat::isReadable( m_filename ) && FXStat::isWritable( m_filename ) );
		  }
		}
		else {
		  FXString dir = FXPath::directory( m_filename );
			m_ready = ( FXStat::isReadable( dir ) && FXStat::isWritable( dir ) );
		}
	}

	return m_ready;
}

template<typename STREAMER, typename CLIENT> void Storage<STREAMER, CLIENT>::operator >>( CLIENT &client )
{
	STREAMER pipe;

	if( !m_ready || !open( m_filename, FXIO::Reading, FXIO::AllReadWrite ) ) {
		pipe.set_state( -1 );
		client.load_data( pipe );
		return;
	}
	DEBUG_OUT( "Loading a data from store file: " << m_filename.text( ) )

	FXlong fsize = size( );
	if( fsize == 0 ) {
		pipe.set_state( -2 );
		client.load_data( pipe );
		close( );
		return;
	}

	FXString buffer;
	buffer.length( fsize );

	if( readBlock( buffer.text( ), fsize ) == fsize ) {
		pipe.set_state( 0 );
		FXint num = buffer.contains( '\n' );
		while( pipe.get_index( ) < num ) {
			FXString line = buffer.section( '\n', pipe.get_index( ) );
			line.trim( );
			if( !line.empty( ) ) {
				pipe.clear( );
				pipe.set_str( line );
				client.load_data( pipe );
			}
			++pipe;
		}
	}
	else {
		pipe.set_state( -3 );
		client.load_data( pipe );
		close( );
		return;
	}

	close( );
}

template<typename STREAMER, typename CLIENT> void Storage<STREAMER, CLIENT>::operator <<( CLIENT &client )
{
	if( !client.isChange( ) ) { return; }
	STREAMER pipe;

	if( !m_ready || !open( m_filename, FXIO::Writing | FXIO::Create, FXIO::AllReadWrite ) ) {
		pipe.set_state( -1 );
		client.save_data( pipe );
		return;
	}

	FXString buffer = "";
	while( pipe.get_state( ) == 0 ) {
		pipe.clear( );
		client.save_data( pipe );
		if( pipe.get_state( ) == 0 ) { buffer += pipe.get_str( ) + "\n"; }
		++pipe;
	}

	FXint size  = 0;
	if( ( size = buffer.length( ) ) > 0 ) {
		DEBUG_OUT( "Saving a data to store file: " << m_filename.text( ) )
	  truncate( 0 );
		if( writeBlock( buffer.text( ), size ) != size ) {
			pipe.set_state( -5 );
			close( );
			return;
		}
		flush( );
	}

	close( );
}

#endif /* FXRUNNER_Storage_H */
