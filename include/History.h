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

#ifndef __HISTORY_H
#define __HISTORY_H
/*************************************************************************
* History.h                                                              *
*                                                                        *
* Zaznam a sprava historie prikazu                                       *
* Copyright (c) 18/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<Storage.h>
#include<Task.h>

class History : public FXObject {
  FXArray<Task*> m_buffer;      // Tasks list
  FXint          m_limit;       // Max number limit of entries in the list   
  FXbool         m_change;      // flag for changes in tasks list
  FXbool         m_add_empty;   // flaeg enabled add empty task
  
  Task*  __create( const FXString &cmd );            
  FXbool __add( Task *entry, FXbool dedupl = true ); // Add Task instance on tasks list
  FXbool __rem( FXint pos, FXbool destroy = false ); // Remove tesk from list
  FXbool __top( FXint index );                       // Move task from index on position 0
  
public :
  History( FXint limit = 0; FXuint opts );
  virtual ~History( );
  
  /* Access methods */
  FXint no( )        { return m_buffer.no( ); } 
  FXbool isChange( ) { return m_change; }
  
  /* operations methods */
  Task*  at( FXint index );
  FXbool add( const FXString &cmd_str ); // Create new Task instance and insert on list
  FXbool insert( Task *task );           // Insert existing task on list
  Task*  remove( FXint index );          // Remove existing index
  void   clear( );                       // Clear all items 
  
  FXint load( Storage &store );
  FXint save( Storage &store );
  
  /* Debug & tests */
  void Dump( );
  
protected :
  FXbool ValidPosition( FXint index );  
};
