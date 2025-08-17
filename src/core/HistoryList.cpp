//
// Created by gabriel on 08.08.25.
//
#include<core/HistoryList.h>
HistoryList::HistoryList( )
             : m_change( false )
{ }

Task* HistoryList::insert( FXint pos, Task *entry, FXbool change )
{
  if( check_position( pos ) && entry && !entry->cmd.empty( ) ) {
    Deduplication( entry );
    if( FXArray<Task*>::insert( pos, entry ) ) {
      Truncate( );
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

void HistoryList::clear( FXbool change )
{
  FXival num = no( );

  while ( num > 0 ) {
    remove( static_cast<FXint>( num - 1 ), true, false  );
    num = no( );
  }
  FXArray<Task*>::clear( );

  m_change = change;
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

void HistoryList::cleaning( )
{
  // Check of size limits
  Truncate( );

  FXint num = static_cast<FXint>( no( ) );
  for( FXint i = 0; i < num; i++ ) {
    Task *entry = at( i );
    // Check empty record
    if( !entry || entry->cmd.empty( )  ) {
      remove( i, true, true );
      break;
    }
    // check duplications
    Deduplication( entry );
  }
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

void HistoryList::Truncate( )
{
  FXint count = 0;                            // Pocet zaznamu (od konce), ktere maji byt smazany
  FXint size  = static_cast<FXint>( no( ) );  // Aktualni pocet zaznamu v seznamu

  if( m_limit > 0 && m_limit <= size ) {  // Povolena limitace poctu radku a zaroven je dosazena, ci prekrocena jeho hodnota
    if( m_hysteresis == 0 ) {  // hysterze neni povolena, udrzuje se za vse okolnosti maximalni pocet zaznamu v seznamu
      count = size - m_limit;
    }
    else if( m_hysteresis > 0 && ( size == m_limit + m_hysteresis) ) { // Kladna hysterze - Pocka se az pocet zaznamu naroste na urcitou mez nad zadany limit a pak se zkrati na nastveny limit
      count = m_hysteresis;
    }
    else if( m_hysteresis < 0 ) { // Zaporna hysterze - smaze se vse co je nadlimit + yvoleny pocet zaznamu pod limitem
      count = -m_hysteresis + ( size - m_limit );
    }

    while( count > 0 ) {
      remove( static_cast<FXint>( no( ) - 1 ), true, false  );
      count--;
    }
  }
}

void HistoryList::CheckLimit( )
{
  if ( m_limit > 0 ) {
    FXint num = FXArray<Task*>::no( );
    while( num < m_limit ) { num--; remove( num, true ); }
  }
}


