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
#ifndef FXRUNNER_TASK_H
#define FXRUNNER_TASK_H

/*************************************************************************
* Task.h                                                                 *
*                                                                        *
* The data structure of the task to run                                  *
* Copyright (c) 23/11/2015 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "defs.h"

enum TASK_MODE {
  PRIVILAGE = 0,  // Privilege access mode flag
  BLOCKED,        // Blocked mode
  TERMINAL,       // Run in the terminal emulator
  UNCLOSED,       // Try to prevent automatic termination of the terminal when the command is finished running.
};

class Task : public FXObject {
FXDECLARE( Task )
  FXString m_cmd;   // Command
  FXString m_wpth;  // Work dir
  FXString prm;     // Params (deprecated?)
  FXuint m_modes;   // Properties bitmap

  FXRunnable *m_notify;

public:
  explicit Task( const FXString &cmd_str, FXRunnable *notifier );
  ~Task( ) override;

  FXbool operator == ( const Task &other ) const { return m_cmd == other.m_cmd && prm == other.prm; }
  FXbool operator == ( const FXString &cmd ) const { return cmd == this->m_cmd; };

  FXbool operator != ( const Task &other ) const { return !( *this == other ); }
  FXbool operator != ( const FXString &cmd ) const { return !( *this == cmd ); }

  /* access methods */
  FXbool is_empty( ) const { return m_cmd.empty( ); }
  FXString get_cmd( ) const { return m_cmd; }
  void set_cmd( const FXString &cmd ) { this->m_cmd = cmd; }
  FXString get_wdir( ) const { return ( !m_wpth.empty( ) ? m_wpth : FXSystem::getHomeDirectory( ) ); }
  void set_wdir( const FXString &wd ) { if ( m_wpth != wd ) { m_wpth = wd; m_notify->run( ); } }

  void   set_property( FXuint prop )   { if( !check_property( prop ) ) { m_modes |= ( 1 << prop ); m_notify->run( ); } }
  void   unset_property( FXuint prop ) { if( check_property( prop ) ) {  m_modes &= ~( 1 << prop ); m_notify->run( ); } }
  FXbool check_property( FXuint prop ) const { return ( m_modes & ( 1 << prop ) ) != 0; }
  void   reset_properties( ) { m_modes = 0; }
  void   switch_property( FXuint prop, FXbool state = true ) { state ? set_property( prop ) : unset_property( prop ); }

  /* Operations */
  void dump( );

  template<class STREAM> void load_data( STREAM &store )
  {
    store >> m_cmd;
    store >> prm;
    store >> m_wpth;
    store >> m_modes;

    DEBUG_OUT( "Load task data from the data store: " << this->m_cmd.text( ) )
  }

  template<class STREAM> void save_data( STREAM &store )
  {
    store << m_cmd;
    store << prm;
    store << m_wpth;
    store << m_modes;
    DEBUG_OUT( "Saving task data from the data store: " << this->m_cmd.text( ) )
  }
protected:
  Task( ) = default;
};

#endif /* FXRUNNER_TASK_H */
/*** END ****************************************************************/




