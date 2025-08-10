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
 FXint m_limit;
 FXbool m_change;

public :
  explicit HistoryList( FXint limit = 0 );
  ~HistoryList( ) = default;

  /* Access methods */
  using FXArray<Task*>::no;
  using FXArray<Task*>::at;
  FXint get_limit( ) const { return m_limit; }
  void set_limit( FXint limit ) { m_limit = limit; }
  FXbool is_changed( ) const { return m_change; }

  /* Operations */
  Task* insert( FXint pos, Task *entry, FXbool change = true );
  Task* remove( FXint pos, FXbool destroy = false, FXbool change = true );
  FXbool top( FXint pos );
  FXbool check_position( FXint value ) const;          // True if the value is between 0 and the current number of records

protected:
  void Deduplication( const Task *entry, FXint start = 0 ); // Finds and removes from the list all duplicates of the task specified by the function parameter
  void CheckLimit( );                                       // Checking and maintaining the set number of records

};


#endif //FXRUNNER_HISTORYBUFFER_H
