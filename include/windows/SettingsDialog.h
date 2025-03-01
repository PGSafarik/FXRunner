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
#ifndef FXRUNNER_SETDIALOG_H
#define FXRUNNER_SETDIALOG_H
/*************************************************************************
* Runner.h                                                               *
*                                                                        *
* Deklarace tridy hlavniho okna aplikace                                 *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include <core.h>
#include <widgets.h>

class SettingsDialog : public FXSecondaryWindow {
FXDECLARE( SettingsDialog )

public :
  SettingsDialog( FXWindow *own );
  ~SettingsDialog( );

  virtual void create( );
  long onCmdCancel( FXObject *sender, FXSelector sel, void *data );

protected:
  SettingsDialog( ) { }
};

#endif /* FXRUNNER_SETDIALOG_H */
/*** END ******************************************************************************************/