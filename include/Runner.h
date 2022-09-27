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
* Deklarace tridy hlavniho okna aplikace                                 *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<Application.h>
#include<History.h>
#include<FXBoxFrame.h>
#include<Boxes.h>


//class Runner : public FXMainWindow {
class Runner : public FXGWindow {
FXDECLARE( Runner )
  Application *app;
  FXComboBox  *r_combo;
  FXTextField *r_pfield;
  FXTextField *r_tfield;

  Task         *r_acmd;
  FXIconsTheme *r_icons;

  //FXStringList history;
  History_b r_history;
  FXArray<Task*> p_buffer;

  /* Configuration data */
  // Paths
  FXString r_CacheDir;   // Ukladani historie prikazu
  FXString r_ShareDir;   // Uzivatelska data programu
  FXString r_WorkDir;    // Defaultni pracovni cesta pro spustene aplikace ( "~/" )

  // Application
  FXbool    r_NoQuit;       // false - Po spusteni prikazu bude runner ukoncen
  FXbool    r_SilentQuit;   // false - Bude vyzadovat potvrzeni ukonceni programu

  // Utils
  FXString r_SuDialog;  // Utilita pro udelovani superuzivatelskych prav
  FXString r_TermEmu;   // Emulator terminalu (XTerm, Konsole, ... )

public:
  Runner( Application *a );
  virtual ~Runner( );

  ////////////////////////////////////////////////
  ///
  ///
  virtual void create( );
  FXint readHistory( const History_b &buff );
  void load( );
  void save( );

  ////////////////////////////////////////////////
  ///
  ///
  enum {
    ID_ACCEPT = FXGWindow::ID_LAST,
    ID_CANCEL,
    ID_OPTIONS,
    ID_HELP,

    ID_OPEN_DIR,
    ID_OPEN_FILE,

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
};

#endif /* __RUNNER_H */
/*** END ****************************************************************/
