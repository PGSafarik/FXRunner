/*************************************************************************
* Runner.cpp Copyright (c) 2012 - 2022 by  D.A.Tiger                     *
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
#include<Runner.h>

FXDEFMAP( Runner ) RUNNER_MAP[ ] = {
  FXMAPFUNCS( SEL_COMMAND, Runner::ID_ACCEPT, Runner::ID_CANCEL, Runner::onCmd_Run ),
  FXMAPFUNCS( SEL_COMMAND, Runner::OPEN_DIR,  Runner::OPEN_HELP, Runner::onCmd_Open ),
  FXMAPFUNCS( SEL_COMMAND, Runner::ID_NOQUIT, Runner::HYSTORY_CLEAR, Runner::onCmd_Tools )
};

FXIMPLEMENT( Runner, FXGWindow, RUNNER_MAP, ARRAYNUMBER( RUNNER_MAP ) )

Runner::Runner( Application *a )
      : FXGWindow( a, "Fox Runner", NULL, NULL, CONTROLS_STATIC | WINDOW_PRIMARY | WINDOW_STATIC , 0, 0, 570, 110 )
{

  FXString cmd;
  app = a;

  this->load( );
  
  //  Window composite mask
  FXVerticalFrame *content = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL );
  
  /* Aplication layout */
  FXVerticalFrame   *up_frame   = new FXVerticalFrame( content, FRAME_NONE| LAYOUT_FILL);
  FXMatrix          *matrix     = new FXMatrix( up_frame, 2, FRAME_NONE | MATRIX_BY_COLUMNS | LAYOUT_FILL );
  new FXStatusBar( content, FRAME_RAISED | LAYOUT_SIDE_BOTTOM | LAYOUT_BOTTOM | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0  );

  /* Command field */
  FXint ncol = 50;
  new FXLabel( matrix, "Command to run: ", NULL, JUSTIFY_LEFT | LAYOUT_FILL );
  FXHorizontalFrame *cmdfrm = new FXHorizontalFrame( matrix, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0  );
  r_combo = new FXComboBox( cmdfrm, 10, NULL, 0, FRAME_GROOVE | COMBOBOX_NORMAL | LAYOUT_FILL  );
  r_combo->setNumVisible( 10 );

  /* Work dir field */ 
  new FXLabel( matrix, "Workdir: ", NULL, JUSTIFY_LEFT | LAYOUT_FILL_X );
  r_tfield = new FXTextField( matrix, ncol, NULL, 0, TEXTFIELD_NORMAL | LAYOUT_FILL_X );
  r_tfield->setText( r_WorkDir );

  /* HEADER BAR */
  FXWindowHeader *whb = this->getHeader( );
  FXIconsTheme *icons = app->get_iconstheme( );
  setIcon( icons->get_icon( "run", "Menu" ) );
  FXString      ver_str = AutoVersion::UBUNTU_VERSION_STYLE;
  ver_str += " ["; 
  ver_str += AutoVersion::STATUS;
  ver_str += "]";
  //whb->setText( ver_str );
  
  new FXMenuBox( whb, this->getMenuIcon( ) );
  new FXVerticalSeparator( whb );
  new Toolbar( whb );
  new FXOptionsBox( whb, this->getMenuIcon( true ) );

  /* Initialize */
  r_acmd = new Task;

  this->readHistory( r_history );
  r_combo->setText( "" );
}


Runner::~Runner( )
{
  //FXint num = 0;

  //if( r_acmd != NULL ) { delete r_acmd; }
  r_history.save( r_CacheDir + "/History", true );

  /*
  FXFileStream b_fd( r_CacheDir + "/History.buf", FXStreamSave );
  if( ( num = p_buffer.no( ) ) > 0 ) {
    std::cout << "Number object for saving on stream: " << num << std::endl;
    for( FXint i = 0; i != num; i++ ) {
      p_buffer[ i ]->save( b_fd );
    }
  }
  b_fd.close( );
  */
}

void Runner::create( )
{
  

  FXGWindow::create( );
  show( PLACEMENT_SCREEN );
}

long Runner::onCmd_Run( FXObject *tgt, FXSelector sel, void *data )
{
  long     resh = 1;
  FXString err_str = "";     // Hlaseni uzivatelseke chyby
  FXbool   err_flg = false;  // Priznak uzivatelske chyby

  switch( FXSELID( sel ) ) {
    case Runner::ID_ACCEPT : {
      ///FXint id = -1;
      // Get command and working path
      FXString cmd = r_combo->getText( );
      FXString pth = r_tfield->getText( );

      if( r_acmd == NULL ) { r_acmd = new Task; }
      // Set command
      if( cmd.empty( ) != true ) { r_acmd->cmd = cmd; }
      else {
		err_flg = true;
	    err_str = "Pole pro zadani prikazu nesmi byt prazdne!\nZadejte, prosim, pozadovany prikaz ke spusteni";
	  }
	  // Set command working path
      r_acmd->prm = FXString::null;
      if( pth.empty( ) != true ) {
        r_acmd->wpth = pth;
        if( r_acmd->wpth[ r_acmd->wpth.length( ) - 1 ] != '/' ) { r_acmd->wpth += "/"; }
      }
      // Compile the command
      if( err_flg != true ) {
        // Create the gui Link (The desktop file)
        ///if( r_acmd->cl == true ) { app->command_write( r_acmd, r_WorkDir ); }
        // Cretae the alias (on .~/profile)
        ///if( this->OnAlias == true ) { app->command_write( r_acmd, "~/.profile", Compile::ALIAS ); }
        // Running application
        app->task_exec( r_acmd );
        ///if( exec( r_acmd ) == false ) { FXMessageBox::warning( this, MBOX_OK, "Novy proces", " Proces nebyl spusten!" ); }
        ///app->command_write( r_acmd, r_lpth );
        // Set command history
        r_history.insert( r_acmd->cmd );
        this->readHistory( r_history );
        //this->p_buffer.append( r_acmd );
        r_combo->setText( "" );
        // Command reset
        r_acmd = NULL;
      }
      else {
        FXMessageBox::error( this, MBOX_OK, "Chybne zadani", err_str.text( ) );
        resh = 0;
      }
      break;
    }
    case Runner::ID_CANCEL : {
      FXuint answer = FXMessageBox::question( this, MBOX_YES_NO, "Dotaz", "Opravdu chcete ukoncit aplikaci Runner?" );
      if( answer == MBOX_CLICKED_YES ) { app->handle( this, FXSEL( SEL_COMMAND, FXApp::ID_QUIT ), NULL ); }
      else{ err_flg = true; }
      break;
    }
  }

  if( ( r_NoQuit == false ) && ( err_flg == false ) ) { app->handle( this, FXSEL( SEL_COMMAND, FXApp::ID_QUIT ), NULL ); }
  else { r_acmd = new Task; }
  return resh;
}

void Runner::load( )
{
  FXApp *a = getApp( );

  // Nacist ulozena configuracni data
  r_ShareDir = a->reg( ).readStringEntry( "Path",   "Share",   ( FXSystem::getHomeDirectory( ) + "/.local/share/" + a->getAppName( ) ).text( ) );
  r_CacheDir = a->reg( ).readStringEntry( "Path",   "Cache",   ( FXSystem::getHomeDirectory( ) + "/.cache/" + a->getAppName( ) ).text( ) );
  r_WorkDir  = a->reg( ).readStringEntry( "Path",   "Work",    FXSystem::getHomeDirectory( ).text( ) );

  r_NoQuit = a->reg( ).readBoolEntry(     "Runner", "NoQuit",     false );
  r_SilentQuit = a->reg( ).readBoolEntry( "Runner", "SilentQuit", false );

  r_SuDialog = a->reg( ).readStringEntry( "Utils", "SUCheckDialog",           "/usr/bin/kdesudo" );
  r_SuDialog = a->reg( ).readStringEntry( "Utils", "SUCheckDialog.execute",   "-c" );
  r_SuDialog = a->reg( ).readStringEntry( "Utils", "TerminalEmulator",        "/usr/bin/xterm" );
  r_SuDialog = a->reg( ).readStringEntry( "Utils", "TerminalEmulator.execute", "-e" );
  r_SuDialog = a->reg( ).readStringEntry( "Utils", "TerminalEmulator.NoClose", FXString::null );
  // Kontrola adresaru
  if( FXStat::exists( r_ShareDir ) == false ) { FXDir::create( r_ShareDir, FXIO::OwnerFull ); }
  if( FXStat::exists( r_CacheDir ) == false ) { FXDir::create( r_CacheDir, FXIO::OwnerFull ); }



  // Historie
  r_history.load( r_CacheDir + "/History" );
}

void Runner::save( )
{


}

/*************************************************************************************************/
long Runner::onCmd_Open( FXObject *tgt, FXSelector sel, void *data )
{
   switch( FXSELID( sel ) ) {
     case Runner::OPEN_FILE :{
       FXString file, pth;

       FXFileDialog filedlg( this, "Vyberte soubor ke spusteni :" );
       if( filedlg.execute( ) ) {
         pth  = filedlg.getDirectory( );
         file = filedlg.getFilename( );
         if( !pth.empty( ) ) {
           if( pth[ pth.length( ) - 1 ] != '/' ) { pth += "/"; }
         }
         r_combo->setText( file );
       }
       break;
     }
     case Runner::OPEN_DIR :{
        FXDirDialog *dir = new FXDirDialog ( app, "Vyberte pracovni adresar aplikace :" );
        if( dir->execute( ) ) {
          r_tfield->setText( dir->getDirectory( ) );
        }
       break;
     }
     case Runner::OPEN_OPTIONS :{
       SettingsDialog *set = new SettingsDialog( app );
       if( set->execute( ) ) { }
       break; 
     }
   }

   return 1;
}

long Runner::onCmd_Tools( FXObject *tgt, FXSelector sel, void *data )
{
   FXMenuCheck *check = ( FXMenuCheck* ) tgt;
   FXbool status = check->getCheck( );

   switch( FXSELID( sel ) ) {
     case Runner::ID_USER     : { r_acmd->su  = status; break; }
     case Runner::ID_ANNOUNCE : { r_acmd->ow  = status; break; }
     case Runner::ID_LINK     : { r_acmd->cl  = status; break; }
     case Runner::ID_TERMINAL : { r_acmd->te  = status; break; }
     case Runner::ID_TERMLOCK : { r_acmd->lt  = status; break; }
     case Runner::ID_NOQUIT   : { r_NoQuit    = status; break; }
     case Runner::HYSTORY_CLEAR : {
       r_combo->clearItems( );
       r_history._clear( );
     }
   }
   //
   if( r_acmd->te == false ) { r_acmd->lt = false; }

   return 1;
}



FXint Runner::readHistory( const History_b &buff )
{
  FXint id, num = 0;
  FXString cmd;

  r_combo->clearItems( );
  if( buff.no( ) > 0 ) {
    for( FXint i = ( buff.no( ) - 1 ); i >= 0; i-- ) {
      cmd = buff.at( i );
      //std::cout << cmd.text( ) << std::endl;
      if( r_combo->findItem( cmd ) == -1 ) {
        id = r_combo->getNumItems( );
        r_combo->insertItem( id, cmd );
        num++;
      }
    }
  }

  return num;
}

/*** END ******************************************************************************************/
