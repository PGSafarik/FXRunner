/*************************************************************************
* HistoryTools.cpp Copyright (c) 2015 - 2025 by D.A.Tiger                        *
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
#include<core/HistoryTools.h>

/**************************************************************************************************/
HistoryBuffer::HistoryBuffer( )
            : m_change( false )
{ }

/**************************************************************************************************/
Task* HistoryBuffer::insert( FXint pos, Task *entry, FXbool change )
{
  if( check_position( pos ) && entry && !entry->get_cmd( ).empty( ) ) {
    Deduplication( entry );
    if( FXArray<Task*>::insert( pos, entry ) ) {
      Truncate( );
      m_change = change;   // note: result is same like for: if( change ) { m_change = true; }
      return entry;
    }
  }

  return nullptr;
}

Task* HistoryBuffer::remove( FXint pos, FXbool destroy, FXbool change )
{
  if( !check_position( pos ) ) { return nullptr; }
  Task *entry = at( pos );

  if( erase( pos ) ) {
    m_change = change;
    if( destroy && entry ) {
      delete entry;
      return nullptr;
    }
  }

  return entry;
}

void HistoryBuffer::clear( FXbool change )
{
  FXival num = no( );

  while ( num > 0 ) {
    remove( static_cast<FXint>( num - 1 ), true, false  );
    num = no( );
  }
  FXArray<Task*>::clear( );

  m_change = change;
}

FXbool HistoryBuffer::top( FXint pos )
{
  if( check_position( pos ) && pos > 0 ) {
    Task *entry = at( pos );
    if( remove( pos, false, false ) && insert( 0, entry ) ) { return true; }
  }
  return false;
}

FXbool HistoryBuffer::check_position( FXint value ) const
{
  FXint num = static_cast<FXint>( no( ) );
  return ( num > 0 ?  0 <= value && value <= num : value == 0 );
}

void HistoryBuffer::cleaning( )
{
  // Check of size limits
  Truncate( );

  FXint num = static_cast<FXint>( no( ) );
  for( FXint i = 0; i < num; i++ ) {
    Task *entry = at( i );
    // Check empty record
    if( !entry || entry->get_cmd( ).empty( )  ) {
      remove( i, true, true );
      break;
    }
    // check duplications
    Deduplication( entry );
  }
}

/**********************************************************************************************************************/
void HistoryBuffer::Deduplication( const Task *entry, const FXint start )
{
  if( !check_position( start ) ) { return; }

  FXint num = no( );
  for( FXint i = start; i < num; i++ ) {
    Task *tmp = at( i );
    if (tmp && tmp != entry && *tmp == *entry) { remove( i , true ); } // if: 'Entry' must exist, 'Entry' and 'tmp' must not be the same object, and both must have the same values
  }
}

void HistoryBuffer::Truncate( )
{
  FXint overload_num = CheckLimit( );

  while( 0 < overload_num ) {
    remove( static_cast<FXint>( no( ) - 1 ), true, false  );
    overload_num--;
  }
}

FXint HistoryBuffer::CheckLimit( )
{
  FXint count = 0;
  FXint size  = static_cast<FXint>( no( ) );

  if( limit > 0 && limit <= size ) {
    if( hysteresis == 0 ) {  // No hysteresis
      count = size - limit;
    }
    else if( hysteresis > 0 && ( size == limit + hysteresis) ) { // Positive hysteresis
      count = hysteresis;
    }
    else if( hysteresis < 0 ) { // Negative hysteresis
      count = -hysteresis + ( size - limit );
    }
  }
  DEBUG_OUT( "History list count of overload: " << limit << "/" << hysteresis << " => " << size << "-" << count << ( size == limit ? " *" : "" ) )
  return count;
}
/*** END ******************************************************************************************/
