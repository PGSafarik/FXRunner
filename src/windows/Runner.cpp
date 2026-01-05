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
#include "windows/Runner.h"

FXDEFMAP( Runner ) RUNNER_MAP[ ] = {
  FXMAPFUNCS( SEL_COMMAND, Runner::ID_ACCEPT,      Runner::ID_CANCEL,   Runner::onCmd_Run ),
  FXMAPFUNCS( SEL_COMMAND, Runner::OPEN_DIR,       Runner::OPEN_HELP,   Runner::onCmd_Open ),
  FXMAPFUNC( SEL_COMMAND,  Runner::HISTORY_EVENT,  Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_UPDATE,   Runner::HISTORY_EVENT,  Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_INSERTED, Runner::HISTORY_EVENT,  Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_DELETED,  Runner::HISTORY_EVENT,  Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_COMMAND,  Runner::HISTORY_SHOW,   Runner::on_HistoryShow ),
  FXMAPFUNC( SEL_UPDATE,   Runner::HISTORY_SHOW,   Runner::on_HistoryShow ),
  FXMAPFUNC( SEL_CHANGED,  Runner::ID_RECONFIGURE, Runner::on_OptionsChange ),
};

FXIMPLEMENT( Runner, FXPrimaryWindow, RUNNER_MAP, ARRAYNUMBER( RUNNER_MAP ) )

Runner::Runner( Application *a )
      : FXPrimaryWindow( a, "Fox Runner", NULL, NULL, CONTROLS_STATIC | WINDOW_MAIN | WINDOW_STATIC , 0, 0, 570, 80 )
{
  FXString cmd;
  r_app = a;
  setWMDecorations( WM_DECOR_PRIMARY );

  /* Command field */
  FXGroupBox *exec_fr = new FXGroupBox( this, "Command to run: ", GROUPBOX_NORMAL | LAYOUT_FILL_X );
  FXint ncol = 50;
  m_cmdfield = new FXTextField( exec_fr, ncol, nullptr, 0, TEXTFIELD_NORMAL | LAYOUT_FILL_X );

  /* Status bar */
  new FXStatusBar( this, FRAME_RAISED | LAYOUT_SIDE_BOTTOM | LAYOUT_BOTTOM | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0  );

  /* HEADER BAR */
  FXWindowHeader *whb = this->getHeader( );
  FXIconsTheme *icons = r_app->get_iconstheme( );
  setIcon( icons->get_icon( "run", "Menu" ) );

  //new FXVerticalSeparator( whb );
  m_runbox = new RunBox( whb );
  new FXMenuBox( whb, this->getMenuIcon(  true ) );
  /* Initialize */
  GetHistory( )->set_target( this );
  GetHistory( )->set_notify( HISTORY_EVENT );
  m_cmdfield->setText( "" );

  ShortCuts( );
}

Runner::~Runner( )
{  }

void Runner::create( )
{
  FXPrimaryWindow::create( );

  show( PLACEMENT_SCREEN );
  DefaultFocus( );
}


/**************************************************************************************************/
long Runner::onCmd_Run( FXObject *tgt, FXSelector sel, void *data )
{
  long     resh = 1;
  FXbool   quit = false;  // flag for exit this application.
  FXuint   msg_id = FXSELID( sel );
  FXuint   msg_type = FXSELTYPE( sel );

  switch( msg_id ) {
    case Runner::ID_ACCEPT : {
      /* FIXME RUNNER_002: The task should always be launched from the top of the history stack. (pos = 0).
         Tzn. Spravne by se zde nemel task vubec resit, ale pripadne pozadat o jeho vytvoreni a hlavne nastavit podle
         uzivatelovych pokynu pres GUI formular. Jeho spusteni by pak bylo zalozitosti FXApplication::task_exec( ),
         ktera si task ke spusteni muze klidne vyzvednout na vrcholu zasobniku historie. */

      if( PrepareTask( ) ) {
        m_runbox->get_Modes( )->handle( this, FXSEL( SEL_COMMAND, RunModes::MODE_APPLY ), nullptr );
#ifdef DEBUG
        DEBUG_OUT( "Dumping item for run: " )
        GetHistory( )->at( )->Dump( );
# endif
        r_app->task_exec( );                       // Running command
        Update( );                                 // Actualize gui forms
        if( r_app->autoexit( ) ) { quit = true; }
      }

      break;
    }
    case Runner::ID_CANCEL : {
      FXuint answer = FXMessageBox::question( this, MBOX_YES_NO, "Question", "Really quit?" );
      if( answer == MBOX_CLICKED_YES ) { quit = true; }
      break;
    }
  }

  if( quit ) { r_app->handle( this, FXSEL( SEL_COMMAND, FXApp::ID_QUIT ), nullptr ); }
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
         m_cmdfield->setText( file );
       }
       break;
     }
     case Runner::OPEN_DIR :{
        FXDirDialog *dir = new FXDirDialog ( r_app, "Select workdir:" );
        if( dir->execute( ) ) {
          //r_tfield->setText( dir->getDirectory( ) );
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

long Runner::on_HistoryEvent( FXObject *tgt, FXSelector sel, void *data )
{
  FXuint msg_id = FXSELID( sel );
  FXuint msg_type = FXSELTYPE( sel );

  if( msg_id == HISTORY_EVENT ) {
    switch( msg_type ) {
      case SEL_UPDATE : {
        DEBUG_OUT(  tgt->getClassName(  ) << " - History updated" )
        Task *task = GetHistory( )->at( );
        if( task ) {
          m_cmdfield->setText( task->cmd );
          m_runbox->get_Modes( )->handle( this, FXSEL( SEL_COMMAND, RunModes::MODE_UPDATE ), task );
        }
        break;
      }
      /* Nothig for this time
      case SEL_COMMAND : {

        break;
      }

      case SEL_INSERTED : {

        break;
      }

      case SEL_DELETED : {

        break;
      }
      */
    }
  }

  return 1;
}

long Runner::on_HistoryShow( FXObject *tgt, FXSelector sel, void *data )
{
  long resh = 1;

  switch ( FXSELTYPE( sel ) ) {
    case SEL_COMMAND :
    {
      HistoryView hview( this );
      hview.execute( );
      resh = 0;
      break;
    }

    case SEL_UPDATE :
    {
      FXuint msg = ( GetHistory( )->no( ) > 0 ) ? FXMenuCommand::ID_ENABLE : FXMenuCommand::ID_DISABLE;
      resh = tgt->handle( this, FXSEL( SEL_COMMAND, msg ), nullptr );

      break;
    }
  }

  return resh;
}

long Runner::on_OptionsChange( FXObject *tgt, FXSelector sel, void *data )
{
  m_runbox->get_Modes( )->handle( this, FXSEL( SEL_CHANGED, RunModes::ID_RECONFIGURE ), nullptr );
  return 1;
}

/**************************************************************************************************/
void Runner::DefaultFocus( )
{
   if( !m_cmdfield->hasFocus( ) ) { m_cmdfield->setFocus( ); }
}

void Runner::ShortCuts( )
{
  FXAccelTable *act = getShell( )->getAccelTable( );
  if( act ) {
    act->addAccel( "Esc",    this, FXSEL( SEL_COMMAND, ID_CANCEL ) );       // [ESC]ape
    act->addAccel( "alt+r",  this, FXSEL( SEL_COMMAND, ID_ACCEPT ) );       // [R]un command
    act->addAccel( "alt+s",  this, FXSEL( SEL_COMMAND, OPEN_FILE ) );       // [S]elect file
    act->addAccel( "alt+d",  this, FXSEL( SEL_COMMAND, OPEN_DIR ) );        // Select work [D]irectory
    act->addAccel( "alt+c",  this, FXSEL( SEL_COMMAND, OPEN_OPTIONS ) );    // [C]onfigure
  }
}

FXbool Runner::PrepareTask( )
{
  FXString err_str = "";                 // Hlaseni uzivatelseke chyby
  FXbool   resh_flg = false;                // Priznak uspesne pripravy tasku

  FXString cmd = m_cmdfield->getText( );

  if( !cmd.empty( ) ) {
    FXint f_index = GetHistory( )->find( cmd );
    resh_flg = ( f_index < 0 ? GetHistory( )->add( cmd, true ) : GetHistory( )->current( f_index, false ) );
  }
  else {
    err_str = "Do you want to run the last command entered:\n '";
    err_str += GetHistory( )->at( )->cmd + "'?";

    if( FXMessageBox::question( this, MBOX_YES_NO, "Incorrect input", err_str.text( ) ) == MBOX_CLICKED_NO ) {
      FXMessageBox::error( this, MBOX_OK, "Incorrect input", "The command input field must not be empty!" );
    }
    else { resh_flg = true; }
  }

  return resh_flg;
}

Task* Runner::SetTask( Task *task )
{
  return nullptr;
}

void Runner::Update( )
{
  m_runbox->get_Modes( )->handle( this, FXSEL( SEL_COMMAND, RunModes::MODE_RESET ), nullptr );
  m_cmdfield->setText( "" );
}

/*** END ******************************************************************************************/
