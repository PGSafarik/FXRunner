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

#ifndef __TASK_H
#define __TASK_H
/*************************************************************************
* Task.h                                                                 *
*                                                                        *
* The data structure of the task to run                                  *
* Copyright (c) 23/11/2015 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<loki/SmallObj.h>
 
struct Properties : public Loki::SmallObject<> {   // Auxiliary structure for storing the state of properties 
  FXbool suaccess    = false; // Run with super user access
  FXbool unblock     = true;  // Add the '&' character to the end of the command string (run a command in the background, in non-blocking mode - in case of GUI) 
  FXbool term        = false; // Run the command in terminal
  FXbool nocloseterm = false; // Try to prevent automatic termination of the terminal when the command is finished running.
};

struct Task : public Loki::SmallObject<> {    
  FXString cmd;   // Command
  FXString prm;   // Params
  FXString wpth;  // Work dir
  
  Properties *prop;
  
  Task( const FXString &cmd_str = FXString::null );
  virtual ~Task( );

  template<class STREAM> void load_data( STREAM &store )
  {
    store >> cmd;
    store >> prm;
    store >> wpth;    
    store >> prop->suaccess;
    store >> prop->unblock;
    store >> prop->term;   
    store >> prop->nocloseterm; 

    DEBUG_OUT( "Load task data from the data store: " << this->cmd.text( ) )
  }

  template<class STREAM> void save_data( STREAM &store )
  {
    store << cmd;
    store << prm;
    store << wpth;
    store << prop->suaccess;
    store << prop->unblock;
    store << prop->term;
    store << prop->nocloseterm; 

    DEBUG_OUT( "Saving task data from the data store: " << this->cmd.text( ) )
  }

};

#endif /* __TASK_H */
/*** END ****************************************************************/




