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
#ifndef FXRUNNER_HISTORYBUFFER_H
#define FXRUNNER_HISTORYBUFFER_H

/*************************************************************************
* History.h                                                              *
*                                                                        *
* Zaznam a sprava historie prikazu                                       *
* Copyright (c) 08/08/2025 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "defs.h"
#include "Task.h"

class HistoryList : protected FXArray<Task*> {
  FXint  m_limit;				// Limit uloziste ( 0 - bez limitu. > 0 - udrzovany pocet radku. )
  FXint  m_hysteresis;	// Presah limitu  ( 0 - bez hysterze. < 0 - hodnota na niz klesne velikost uloziste pri dosazeni limitu. > 0 - hodnota o niz muze velikost prerust, nez bude zkracena na hodnotu limitu)
  FXbool m_change;      //

public :
  explicit HistoryList( FXint limit = 0, FXint hysteresis = 0 );
  ~HistoryList( ) = default;

  /* Access methods */
  using FXArray<Task*>::no;
  using FXArray<Task*>::at;
  FXint  get_limit( ) const         { return m_limit; }
  void   set_limit( FXint limit )   { m_limit = limit; }
  FXint  get_hysteresis( ) const    { return m_hysteresis; }
  void   set_hysteresis( FXint hs ) { m_hysteresis = hs; }
  FXbool is_changed( ) const        { return m_change; }

  /* Operations */
  Task*  insert( FXint pos, Task *entry, FXbool change = true );
  Task*  remove( FXint pos, FXbool destroy = false, FXbool change = true );
  void   clear( FXbool change = true );
  FXbool top( FXint pos );
  FXbool check_position( FXint value ) const;          // True if the value is between 0 and the current number of records

protected:
  void Deduplication( const Task *entry, FXint start = 0 ); // Finds and removes from the list all duplicates of the task specified by the function parameter
  void Truncate( );                                         //{} Monitors the buffer size based on the set limit and hysteresis.
  void CheckLimit( );                                       // Checking and maintaining the set number of records
};

#endif //FXRUNNER_HISTORYBUFFER_H
