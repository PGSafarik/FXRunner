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

struct Properties : public Loki::SmallObject<> {   // Auxiliary structure for storing the state of properties 
  FXbool suaccess    = false; // Run with super user access
  FXbool unblock     = true;  // Add the '&' character to the end of the command string (run a command in the background, in non-blocking mode - in case of GUI) 
  FXbool term        = false; // Run the command in terminal
  FXbool nocloseterm = false; // Try to prevent automatic termination of the terminal when the command is finished running.
};

enum PROPERTIES {
  PRIVILAGE = 0,  // Privilage access flag
  UNBLOCK,        // Add the '&' character to the end of the command string (run a command in the background, in non-blocking mode - in case of GUI)
  TERMINAL,       // Run in termonal emulator
  UNCLOSED,       // Try to prevent automatic termination of the terminal when the command is finished running.
};

class Task : public FXObject /*Loki::SmallObject<>*/ {
FXDECLARE( Task )
  FXString m_cmd;   // Command
  FXString m_wpth;  // Work dir
  FXString prm;     // Params - depracated
  FXuint m_properties;   // Properties bitmap

  FXObject   *m_tgt;  //
  FXSelector  m_msg;  //
public:
  Properties *prop;
  
  Task( const FXString &cmd_str = FXString::null );
  virtual ~Task( );

  FXbool operator == ( const Task &other ) const { return m_cmd == other.m_cmd && prm == other.prm; }
  FXbool operator == ( const FXString &cmd ) const { return cmd == this->m_cmd; };

  FXbool operator != ( const Task &other ) const { return !( *this == other ); }
  FXbool operator != ( const FXString &cmd ) const { return !( *this == cmd ); }

/* access methods */
  FXbool is_empty( ) const { return m_cmd.empty( ); }
  FXString get_cmd( ) const { return m_cmd; }
  void set_cmd( const FXString &cmd ) { this->m_cmd = cmd; }
  FXString get_wdir( ) const { return ( !m_wpth.empty( ) ? m_wpth : FXSystem::getHomeDirectory( ) ); }
  void set_wdir( const FXString &wd ) { if ( m_wpth != wd ) { m_wpth = wd; } }

  void   set_property( FXuint prop )   { m_properties |= ( 1 << prop ); }
  void   unset_property( FXuint prop ) { m_properties &= ~( 1 << prop ); }
  FXbool check_property( FXuint prop ) const { return ( m_properties & ( 1 << prop ) ) != 0; }
  void   reset_properties( ) { m_properties = 0; set_property( UNBLOCK ); }

  /* Operations */
  void dump( );

  template<class STREAM> void load_data( STREAM &store )
  {
    store >> m_cmd;
    store >> prm;
    store >> m_wpth;
    store >> prop->suaccess;
    store >> prop->unblock;
    store >> prop->term;   
    store >> prop->nocloseterm;
    store >> m_properties;

    DEBUG_OUT( "Load task data from the data store: " << this->m_cmd.text( ) )
  }

  template<class STREAM> void save_data( STREAM &store )
  {
    store << m_cmd;
    store << prm;
    store << m_wpth;
    store << prop->suaccess;
    store << prop->unblock;
    store << prop->term;
    store << prop->nocloseterm; 
    store << m_properties;
    DEBUG_OUT( "Saving task data from the data store: " << this->m_cmd.text( ) )
  }


protected:

};



#endif /* FXRUNNER_TASK_H */
/*** END ****************************************************************/




