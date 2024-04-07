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

  Task( const FXString &cmd_str = FXString::null );
  virtual ~Task( );

  template<class STREAM> void load_data( STREAM &store )
	{
    store.operator >> ( this->cmd );
    store >> prm;
    store >> wpth;
    store >> su;
    store >> ow;
    store >> te;
    store >> lt;
		
    DEBUG_OUT( "Load command data from the data store: " << this->cmd.text( ) )
	}

  template<class STREAM> void save_data( STREAM &store )
	{
    store << cmd;
    store << prm;
    store << wpth;
    store << su;
    store << ow;
    store << te;
    store << lt;		
		
		DEBUG_OUT( "Saving command data from the data store: " << this->cmd.text( ) )
	}

};


#endif /* __TASK_H */
/*** END ****************************************************************/




