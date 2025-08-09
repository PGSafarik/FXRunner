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
#ifndef FXRUNNER_HISTORY_H
#define FXRUNNER_HISTORY_H

/*************************************************************************
* History.h                                                              *
*                                                                        *
* Zaznam a sprava historie prikazu                                       *
* Copyright (c) 18/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "defs.h"
#include "StringUtils.h"
#include "Task.h"
#include "HistoryList.h"

class History : public FXObject {
FXDECLARE( History )
  HistoryList  m_buffer;      // Tasks list

  FXObject   *m_tgt;   // A target object for notifications
  FXSelector  m_sel;   // A notification message ID
  FXuint      m_opts;  // List of the object options

  /* FIXME HITORY_001: Adding the settings flags for: Only read history, adding emty task, */
  /* FIXME HISTOR_002: Adding messages & handlers ;) */
public :
  History( FXint limit = 0, FXuint opts = 0, FXObject* target = nullptr, FXSelector notify = 0 );
  virtual ~History( );
  
  /* Access methods */
  FXint no( )        { return m_buffer.no( ); } 
  FXbool isChange( ) { return m_buffer.is_changed( ); }

  FXObject* get_target( )             { return m_tgt; }
  void set_target( FXObject* target ) { m_tgt = target; }
  FXSelector get_notify( )            { return m_sel; }
  void set_notify( FXSelector sel )   { m_sel = sel; }
  
  /* operations methods */
  Task*  at( FXint index, FXbool noup = false );
  Task*  add( const FXString &cmd_str );                  // Create new Task instance and insert on list
	FXbool push( Task *task, FXbool ch_state = true );      // Insert a task on end of history list
  FXbool insert( Task *task, FXint pos = 0 );             // Insert existing task on list
  Task*  remove( FXint index );                           // Remove existing index
  void   clear( );                                        // Clear all items 

  template <typename STREAMER> FXbool load_data( STREAMER &streamer ) {
    if( streamer.get_index( ) == 0 ) { clear( ); }

    Task *task = new Task;
    task->load_data( streamer );
    //push( task, false );
    m_buffer.insert( static_cast<FXint>( m_buffer.no( ) ), task, false );
    return true;
  }

  template <typename STREAMER> FXbool save_data( STREAMER &streamer ) {
    FXbool success = false;

     FXint index = streamer.get_index( );
     if( index < m_buffer.no( ) ) {
       Task *task = m_buffer.at( index );
       if( task ) { task->save_data( streamer );
         success = true;
       }
     }
     else { streamer.set_state( 1 ); }

     return success;
  }
  /* Debug & tests */
  void Dump( );                                           // Dumping actual history state on std::cout
  
protected :
  long notify( FXuint type = 0, void *data = nullptr );
};

#endif /* FXRUNNER_HISTORY_H */
