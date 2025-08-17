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
History::History( FXuint opts, FXObject* target, FXSelector notify )
{
  m_buffer.set_limit( limit );
  m_tgt = target;
  m_sel = notify;
  m_opts = opts;
  //! m_select = -1;
}

History::~History( )
{ }

/**************************************************************************************************/
FXbool History::current( FXint pos, FXbool notify )
{
  FXbool result = false;

  if( m_buffer.check_position( pos ) ) {
    result = m_buffer.top( pos );
    if( result && notify ) { Notify( SEL_UPDATE ); }
  }

  return result;
}

/**************************************************************************************************/
Task* History::at( FXint index, FXbool noup )
{ 
  Task *entry = nullptr;
  if( noup && m_buffer.top( index ) ) { entry = m_buffer.at( 0 ); } else { entry = m_buffer.at( index ); }

  return entry;
}

Task* History::add( const FXString &cmd_str, FXbool notify )
{
  if( !cmd_str.empty( ) ) {
    Task *task = new Task( cmd_str );
    if( task && m_buffer.insert( 0, task, notify ) ) {
      return task;
    }
  }

  return nullptr;
}

FXbool History::push( Task *task, FXbool ch_state ) // DEPRECATED: Used History::add( ), or History::insert( )
{
  FXbool result = false;
  if( task && !task->cmd.empty( ) ) {
    Task *t = m_buffer.insert( 0, task, ch_state );
    if( t ) { result = true; }
  }
  return result;
}

FXbool History::insert( FXint pos, Task *task, FXbool notify )
{
  FXbool result = false;

  if( task && !task->cmd.empty( ) ) {
    if ( m_buffer.insert( pos, task ) ) {
      if ( notify ) { Notify( SEL_INSERTED ); }
      return true;
    }
  }
  return result;
}

Task* History::remove( FXint index, FXbool notify )
{
    Task *t = m_buffer.at( index );
    if( t && m_buffer.remove( index ) ) {
      if ( notify) { Notify( SEL_DELETED ); }
      return t;
    }
    return nullptr;
}

void History::clear( FXbool notify )
{
  FXint num = m_buffer.no( );
  
  while( num > 0 ) {
    num--;
    m_buffer.remove( num, true );
  }

  if( notify ) { Notify( SEL_CHANGED ); }
}


/**************************************************************************************************/
void History::Dump( )
{
  FXint num = m_buffer.no( );
  std::cout << "[DUMP History] count: " << num << "; limit: " << m_buffer.get_limit( ) << std::endl;

  if( num > 0 ) {
    for( FXint i = 0; i != num; i++ ) {
      std::cout << i << ". ";

      Task *task = m_buffer.at( i );
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
long History::Notify( FXuint type, void *data )
{
  long result = -1;
  if( m_tgt ) {
    result = m_tgt->handle( this, type == 0 ? m_sel : FXSEL( type, m_sel ), data );
  }

  return result;
}

/*** END ******************************************************************************************/
