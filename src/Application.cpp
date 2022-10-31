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

FXDEFMAP( Application ) APPLICATION_MAP[ ] = { };
FXIMPLEMENT( Application, FXApp, APPLICATION_MAP, ARRAYNUMBER( APPLICATION_MAP ) )

/*************************************************************************************************/
Application::Application( )
           : FXApp( "Runner", "TigerDesktopTools" )
{
  std::cout << "=== FXRunner ========================================" << std::endl;
  std::cout << "Copyright " << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << "  D.A.Tiger <drakarax@seznam.cz>, GNU GPL 3" << std::endl;
  std::cout << "Version    : "<< AutoVersion::MAJOR<< "."<< AutoVersion::MINOR << "." << AutoVersion::REVISION;
  std::cout << " [" << AutoVersion::STATUS << "]" << std::endl;
  std::cout << "lib Fox    : " << FOX_MAJOR << "." << FOX_MINOR << "." << FOX_LEVEL << std::endl;
  std::cout << "lib FoxGHI : " << 0 << "." << 2 << "." << 1 << std::endl;
  std::cout << "=== Message =========================================" << std::endl;

  a_cfg = new app_config;
  a_iconsth = new FXIconsTheme( this );
 
  settings_load( );
  std::cout.flush( );
}

Application::~Application( )
{
  settings_save( );
  std::cout << "=== End =============================================" << std::endl;
}

/*************************************************************************************************/
FXint Application::task_exec( Task *cmd )
{
  FXint resh = -1;
  FXString _command = "";
  FXString _cmd = "";
  FXString _term = "";

  std::cout << "Running: " << cmd->cmd.text( ) << std::endl;
/*
  std::cout << cmd->cmd.text( ) << std::endl;
  std::cout << cmd->wpth.text( ) << std::endl;
  std::cout << cmd->su << std::endl;
  std::cout << cmd->ow << std::endl;
  std::cout << cmd->te << std::endl;
*/
  // Spustit s pravy uzivatele root?
  if( cmd->su == true ) { _command += a_cfg->su + " "; }
  // Spustit v terminalu
  if( cmd->te == true ) {
    _term = a_cfg->term + " ";
    // Nastavit pracovni cestu v terminalu
    if( !cmd->wpth.empty( ) ) {
      if( cmd->te ) { _term += "--workdir " + cmd->wpth + " "; }
      else { _cmd += "cd " + cmd->wpth + "; "; }
    }
    // Nezavirat terminal
    if( cmd->lt == true ) { _term += a_cfg->term_noclose + " "; }
    _term += a_cfg->term_run + " ";
  }
  // Prikaz
  if( !cmd->cmd.empty( ) ) { _cmd += cmd->cmd; }
  // Parametry
  if( !cmd->prm.empty( ) ) { _cmd += cmd->prm; }
  // Parametry
  if( cmd->ow == true ) { _cmd += "&"; }
  //if( cmd->te == true ) { _cmd = "\"" + _cmd + "\""; }

  if( _cmd.empty( ) != true  ) {
    _command += _term + _cmd;
    std::cout << _command.text( ) << std::endl;
    resh = system( _command.text( ) );
  }
  /*
    FXProcess proc;
    proc.start( _cmd.text( ), _args.data( ) );
    a_processes->insert( &proc );
  */
  //std::cout<< "Execute resulth " << resh << std::endl;

  return resh;
}

void Application::task_write( Task *cmd, const FXString &pth )
{
  if( ( pth.empty( ) == true ) || ( cmd == NULL )  ) { return; }

  std::cout << "writing " << pth.text( ) << std::endl;

  FXSettings desk_file;
  FXString   desk_head = "Desktop Entry";
  FXString   command, name = FXPath::name( cmd->cmd );

  if( cmd->su == true ) { command += a_cfg->su + " "; }
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

void Application::settings_load( )
{
  reg( ).read( );
  /*
  std::cout << "Application key    :" << reg( ).getAppKey( ).text( )            << std::endl;
  std::cout << "Vendor key         :" << reg( ).getVendorKey( ).text( )         << std::endl;
  std::cout << "System directories :" << reg( ).getSystemDirectories( ).text( ) << std::endl;
  std::cout << "User directory     :" << reg( ).getUserDirectory( ).text( )     << std::endl;
  std::cout << "Theme section num  :" << reg( ).used( )                         << std::endl;
  for ( FXint i = 0; i != reg( ).no( ); i++ ) {
    if( !reg( ).empty( i ) ) {
      std::cout << "\tRegistry section (" << i << "): " << reg( ).key( i ).text( ) << std::endl; 
    }
  }
  */
   
  a_cfg->su           = reg( ).readStringEntry( getClassName( ), "Change user",  "kdesudo" );
  a_cfg->term         = reg( ).readStringEntry( getClassName( ), "Terminal",     "konsole" );
  a_cfg->term_noclose = reg( ).readStringEntry( getClassName( ), "No close term", "--noclose" );
  a_cfg->term_run     = reg( ).readStringEntry( getClassName( ), "Term run",      "-e" );
  //a_cfg-> = reg( ).readStringEntry( getClassName( ), "", FXString::null ); */

  // GUI Icons theme
  FXString icons_name, icons_map; 
  icons_name = reg( ).readStringEntry( "Theme", "icons", "Oxygen" );
  icons_map  = reg( ).readStringEntry( "Theme", "imap",  "/usr/share/FXRunner/data/icons.map" );
  a_iconsth->load( icons_map, icons_name );
}

void Application::settings_save( )
{
  reg( ).writeStringEntry( getClassName( ), "Change user",   a_cfg->su.text( ) );
  reg( ).writeStringEntry( getClassName( ), "Terminal",      a_cfg->term.text( ) );
  reg( ).writeStringEntry( getClassName( ), "No close term", a_cfg->term_noclose.text( ) );
  reg( ).writeStringEntry( getClassName( ), "Term run",      a_cfg->term_run.text( ) );
  reg( ).writeStringEntry( getClassName( ), "Icons path",    a_cfg->icon_path.text( ) );
  //reg( ).writeStringEntry( getClassName( ), "", a_cfg->  );

  if( reg( ).isModified( ) == true ) { reg( ).write( ); }
}

/*************************************************************************************************/
/* END */
