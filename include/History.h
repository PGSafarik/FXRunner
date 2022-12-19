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

/*
enum HistoryFormat = {
  HF_BINARY = 0x00000010,
  HF_TEXT   = 0x00000020
};
*/
class History_b : public FXArray<FXString> {
  FXString h_name;
  FXint    h_num;
  FXuint   h_hash;
  FXbool   h_change;

  SimpleFileStorage *h_storage;

public :
  History_b( FXint num = 0, FXuint opts = 0, const FXString &cache_dir = FXString::null, const FXString &name = "History" );
  virtual ~History_b( );

  void dump( );

  void insert( const FXString &data );
  void _clear( );

  FXbool write( );
  FXbool read( );

};

#endif /* __HISTORY_H */
/*** END ****************************************************************/
