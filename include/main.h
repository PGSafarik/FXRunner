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
#include<FXIconsTheme.h>

/* Declaration new types */
typedef FXArray<FXString> FXString_List;

/* Runner configure sections */
#define CFG_RUNNER      "Runner"
#define CFG_TERM_PREFIX "Term"
#define CFG_SUDO_PREFIX "Sudo"
#define CFG_UI_PREFIX   "Interface"

#define ICON_THEME_MAP  "/usr/share/FXRunner/data/icons.map"

/* Print debug log */
#ifdef DEBUG
#define DEBUG_OUT(text) std::cout << "[DEBUG " << __FILE__ << " " << __LINE__ << "]: " << text << std::endl; 
#else
#define DEBUG_OUT(text)
#endif



#endif /* __MAIN_H */
/*** END ****************************************************************/
