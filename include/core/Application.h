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
#include "defs.h"
#include "Task.h"
#include "History.h"
#include "Storage.h"
#include "FXIconsTheme.h"
#include "version.h"


/*** Terminal emulator Profile struct ***/
struct app_teprofile {
  FXString name;      // Nazev emulatoru terminalu
  FXString cmd;       // Prikaz spoustejici terminal
  FXString p_run;     // (param), retezec ulohy ke spusteni 
  FXString p_noclose; // (param), nezavirat terminal po skonceni ulohy
  FXString p_Workdir; // (param), nastavit pracovni adresar  
};  

/*** Application configuration structure ***/
struct app_config {
  FXbool sudo;            // [Sudo.Enable]  - Enable running as root (using sudo; def: false )
  FXbool askpass;         // [Sudo.Askpass] - Enable use of the askpass parameter for sudo ( sudo -A ..., def: false )

  FXString term_enable;             // Enable run command in terminal (def: true)
  FXString term;                    // aplikace emulatoru terminalu
  FXString term_noclose;            // parametr terminalu, nezavirat terminal
  FXString term_run;                // parametr terminalu, spustit prikaz
  FXString term_work;               // parametr terminalu, nastavit pracovni adresar
  FXStringDictionary term_profiles; // List of terminal emulators

  FXString icon_path;     // Path for application icons
  FXString icons_name;    // Name of the used icons theme
  FXString cache_dir;     // Path of the cache directory (def: $HOME/.cache )
  FXbool   auto_exit;     // Automatically exit FXRunner after launch application
  FXbool   silent_exit;   // Require confirmation of program termination

  FXint    hist_limit;    // Limit value for the number of records in the history buffer (def: 0 )
  FXint    hist_lhyster;  // History limit hysteresis value (def: 0)
  FXbool   hist_loadopt;  // Enable clean and optimize history buffer in loading data from store (def: false)
  FXint    hist_headsize; // Number of an items from history, visible with the Runbox

  FXbool  change;         // Indicate changes
};

/*** Application core ***/
class Application : public FXApp {
FXDECLARE( Application )
  friend class Settings;

  app_config                     *a_cfg;           // The application Configuration struct
  FXIconsTheme                   *a_iconsth;     	 // Icons theme manager
  History                        *a_history;     	 // Manager of history of commands
  Storage<SubstrStream, History>  m_history_store; //
  FXbool                          a_nquit_flg;     // Flag about temporary negation of autoexit settings

public :
  Application( );
  ~Application( ) override;

  /* Access methods */
  FXIconsTheme* get_iconstheme( )   { return a_iconsth; }                // Get a icons theme instance
  History*      get_History( )      { return a_history; }                // Get cache object, represent the launch history
  FXint         HistoryQuickSize( ) { return a_cfg->hist_headsize; }     //
  FXbool        autoexit( )         { return ( ( a_nquit_flg ) ? !a_cfg->auto_exit : a_cfg->auto_exit ); }
  FXbool        is_silent( )        { return a_cfg->silent_exit; }       // if true, the application will not ask for confirmation of termination
  FXbool        is_changed( )       { return a_cfg->change; }            // Indicate configration is changed


  /* Operations */
	virtual void init( int& argc, char** argv, FXbool connect = true ); 
  int task_exec( Task *cmd );                                            // Run a command
  int task_exec( );                                                      //
  void task_write( Task *cmd, const FXString &pth = FXString::null ); // Write a command on a desktop file
  void load( );                                                          // Complete load
  void save( );                                                          // Complete application save

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

  void Welcome( );
};

#endif /* __APPLICATION_H */
/*** END ****************************************************************/
