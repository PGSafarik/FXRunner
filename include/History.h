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

class History_b : public FXArray<FXString> {
  FXString h_name;
  FXint    h_num;
  FXuint   h_hash;
  FXbool   h_change;

public :
  History_b( FXint num = 0, const FXString name = "UNAMED CACHE" );
  virtual ~History_b( );

  void dump( );

  void insert( const FXString &data );
  void _clear( );

  FXbool save( const FXString &file, FXbool mk = false );
  FXbool load( const FXString &file );
};

#endif /* __HISTORY_H */
/*** END ****************************************************************/