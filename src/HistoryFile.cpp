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
#include<HistoryFile.h>

HistoryFile::HistoryFile( const FXString &filename, FXuint m, FXuint perm ) : FXFile( filename, m, perm )
{ }
	
FXint HistoryFile::readHistory( History *history )
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
        Task *n_task = new Task;
				n_task->load( line );
				history->push( n_task, false );
				count++;
      }           
    }
  } 
	else { return -3; }
		
	return count;
}

FXint HistoryFile::writeHistory( History *history )
{
	FXint size  = 0;
		
	if( !isOpen( ) ) { return -1; }
		
	FXint num = history->no( );
	if( ( num == 0 ) || ( !history->isChange( ) ) ) { return -4; }
		
	FXString buffer = "";
		
	for( FXint i = 0; i != num; i++ ) {
		FXString line;
    history->at( i )->save( line );
    buffer += line + "\n";  
	}
		
	if( ( size = buffer.length( ) ) > 0 ) {
		DEBUG_OUT( "Write history list" )
		truncate( 0 );
		if( writeBlock( buffer.text( ), size ) != size ) { return -5; }
		flush( );
	}
     
	return 0;
}

/*** END ******************************************************************************************/
