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

#ifndef __HISTORYFILE_H
#define __HISTORYFILE_H
/*************************************************************************
* HistoryFile.h                                                          *
*                                                                        *
* Trida, reprezentujici soubor pro zaznam historie prikazu               *
* Copyright (c) 23/11/2015 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<History.h>

class HistoryFile : public FXFile {	
public:
  HistoryFile(const FXString &filename, FXuint m = FXIO::Reading, FXuint perm = FXIO::AllReadWrite );
  FXint readHistory( History *history );
  FXint writeHistory( History *history );
};

#endif /* __HISTORYFILE_H */
