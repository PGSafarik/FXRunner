#include<History.h>

FXMAPDEF( History ) HISTMAP[ ] = { };
FXIMPLEMENT( History, FXObject, NULL, 0 )

/*************************************************************************************************/
History::History( FXint lim = 0, FXuint opts = 0 ) : m_limit( lim ), m_change( false )
{  }

History::~History_b( )
{ }

/*************************************************************************************************/
Task* History::at( FXint index )
{ 
  return ( ( this->Index( index ) && __top( index ) ) ? m_buffer[ 0 ] : NULL );
}

FXbool History::add( const FXString &cmd_str )
{
  if( !cmd_str.empty( ) || m_add_empty ) { 
   
    Task *task = new Task;
    task->cmd = cmd_str; /// FIXME HISTORY_002 : Move to Task construktor
   
    if( task && __add( task ) ) { return true; } else { delete task; } 
  }

  return false;    
}

FXbool History::insert( Task *task )
{
  if( task == NULL || ( task->cmd.empty( ) && !m_add_empty ) ) { return false; } 
  return __add( task ); 
}

Task* History::remove( FXint index )
{
  if( this->Index( index ) ) {
    Task *t = buffer[ index ];  
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
  
FXint History::load( Storage &store )
{
  if( storage.isOpen( ) ) {
    clear( );  
    m_add_empty = true;  
    
    while( storage.eof( ) > 0 ) { 
      if( this->add( ) ) { m_buffer[ 0 ]->load( store ); }
    }
    
    m_add_empty = false;
    m_change    = false;
  }
  else { std::cerr << "[ERROR] Unable to open a history storage " << ( storage.getUri( ) + "/" + storage.getName( ) ) << std::endl; }

 return true;
}

FXint History::save( Storage &store )
{
  if( m_change && store.isOpen( ) ) {
    FXint num = m_buffer.no( );
    
    for( FXint i = 0; i != num; i++ ) {
      Task *entry = m_buffer[ i ];
      if( entry ) { entry->save( store ); }
    } 
    
    store.flush( );
    m_change = false;
  }
  else { std::cerr << "[ERROR] Unable to load a history storage " << ( store.getUri( ) + "/" + store.getName( ) ) << std::endl; }    

  return true;   
    
}

/**************************************************************************************************/
void History::Dump( )
{
  FXint num = m_buffer.no( );
  std::cout << "[DUMP History] count: " << num << "; limit: " << m_limit << std::endl;

  if( num > 0 ) {
    for( i = 0; i != num; i++ ) {
      std::cout << i << ". ";

      Task *task = m_buffer[ i ];
      if( task ) {
        std::cout << task->cmd  << "; ";
        std::cout << task->wpth << "; ";
        std::cout << task->su   << "; ";
        std::cout << task->te;
      }
      else { std::cout << "- This entry is EMPTY! -"; }

      std::cout << std::endl;     
    }
  }
  else {  std::cout << "  - History is empty -  " << std::endl;
  std::cout << "--- End History object dump ---" << std::endl;

}

/**************************************************************************************************/
FXbool History::__add( Task *entry, FXbool dedupl )
{
  if( m_buffer.insert( 0, task ) ) {
    if( dedupl && !task->cmd.empty( ) ) {  
      FXint num = m_buffer.no( );

      // Deduplication
      for( FXint i = 1; i != num; i++ ) {
        Task *tmp = m_buffer[ i ];
        if( tmp && tmp != task && tmp->cmd == task->cmd ) { /// FIXME HISTORY_001 : Add copmpare operator for Task 
          if( m_buffer.erase( i ) ) { delete tmp; }  
        }
      }     
    }
    
    // Check entries num limit
    if( m_limit > 0 ) {
      num = m_buffer.no( );
      while( num < m_limits ) { num--; __rem( num, true ); }
    }

    m_change = true;       
    return true;
  }
  
  return false;
}

FXbool History::__rem( FXint pos, FXbool destroy )
{
   Task *elem = ( destroy ? m_buffer[ pos ] : NULL );
   
   if( m_buffer.erase( pos ) ) {
     m_change = true;
     if( entry ) { delete entry; } 
     return true;  
   }
   
   return false;
}

void History::__top( FXint index )
{
  if( index == 0 ) { return true; }  
  if( index > 0 ) {  
    Task *tmp = m_buffer[ index ];
    if( m_buffer.erase( index ) && __add( tmp, false ) ) { 
      return true; 
    }
  }
  
  return false;
}

/*************************************************************************************************/
FXbool History::Index( FXint index )
{
  return ( index >= 0 && index < m_buffer.no( ) );   
}


/*** END *****************************************************************************************/
