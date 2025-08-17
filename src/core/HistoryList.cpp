//
// Created by gabriel on 08.08.25.
//
#include<core/HistoryList.h>
HistoryList::HistoryList( FXint limit )
             : m_limit( limit ), m_change( false )
{ }

Task* HistoryList::insert( FXint pos, Task *entry, FXbool change )
{
  if( check_position( pos ) && entry && !entry->cmd.empty( ) ) {
    Deduplication( entry );
    if( FXArray<Task*>::insert( pos, entry ) ) {
      CheckLimit( );
      if( change ) { m_change = true; }
      return entry;
    }
  }

  return nullptr;
}

Task* HistoryList::remove( FXint pos, FXbool destroy, FXbool change )
{
  if( !check_position( pos ) ) { return nullptr; }
  Task *entry = at( pos );

  if( erase( pos ) ) {
    if( change ) { m_change = true; }
    if( destroy && entry ) {
      delete entry;
      return nullptr;
    }
  }

  return entry;
}

void HistoryList::clear( )
{

}

FXbool HistoryList::top( FXint pos )
{
  if( check_position( pos ) && pos > 0 ) {
    Task *entry = at( pos );
    if( remove( pos, false, false ) && insert( 0, entry ) ) { return true; }
  }
  return false;
}

FXbool HistoryList::check_position( FXint value ) const
{
  FXint num = FXArray<Task*>::no( );
  return ( num > 0 ?  0 <= value && value <= num : value == 0 );
}

/**********************************************************************************************************************/
void HistoryList::Deduplication( const Task *entry, const FXint start )
{
  if( !check_position( start ) ) { return; }

  FXint num = no( );
  for( FXint i = start; i < num; i++ ) {
    Task *tmp = at( i );
    if (tmp && tmp != entry && *tmp == *entry) { remove( i , true ); } // if: 'Entry' must exist, 'Entry' and 'tmp' must not be the same object, and both must have the same values
  }
}

void HistoryList::CheckLimit( )
{
  if ( m_limit > 0 ) {
    FXint num = FXArray<Task*>::no( );
    while( num < m_limit ) { num--; remove( num, true ); }
  }
}


