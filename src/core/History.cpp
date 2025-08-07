/*************************************************************************
* Task.cpp Copyright (c) 2015 - 2025 by D.A.Tiger                        *
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
#include "core/History.h"

FXDEFMAP( History ) HISTMAP[ ] = { };
FXIMPLEMENT( History, FXObject, NULL, 0 )

/*************************************************************************************************/
History::History( FXint limit, FXuint opts ) : m_limit( limit ), m_change( false )
{  }

History::~History( )
{ }

/*************************************************************************************************/
Task* History::at( FXint index, FXbool noup )
{ 
  Task *entry = NULL;

  if( this->Index( index ) ) {
    if( noup && __top( index ) ) { entry = m_buffer[ 0 ]; } else { entry = m_buffer[ index ]; }
  }
  

  return entry;
}

FXbool History::add( const FXString &cmd_str )
{
  /// FIXME HISTORY_002 : Remove m_add_empty flag 
  if( !cmd_str.empty( ) || m_add_empty ) { 
   
    Task *task = new Task( cmd_str );
    if( task && __add( task ) ) { return true; } else { delete task; } 
  }

  return false;    
}

FXbool History::push( Task *task, FXbool ch_state )
{
  if( task == NULL || ( task->cmd.empty( ) && !m_add_empty ) ) { return false; } 
  return __add( task, ch_state ); 	
}

FXbool History::insert( Task *task, FXint pos )
{
  /// FIXME HISTORY_002 : Remove m_add_empty flag
  if( task == NULL || ( task->cmd.empty( ) && !m_add_empty ) ) { return false; } 
  return __insert( task, pos ); 
}

Task* History::remove( FXint index )
{
  if( this->Index( index ) ) {
    Task *t = m_buffer[ index ];  
    if( __rem( index ) ) { return t; }
  }
  
  return NULL;  
}

void History::clear( )
{
  FXint num = m_buffer.no( );
  
  while( num > 0 ) {
    num--;
    __rem( num, true );
  }  

  m_buffer.clear( );
}


/**************************************************************************************************/
void History::Dump( )
{
  FXint num = m_buffer.no( );
  std::cout << "[DUMP History] count: " << num << "; limit: " << m_limit << std::endl;

  if( num > 0 ) {
    for( FXint i = 0; i != num; i++ ) {
      std::cout << i << ". ";

      Task *task = m_buffer[ i ];
      if( task ) {
        std::cout << task->cmd  << "; ";
        std::cout << task->wpth << "; ";
        std::cout << task->prop->suaccess   << "; ";
        std::cout << task->prop->term;
      }
      else { std::cout << "- This entry is EMPTY! -"; }

      std::cout << std::endl;     
    }
  }
  else {  std::cout << "  - History is empty -  " << std::endl; }
  std::cout << "--- End History object dump ---" << std::endl;

}

/**************************************************************************************************/
FXbool History::__add( Task *entry, FXbool ch )
{
	if( !entry->cmd.empty( ) ) {
		__dedupl( entry );
    if( m_buffer.push( entry ) ) {
			__limit( );        
      if( ch ) { m_change = true; }  /// FIXME HISTORY_002 : Add optional flag for disable change the value of the m_change 
      return true;
		}
  }
  
  return false;
}

FXbool History::__insert( Task *entry, FXint pos, FXbool ch )
{
	if( !entry->cmd.empty( ) ) {
		__dedupl( entry );
    if( m_buffer.insert( pos, entry ) ) {
			__limit( );        
      if( ch ) { m_change = true; } 
      return true;
		}
  }
  
  return false;
}

FXbool History::__rem( FXint pos, FXbool destroy )
{
   Task *entry = ( destroy ? m_buffer[ pos ] : NULL );
   
   if( m_buffer.erase( pos ) ) {
     m_change = true;
     if( entry ) { delete entry; } 
     return true;  
   }
   
   return false;
}

FXbool History::__top( FXint index )
{
  if( index == 0 ) { return true; }  
  if( index > 0 ) {  
    Task *tmp = m_buffer[ index ];
    if( __rem( index ) && __add( tmp, false ) ) { 
      return true; 
    }
  }
  
  return false;
}

void History::__dedupl( Task *entry, FXint start )
{
	// Deduplication
  FXint num = m_buffer.no( );
  for( FXint index = start; index != num; index++ ) {
    Task *tmp = m_buffer[ index ];
    if( tmp && tmp != entry && *tmp == *entry ) { __rem( index, true ); }  // They must exist, they must not be the same object, but they must also match in values.
  }  
}

void History::__limit( )
{
  if( m_limit > 0 ) {
    FXint num = m_buffer.no( );
    while( num < m_limit ) { num--; __rem( num, true ); }
  }
}

/*************************************************************************************************/
FXbool History::Index( FXint value )
{
  return ( value >= 0 && value < m_buffer.no( ) );   
}


/*** END *****************************************************************************************/
