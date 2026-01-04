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
#ifndef FXRUNNER_RUNNER_H
#define FXRUNNER_RUNNER_H

/*************************************************************************
* Runner.h                                                               *
*                                                                        *
* Main window                                                            *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "core.h"
#include "widgets.h"

class RunBox;

class Runner : public FXPrimaryWindow {
FXDECLARE( Runner )
  Application  *r_app;
  FXIconsTheme *r_icons;
  FXTextField  *m_cmdfield;
  RunBox       *m_runbox;

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

    HISTORY_EVENT,
    HISTORY_SHOW,
  };

  long onCmd_Run( FXObject *tgt, FXSelector sel, void *data );
  long onCmd_Open( FXObject *tgt, FXSelector sel, void *data );
  long on_HistoryEvent( FXObject *tgt, FXSelector sel, void *data );
  long on_HistoryShow( FXObject *tgt, FXSelector sel, void *data );


protected:
  Runner( ) { }
  
  /* Help methods */
  History*   GetHistory( ) { return r_app->get_History( ); }
  void       DefaultFocus( );
  void       ShortCuts( );
  FXbool     PrepareTask( );        // Vytvori novou ulohu z formulare
  Task*      SetTask( Task *task ); // Nastavi
  void       Update( );             // Provede nacteni hodnot z historie do UI
};

#endif /* FXRUNNER_RUNNER_H */
/*** END ****************************************************************/
