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

#ifndef __MAIN_H
#define __MAIN_H
/*************************************************************************
* main.h                                                                 *
*                                                                        *
* Deklarace zakladnich datovych struktur a hlavickovych souboru          *
* Copyright (c) 18/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<iostream>
#include<fox-1.7/fx.h>
#include<fxghi.h>

#include<../version.h>
#include<Task.h>

typedef FXArray<FXString> FXString_List;

struct app_config {       /// konfiguracni data tridy Application
  FXString su;            /// prikaz ke spusteni s pravy superuzivatele
  FXString term;          /// aplikace emulatoru terminalu
  FXString term_noclose;  /// parametr terminalu, nezavirat terminal
  FXString term_run;      /// parametr terminal,  spustit prikaz
  FXString icon_path;     /// cesta k ikonam aplikace
};

#endif /* __MAIN_H */
/*** END ****************************************************************/
