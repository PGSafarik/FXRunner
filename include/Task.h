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
* Uloha ke spusteni                                                      *
* Copyright (c) 23/11/2015 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<Storage.h>

class Task : public FXObject {  /// data prikazu ke spusteni
FXDECLARE( Task )

public :
  FXString cmd;   /// prikaz
  FXString prm;   /// parametry
  FXString wpth;  /// pracovni cesta
  FXbool   su;    /// su(do) - spusti s pravy superuzivatele
  FXbool   ow;    /// & - na pozadi
  FXbool   te;    /// terminal - v terminalu
  FXbool   lt;    /// lock term - po skonceni nezvirat terminal
  FXbool   cl;    /// create link

  Task( );
  virtual ~Task( );
  virtual void load( FXStream &store );  // Binary straem
  virtual void save( FXStream &store );  // Binary stream

  friend Storage& operator <<( Storage &s, Task &task );
  friend Storage& operator >>( Storage &s, Task &task );

protected:
  FXString ConvOnText( const FXString &str );   
};


#endif /* __TASK_H */
/*** END ****************************************************************/




