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
  FXMAPFUNCS( SEL_COMMAND, Runner::ID_NOQUIT, Runner::HYSTORY_CLEAR, Runner::onCmd_Tools ),
  FXMAPFUNC(  SEL_UPDATE,  Runner::ID_NOQUIT, Runner::onCmd_Tools )
};

FXIMPLEMENT( Runner, FXPrimaryWindow, RUNNER_MAP, ARRAYNUMBER( RUNNER_MAP ) )

Runner::Runner( Application *a )
      : FXPrimaryWindow( a, "Fox Runner", NULL, NULL, CONTROLS_STATIC | WINDOW_MAIN | WINDOW_STATIC , 0, 0, 570, 110 )
{
  FXString cmd;
  r_app = a;
  
  //  Window composite mask
  setWMDecorations( WM_DECOR_PRIMARY );
  FXVerticalFrame *content = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0 );
  
  /* Aplication layout */
  FXVerticalFrame *up_frame = new FXVerticalFrame( content, FRAME_NONE| LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0 );
  FXMatrix        *matrix   = new FXMatrix( up_frame, 2, FRAME_NONE | MATRIX_BY_COLUMNS | LAYOUT_FILL );
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
  r_tfield->setText( FXSystem::getHomeDirectory( ) );

  /* HEADER BAR */
  FXWindowHeader *whb = this->getHeader( );
  FXIconsTheme *icons = r_app->get_iconstheme( );
  setIcon( icons->get_icon( "run", "Menu" ) );

  new FXMenuBox( whb, this->getMenuIcon( ) );
  new FXVerticalSeparator( whb );
  new Toolbar( whb );
  new FXOptionsBox( whb, this->getMenuIcon( true ) );

  /* Initialize */
  this->LoadHistory( );
  r_combo->setText( "" );

}


Runner::~Runner( )
{  }

void Runner::create( )
{
  FXPrimaryWindow::create( );
  show( PLACEMENT_SCREEN );
}


/**************************************************************************************************/
long Runner::onCmd_Run( FXObject *tgt, FXSelector sel, void *data )
{
  long     resh = 1;
  FXString err_str = "";     // Hlaseni uzivatelseke chyby
  FXbool   err_flg = false;  // Priznak uzivatelske chyby

  switch( FXSELID( sel ) ) {
    case Runner::ID_ACCEPT : {
      // Get command and working path
      FXString cmd = r_combo->getText( );
      FXString pth = r_tfield->getText( );
      
      // Set command
      if( cmd.empty( ) != true ) { 
        Task *task = new Task;
        task->cmd = cmd; 

	      // Set command working path
        task->prm = FXString::null;
        if( pth.empty( ) != true ) {
          task->wpth = pth;
          if( task->wpth[ task->wpth.length( ) - 1 ] != '/' ) { task->wpth += "/"; }
        }

        // Set properties
        Check_property( task );

        // Compile the command
        if( err_flg != true ) {
          r_app->task_exec( task );         // Running application
          GetHistory( )->insert( task );  // Insert command to history
          this->LoadHistory( );            // Aktualize history command list in combobox
          r_combo->setText( "" );           // Clean command text 

          // Command reset
          //delete task;  // I do not like it... :( if will completed task history, ok )
        }
        else {
          FXMessageBox::error( this, MBOX_OK, "Incorrect input", err_str.text( ) );
          resh = 0;
        } 
      }
      else {
		    err_flg = true;
	      err_str = "The field for entering the command must not be empty!\nPlease enter the required command to run";
	    }

      break;
    }

    case Runner::ID_CANCEL : {
      FXuint answer = FXMessageBox::question( this, MBOX_YES_NO, "Question", "Really quit?" );
      if( answer == MBOX_CLICKED_YES ) { r_app->handle( this, FXSEL( SEL_COMMAND, FXApp::ID_QUIT ), NULL ); }
      else{ err_flg = true; }
      break;
    }
  }

  if( r_app->autoexit( ) && !err_flg ) { r_app->handle( this, FXSEL( SEL_COMMAND, FXApp::ID_QUIT ), NULL ); }
  return resh;
}

long Runner::onCmd_Open( FXObject *tgt, FXSelector sel, void *data )
{
   switch( FXSELID( sel ) ) {
     case Runner::OPEN_FILE :{
       FXString file, pth;

       FXFileDialog filedlg( this, "Select executable file:" );
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
        FXDirDialog *dir = new FXDirDialog ( r_app, "Select workdir:" );
        if( dir->execute( ) ) {
          r_tfield->setText( dir->getDirectory( ) );
        }
       break;
     }
     case Runner::OPEN_OPTIONS :{
       SettingsDialog *set = new SettingsDialog( this );
       //if( set->execute( ) ) { }
       set->execute( );
       break; 
     }
   }

   return 1;
}

long Runner::onCmd_Tools( FXObject *tgt, FXSelector sel, void *data )
{
  FXMenuCheck *check = ( FXMenuCheck* ) tgt;
  FXbool status  = check->getCheck( );
  FXuint msgtype = FXSELTYPE( sel );
  FXuint msgid   = FXSELID( sel );

  switch( msgid ) {
    case Runner::ID_USER     : { /*r_acmd->su*/ r_prop.suaccess    = status; break; }
    case Runner::ID_ANNOUNCE : { /*r_acmd->ow*/ r_prop.unblock     = status; break; }
    case Runner::ID_TERMINAL : { /*r_acmd->te*/ r_prop.term        = status; break; }
    case Runner::ID_TERMLOCK : { /*r_acmd->lt*/ r_prop.nocloseterm = status; break; }
    //case Runner::ID_LINK     : { r_acmd->cl  = status; break; }
    case Runner::ID_NOQUIT   : { 
      if( msgtype == SEL_UPDATE ) {
        FXbool state =  r_app->autoexit( );
        check->setCheck( state ); 
      }
      else { r_app->handle( this, FXSEL( SEL_COMMAND, Application::QUIT_NEGATION ), NULL ); }
      break; 
    }
    case Runner::HYSTORY_CLEAR : {
      r_combo->clearItems( );
      GetHistory( )->clear( );
    }
  }
   
  if( r_prop.term == false ) { r_prop.nocloseterm = false; }

  return 1;
}

/**************************************************************************************************/
void Runner::LoadHistory( )
{
  History *history = GetHistory( );
  FXint num = history->no( );
  FXString cmd;
  
  r_combo->clearItems( );
  for( FXint i = 0; i != num; i++ ) {
    Task *entry = history->at( i, false );
    r_combo->insertItem( i, entry->cmd );
  }
}

void Runner::Check_property( Task *task )
{
  if( task ) {
    task->prop->suaccess = r_prop.suaccess;
    task->prop->unblock = r_prop.unblock;
    task->prop->term = r_prop.term;
    task->prop->nocloseterm = r_prop.nocloseterm;
  }
}

/*** END ******************************************************************************************/
