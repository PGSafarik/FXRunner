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

#ifndef __APPLICATION_H
#define __APPLICATION_H
/*************************************************************************
* Appliction.h                                                           *
*                                                                        *
* Systemovy zaklad aplikace                                              *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<History.h> 

class Application : public FXApp {
FXDECLARE( Application )
  friend class Settings;

  app_config   *a_cfg;       // Konfiguracni data aplikace
  FXIconsTheme *a_iconsth;   // Spravce ikonoveho schematu
  History_b    *a_history;   // Spravce historie spustenych prikazu

  FXbool a_nquit_flg;        // Priznak o docasne negaci nastaveni autoexitu

public :
  Application( );
  virtual ~Application( );

  /* Operations */
  int task_exec( Task *cmd );                                          // Spusti prikaz
  void task_write( Task *cmd, const FXString &pth = FXString::null );  // Zapise prikaz do desktop souboru

  /* Access methods */
  FXIconsTheme* get_iconstheme( ) { return a_iconsth; }                // Get a icons theme instance
  History_b*    get_History( )    { return a_history; }                // Get cache object, represent the launch history 
  FXbool        autoexit( )       { return ( ( a_nquit_flg ) ? !a_cfg->auto_exit : a_cfg->auto_exit ); }
  FXbool        is_silent( )      { return a_cfg->silent_exit; }
  FXbool        is_changed( )     { return a_cfg->change; }

  enum {
    QUIT_NEGATION = FXApp::ID_LAST,
    ID_LAST
  };
  long OnCmd_QuitNegation( FXObject *tgt, FXSelector sel, void *data );

protected :
  // Helpers
  void settings_load( );
  void settings_save( );

  FXString CheckPrivilege( Task *t );  // Check to using sudo
  FXString CheckTerminal( Task *t );   // Check to using terminal emulator 
};

#endif /* __APPLICATION_H */
/*** END ****************************************************************/
