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
* HistoryTools.h                                                          *
*                                                                        *
* Basic structure and operation of the history list                      *
* Copyright (c) 17/08/2025 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "defs.h"
#include "Task.h"

class HistoryBuffer : protected FXArray<Task*> {
  FXbool m_change;      // List changes indicator

public :
  HistoryBuffer( );
  ~HistoryBuffer( ) = default;

  /* Access objects & methods */
  FXint  limit = 0;				     // Limit number of items in the list ( 0 - no limit; > 0 - the value at which the number of items is maintained )
  FXint  hysteresis = 0;	     // Limit hysteresis ( 0 - limit = strict maximum; < 0 - The value by which the list will be truncated as soon as the limit is reached; > 0 - Exceeding the limit )
  using FXArray<Task*>::no;
  using FXArray<Task*>::at;
  FXbool is_changed( ) const  { return m_change; }

  /* Operations */
  Task*  insert( FXint pos, Task *entry, FXbool change = true );            // Insert item to specific position in list
  Task*  remove( FXint pos, FXbool destroy = false, FXbool change = true ); // Remove item from specific index
  void   clear( FXbool change = true );                                     // Delete all items in list
  FXbool top( FXint pos );                                                  // True if the item at the specified index was successfully moved to the beginning of the list
  FXbool check_position( FXint value ) const;                               // True if the value is between 0 and the current number of records
  void   cleaning( );                                                       // Cleaning and optimization of this list

protected:
  void Deduplication( const Task *entry, FXint start = 0 ); // Finds and removes from the list all duplicates of the task specified by the function parameter
  void Truncate( );                                         // Monitors the buffer size based on the set limit and hysteresis.
  FXint CheckLimit( );                                      // Checking and maintaining the set number of records
};

#endif //FXRUNNER_HISTORYBUFFER_H
