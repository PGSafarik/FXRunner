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

/*** Konfiguracni struktura aplikace ***/
struct app_config {       // konfiguracni data tridy Application
  FXbool sudo;            // [Sudo.Enable]  - Povolit spusteni s pravy superuzivatele (s pomoci sudo )
  FXbool askpass;         // [Sudo.Askpass] - Povolit pouziti parametru askpass ( sudo -A ... )
  FXString term_enable;   // 
  FXString term;          // aplikace emulatoru terminalu
  FXString term_noclose;  // parametr terminalu, nezavirat terminal
  FXString term_run;      // parametr terminalu, spustit prikaz
  FXString term_work;     // parametr terminalu, nastavit pracovni adresar 
  FXString icon_path;     // cesta k ikonam aplikace
  FXString icons_name;    // Nazev ikonoveho tematu
  FXString cache_dir;     // Cache Adresas 
  FXbool   auto_exit;     // Automaticaly exit FXRunner after launch application
  FXbool   silent_exit;   // Require confirmation of program termination

  FXbool  change;         // Indicate changes
};

/*** Application core ***/
class Application : public FXApp {
FXDECLARE( Application )
  friend class Settings;

  app_config   *a_cfg;       // The application Configuration struct
  FXIconsTheme *a_iconsth;   // Icons theme manager
  History_b    *a_history;   // Manager of history of commands

  FXbool a_nquit_flg;        // Flag about temporary negation of autoexit settings

public :
  Application( );
  virtual ~Application( );

  /* Operations */
  int task_exec( Task *cmd );                                          // Run a command
  void task_write( Task *cmd, const FXString &pth = FXString::null );  // Write a command on a desktop file

  /* Access methods */
  FXIconsTheme* get_iconstheme( ) { return a_iconsth; }                // Get a icons theme instance
  History_b*    get_History( )    { return a_history; }                // Get cache object, represent the launch history 
  FXbool        autoexit( )       { return ( ( a_nquit_flg ) ? !a_cfg->auto_exit : a_cfg->auto_exit ); }
  FXbool        is_silent( )      { return a_cfg->silent_exit; }       // if true, the application will not ask for confirmation of termination
  FXbool        is_changed( )     { return a_cfg->change; }            // Idicate configration is changed

  /* messages & handlers */
  enum {
    QUIT_NEGATION = FXApp::ID_LAST,
    ID_LAST
  };
  long OnCmd_QuitNegation( FXObject *tgt, FXSelector sel, void *data );

protected :
  /* Helpful methods */
  void settings_load( );
  void settings_save( );

  FXString CheckPrivilege( Task *t );  // Check to using sudo
  FXString CheckTerminal( Task *t );   // Check to using terminal emulator 
};

#endif /* __APPLICATION_H */
/*** END ****************************************************************/
