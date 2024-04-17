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
#ifndef __RUNNER_H
#define __RUNNER_H
/*************************************************************************
* Runner.h                                                               *
*                                                                        *
* Main window                                                            *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<Application.h>
#include<History.h>
#include<Boxes.h>
#include<Settings.h>
#include<Task.h>

//class Runner : public FXMainWindow {
class Runner : public FXPrimaryWindow {
FXDECLARE( Runner )
  Application *r_app;
  FXComboBox  *r_combo;
  FXTextField *r_pfield;
  FXTextField *r_tfield;

  Properties    r_prop;
  FXIconsTheme *r_icons;

public:
  Runner( Application *a );
  virtual ~Runner( );

  /* Operations */
  virtual void create( );

  /* Gui messages & handlers */
  enum {
    ID_ACCEPT = FXPrimaryWindow::ID_LAST,
    ID_CANCEL,

    OPEN_DIR,
    OPEN_FILE,
    OPEN_OPTIONS,
    OPEN_HELP,

    ID_NOQUIT,
    ID_USER,
    ID_ANNOUNCE,
    ID_LINK,
    ID_TERMINAL,
    ID_TERMLOCK,

    HYSTORY_CLEAR,
  };

  long onCmd_Run( FXObject *tgt, FXSelector sel, void *data );
  long onCmd_Open( FXObject *tgt, FXSelector sel, void *data );
  long onCmd_Tools( FXObject *tgt, FXSelector sel, void *data );

protected:
  Runner( ) { }
  
  /* Help methods */
  History*   GetHistory( ) { return r_app->get_History( ); } 
  void       LoadHistory( );
  void       Check_property( Task *task );
};

#endif /* __RUNNER_H */
/*** END ****************************************************************/
