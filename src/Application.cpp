/*************************************************************************
* Application.cpp Copyright (c) 2012 - 2022 by  D.A.Tiger                *
*                                                                        *
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
#include<Application.h>

FXDEFMAP( Application ) APPLICATION_MAP[ ] = { 
  FXMAPFUNC( SEL_COMMAND, Application::QUIT_NEGATION, Application::OnCmd_QuitNegation )
};

FXIMPLEMENT( Application, FXApp, APPLICATION_MAP, ARRAYNUMBER( APPLICATION_MAP ) )

/*************************************************************************************************/
Application::Application( )
           : FXApp( "Runner", "FOX-DESKTOP" )
{
  Welcome( );

  a_cfg     = new app_config;
  a_iconsth = new FXIconsTheme( this );
  a_history = new History;
  
  //load( );  
}

Application::~Application( )
{
  save( );

  std::cout << "=== End =============================================" << std::endl;
  std::cout.flush( );
}

/*************************************************************************************************/
void Application::init( int& argc, char** argv, FXbool connect )
{
  FXApp::init( argc, argv, connect );
  load( );
}

FXint Application::task_exec( Task *cmd )
{
  FXint resh        = -1;
  FXString _command = "";  // Kompletni prikaz ke spusteni
  FXString _cmd     = "";  // zadany prikaz  
  FXString _term    = "";  // prikaz terminalu
  FXString _paccess = "";  // prikaz pro privilegovany pristup

  // Prikaz, parametry, neblokujici spusteni
  if( !cmd->cmd.empty( ) ) { _cmd += cmd->cmd; }
  if( !cmd->prm.empty( ) ) { _cmd += cmd->prm; }
  if( cmd->ow == true ) { _cmd += "&"; }

  if( _cmd.empty( ) != true  ) {
    _command = CheckTerminal( cmd ) + CheckPrivilege( cmd ) + _cmd;
    DEBUG_OUT( "Running: " << _command )
    resh = system( _command.text( ) );
  }

  DEBUG_OUT( "Execute resulth " << resh )
  return resh;
}

void Application::task_write( Task *cmd, const FXString &pth )
{
  if( ( pth.empty( ) == true ) || ( cmd == NULL )  ) { return; }

  DEBUG_OUT( "writing " << pth )
  FXSettings desk_file;
  FXString   desk_head = "Desktop Entry";
  FXString   command, name = FXPath::name( cmd->cmd );

  /// FIXME APP_001 : sudo!
  //if( cmd->su == true ) { command += a_cfg->su + " "; }
  command += cmd->cmd;
  if( !cmd->prm.empty( ) ) { command += " " + cmd->prm; }

  desk_file.writeStringEntry( desk_head, "name",     name.text( ) );
  desk_file.writeStringEntry( desk_head, "TryExec",  cmd->cmd.text( ) );
  desk_file.writeStringEntry( desk_head, "Exec",     command.text( ) );
  desk_file.writeStringEntry( desk_head, "Path",     cmd->wpth.text( ) );
  desk_file.writeBoolEntry(   desk_head, "Terminal", cmd->te );

  desk_file.writeStringEntry( desk_head, "Comment", FXString::null );
  desk_file.writeStringEntry( desk_head, "Icon",    FXString::null );
  desk_file.writeStringEntry( desk_head, "Notyfy",  FXString::null );
  desk_file.writeStringEntry( desk_head, "Type",    "Application" );

  desk_file.unparseFile( pth + "/" + name + ".desktop" );
}

void Application::load( )
{
  settings_read( );
  a_iconsth->load( ICON_THEME_MAP, a_cfg->icons_name );
  a_hstore.changeUri( a_cfg->cache_dir + "/" +  getAppName( ) );
  a_hstore.open( "History", ";", FXString::null, TASKENTRIESNUM );
  a_history->load( a_hstore );
}

void Application::save( )
{
  a_history->save( a_hstore );
  settings_write( );
}


/**************************************************************************************************/
long Application::OnCmd_QuitNegation( FXObject *tgt, FXSelector sel, void *data )
{
  a_nquit_flg = !a_nquit_flg;
  return 1;
}

/**************************************************************************************************/
void Application::settings_read( )
{
  reg( ).read( );
  
  FXString cfg_prefix = CFG_UI_PREFIX;
  a_cfg->icons_name   = reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".IconsTheme", "Oxygen" );
  a_cfg->cache_dir    = reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".CacheDir",   ( FXSystem::getHomeDirectory( ) + "/.cache").text( ) );
  a_cfg->auto_exit    = reg( ).readBoolEntry(   CFG_RUNNER, cfg_prefix + ".AutoExit",   true );
  a_cfg->silent_exit  = reg( ).readBoolEntry(   CFG_RUNNER, cfg_prefix + ".SilentExit", false );

  cfg_prefix = CFG_SUDO_PREFIX;
  a_cfg->sudo    = reg( ).readBoolEntry( CFG_RUNNER, cfg_prefix + ".Enable", true );
  a_cfg->askpass = reg( ).readBoolEntry( CFG_RUNNER, cfg_prefix + ".Askpass", false );  
  
  cfg_prefix = CFG_TERM_PREFIX;
  a_cfg->term_enable  = reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".Enable", "On request" );
  a_cfg->term         = reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".Command", "/usr/bin/xterm" );
  a_cfg->term_noclose = reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".arg_disclose", "+hold" );
  a_cfg->term_run     = reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".arg_exec", "-e" );

  a_nquit_flg = false;

  a_cfg->change = false;
}

void Application::settings_write( )
{
  FXString cfg_prefix;

  if( a_cfg->change ) {
    cfg_prefix = CFG_UI_PREFIX;
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".IconsTheme", a_cfg->icons_name.text( ) );
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".CacheDir",   a_cfg->cache_dir.text( )  );
    reg( ).writeBoolEntry(   CFG_RUNNER, cfg_prefix + ".AutoExit",   a_cfg->auto_exit );
    reg( ).writeBoolEntry(   CFG_RUNNER, cfg_prefix + ".SilentExit", a_cfg->silent_exit );
  
    cfg_prefix = CFG_SUDO_PREFIX;
    reg( ).writeBoolEntry(   CFG_RUNNER, cfg_prefix + ".Enable",  a_cfg->sudo );
    reg( ).writeBoolEntry(   CFG_RUNNER, cfg_prefix + ".Askpass", a_cfg->askpass );

    cfg_prefix = CFG_TERM_PREFIX;
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".Enable",       a_cfg->term_enable.text( ) );
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".Command",      a_cfg->term.text( ) );
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".arg_exec",     a_cfg->term_run.text( ) );
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".arg_disclose", a_cfg->term_noclose.text( ) );
    reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".arg_workdir",  a_cfg->term_work.text( ) );
    a_cfg->change = false;
  } 

  if( reg( ).isModified( ) == true ) { 
    reg( ).write( ); 
  }
}

FXString Application::CheckPrivilege( Task *t )
{
  FXString resh = "";

  if( t->su && a_cfg->sudo  ) { 
    resh = "sudo"; 
    resh += ( a_cfg->askpass ? " -A " : " " );
  }
  
  return resh;
}

FXString Application::CheckTerminal( Task *t )
{
  FXString resh       = "";                            // Vysledny prikaz
  FXString supplement = "";                            // Doplnek prikazu
  FXbool   pa_term    = ( t->su && !a_cfg->askpass );  // Ma byt ET pouzit k autentizaci pro sudo?
  FXbool   use        = false;                         // Je ET vubec vyzadovan a povolen?

  if( a_cfg->term_enable != "Disable" ) {
    if( a_cfg->term_enable == "Always" )  { use = true; }
    else if( t->te || pa_term )           { use = true; }
  }
  
  if( use ) {
    // Zakldni prikaz - binarka
    resh = a_cfg->term + " ";

    // Nezavirat terminal
    if( t->lt == true ) { resh += a_cfg->term_noclose + " "; }

    // Nastavit pracovni cestu v terminalu
    if( !t->wpth.empty( ) ) {
      if( !a_cfg->term_work.empty( ) ) { resh += a_cfg->term_work + t->wpth + " "; }
      //else { supplement = "/bin/cd " + t->wpth + "; "; }
    }

    // Kompletni retezec prikazu emul. term.
    resh += a_cfg->term_run + " " + supplement;
  }

  return resh;
}

void Application::Welcome( )
{
  std::cout << "=== " << getAppName( ) << " ========================================" << std::endl;
  std::cout << "Copyright " << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << "  D.A.Tiger <drakarax@seznam.cz>, GNU GPL 3" << std::endl;
  std::cout << "Version    : "<< AutoVersion::MAJOR<< "."<< AutoVersion::MINOR << "." << AutoVersion::REVISION;
  std::cout << " [" << AutoVersion::STATUS << "]" << std::endl;
  std::cout << "lib Fox    : " << FOX_MAJOR << "." << FOX_MINOR << "." << FOX_LEVEL << std::endl;
  std::cout << "lib FoxGHI : " << 0 << "." << 4 << "." << 1 << std::endl;
  std::cout << "=== Message =========================================" << std::endl;
}

/*** END ******************************************************************************************/

