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
* Record and management command history                                  *
* Copyright (c) 18/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "defs.h"
#include "StringUtils.h"
#include "Task.h"
#include "HistoryList.h"

class History : public FXObject {
FXDECLARE( History )
  HistoryList  m_buffer;  // Tasks list

  FXObject   *m_tgt;      // A target object for notifications
  FXSelector  m_sel;      // A notification message ID
  FXuint      m_opts;     // List of the object options

  /* FIXME HITORY_001: Adding the settings flags for: Only read history, adding emty task, */
  /* FIXME HISTOR_002: Adding messages & handlers ;) */
public :
  History( FXuint opts = 0, FXObject* target = nullptr, FXSelector notify = 0 );
  ~History( ) override = default;
  
  /* Access methods */
  FXival no( )        const         { return m_buffer.no( ); }
  FXint  get_limit( ) const         { return m_buffer.limit; }
  void   set_limit( FXint limit )   { m_buffer.limit = limit; }
  FXint  get_hysteresis( ) const    { return m_buffer.hysteresis; }
  void   set_hysteresis( FXint hs ) { m_buffer.hysteresis = hs; }
  FXbool isChange( )   const        { return m_buffer.is_changed( ); }

  FXObject*  get_target( )                  { return m_tgt; }
  void       set_target( FXObject* target ) { m_tgt = target; }
  FXSelector get_notify( )                  { return m_sel; }
  void       set_notify( FXSelector sel )   { m_sel = sel; }

  /* operations methods */
  Task*  at( const FXint index = 0 ) { return m_buffer.at( index ); }   // Returns task from specific position (if any)
  FXbool current( FXint pos, FXbool notify = true );                    // Returns true if the task is set to current (i.e., to the top of the history stack).
  Task*  add( const FXString &cmd_str, FXbool notify = false );         // Create new Task instance and insert on list
  Task*  remove( FXint index, FXbool notify = false );                  // Remove existing index
  void   clear( FXbool notify = false );                                // Clear all items

  template <typename STREAMER> void load_data( STREAMER &pipe ) {
    if( pipe.get_state( ) == 0 ) {
      if( pipe.get_index( ) == 0 ) { clear( ); }

      Task *task = new Task;
      task->load_data( pipe );
      m_buffer.insert( static_cast<FXint>( m_buffer.no( ) ), task, false );
    } // FIXME HISTOR_003: else -> handle state
  }
  template <typename STREAMER> void save_data( STREAMER &pipe ) {
    if( pipe.get_state( ) == 0 ) {
      FXint index = pipe.get_index( );
      if( index < m_buffer.no( ) ) {
        Task *task = m_buffer.at( index );
        if( task ) { task->save_data( pipe ); }
      }
      else { pipe.set_state( 1 ); }
    } // FIXME HISTOR_003: else -> handle stating
  }

  /* Debug & tests */
  void Dump( );                                           // Dumping actual history state on std::cout
  
protected :
  long Notify( FXuint type = 0, void *data = nullptr );
};

#endif /* FXRUNNER_HISTORY_H */
