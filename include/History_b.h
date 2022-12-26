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

class History_b : public FXArray<FXString> {
  FXint  h_limit;  // Limit of num entries in history
  FXbool h_change; // Changed state indicator

public :
  History_b( FXint num = 0, FXuint opts = 0 );
  virtual ~History_b( );

  /* Operations methods */
  void   insert( const FXString &data );
  void   _clear( );
  FXbool save( Storage &storage );
  FXbool load( Storage &storage );

  /* Debug & tests */
  void dump( ); 
};

#endif /* __HISTORY_H */
/*** END ****************************************************************/
