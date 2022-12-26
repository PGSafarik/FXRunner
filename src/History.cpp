#include<History.h>

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

FXbool History::insert( Task *task )
{
  /// FIXME HISTORY_002 : Remove m_add_empty flag
  if( task == NULL || ( task->cmd.empty( ) && !m_add_empty ) ) { return false; } 
  return __add( task ); 
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
  
FXint History::load( Storage &store )
{
  if( store.isOpen( ) ) {
    clear( );  
    
    while( store.eof( ) > 0 ) { 
      Task *entry = new Task;
      if( entry ) { 
        entry->load( store ); 
        m_buffer.push( entry );
      }
    }
    
    m_change    = false;
  }
  else { std::cerr << "[ERROR] Unable to open a history storage " << ( store.getUri( ) + "/" + store.getName( ) ) << std::endl; }

 return true;
}

FXint History::save( Storage &store )
{
  if( !m_change ) { return true; }
  if( store.isOpen( ) ) {
    FXint num = m_buffer.no( );

    for( FXint i = 0; i != num; i++ ) {
      Task *entry = m_buffer[ i ];
      if( entry ) { entry->save( store ); }
    } 
    
    store.flush( );
    m_change = false;
  }
  else { std::cerr << "[ERROR] Unable to save a history storage " << ( store.getUri( ) + "/" + store.getName( ) ) << std::endl; }    

  return true;   
    
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
        std::cout << task->su   << "; ";
        std::cout << task->te;
      }
      else { std::cout << "- This entry is EMPTY! -"; }

      std::cout << std::endl;     
    }
  }
  else {  std::cout << "  - History is empty -  " << std::endl; }
  std::cout << "--- End History object dump ---" << std::endl;

}

/**************************************************************************************************/
FXbool History::__add( Task *entry, FXbool dedupl )
{
  FXint num = 0;
  if( m_buffer.insert( 0, entry ) ) {
    if( dedupl && !entry->cmd.empty( ) ) {  
      
      // Check entries num limit
      if( m_limit > 0 ) {
        num = m_buffer.no( );
        while( num < m_limit ) { num--; __rem( num, true ); }
      }

      // Deduplication
      num = m_buffer.no( );
      for( FXint i = 1; i != num; i++ ) {
        Task *tmp = m_buffer[ i ];
        if( tmp && tmp != entry && tmp->cmd == entry->cmd ) { /// FIXME HISTORY_001 : Add copmpare operator for Task 
          if( m_buffer.erase( i ) ) { delete tmp; }  
        }
      }     
    }
    
    m_change = true;       
    return true;
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
    if( m_buffer.erase( index ) && __add( tmp, false ) ) { 
      return true; 
    }
  }
  
  return false;
}

/*************************************************************************************************/
FXbool History::Index( FXint value )
{
  return ( value >= 0 && value < m_buffer.no( ) );   
}


/*** END *****************************************************************************************/
