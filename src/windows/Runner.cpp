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
  FXMAPFUNCS( SEL_COMMAND, Runner::ID_ACCEPT,     Runner::ID_CANCEL,   Runner::onCmd_Run ),
  FXMAPFUNCS( SEL_COMMAND, Runner::OPEN_DIR,      Runner::OPEN_HELP,   Runner::onCmd_Open ),
  FXMAPFUNCS( SEL_COMMAND, Runner::ID_NOQUIT,     Runner::ID_TERMLOCK, Runner::onCmd_Tools ),
  FXMAPFUNC(  SEL_UPDATE,  Runner::ID_NOQUIT,     Runner::onCmd_Tools ),
  FXMAPFUNC( SEL_COMMAND,  Runner::HISTORY_EVENT, Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_UPDATE,   Runner::HISTORY_EVENT, Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_INSERTED, Runner::HISTORY_EVENT, Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_DELETED,  Runner::HISTORY_EVENT, Runner::on_HistoryEvent ),
  FXMAPFUNC( SEL_COMMAND,  Runner::HISTORY_SHOW,  Runner::on_HistoryShow ),
  FXMAPFUNC( SEL_UPDATE,   Runner::HISTORY_SHOW,  Runner::on_HistoryShow ),
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

  new FXMenuBox( whb, this->getMenuIcon( ) );
  new FXVerticalSeparator( whb );
  m_runbox = new RunBox( whb );

  /* Initialize */
  GetHistory( )->set_target( this );
  GetHistory( )->set_notify( HISTORY_EVENT );

  this->LoadHistory( );
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
      Task *task = nullptr;

      switch ( msg_type ) {
        case SEL_COMMAND : {
          // Command -> Create new task and insert on top in history
          task = MakeTask( );
          break;
        }
        default: {
          // Used select task in history -> see note RUNNER_002 at up.
          task = GetHistory( )->at( );
        }
      }

      // Compile the command
      if( task ) {
        m_runbox->get_Modes( )->handle( this, FXSEL( SEL_COMMAND, RunModes::MODE_APPLY ), task );
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

  if( quit ) { r_app->handle( this, FXSEL( SEL_COMMAND, FXApp::ID_QUIT ), NULL ); }
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

long Runner::onCmd_Tools( FXObject *tgt, FXSelector sel, void *data )
{
  FXMenuCheck *check = ( FXMenuCheck* ) tgt;
  FXbool status  = check->getCheck( );
  FXuint msgtype = FXSELTYPE( sel );
  FXuint msgid   = FXSELID( sel );

  switch( msgid ) {
    case Runner::ID_USER     : { /*r_acmd->su*/ r_prop.suaccess    = status; break; }
    case Runner::ID_ANNOUNCE : { /*r_acmd->ow*/ r_prop.unblock     = status; break; }
    case Runner::ID_TERMINAL : { /*r_acmd->te*/ r_prop.term        = status; break; } // FIXME RUNNER_001: Nefunguje?
    case Runner::ID_TERMLOCK : { /*r_acmd->lt*/ r_prop.nocloseterm = status; break; } //  FIXME RUNNER_001: Nefunguje?
    //case Runner::ID_LINK     : { r_acmd->cl  = status; break; }
    case Runner::ID_NOQUIT   : { 
      if( msgtype == SEL_UPDATE ) {
        FXbool state =  r_app->autoexit( );
        check->setCheck( state ); 
      }
      else { r_app->handle( this, FXSEL( SEL_COMMAND, Application::QUIT_NEGATION ), NULL ); }
      break; 
    }
  }
   
  if( !r_prop.term ) { r_prop.nocloseterm = false; }

  return 1;
}

long Runner::on_HistoryEvent( FXObject *tgt, FXSelector sel, void *data )
{
  FXuint msg_id = FXSELID( sel );
  FXuint msg_type = FXSELTYPE( sel );

  if( msg_id == HISTORY_EVENT ) {
    switch( msg_type ) {
      case SEL_UPDATE : {
        std::cout << tgt->getClassName(  ) << " - History updated" << std::endl;
        Task *task = GetHistory( )->at( );
        if( task ) {
          m_cmdfield->setText( task->cmd );
          m_runbox->get_Modes( )->handle( this, FXSEL( SEL_COMMAND, RunModes::ID_UPDATE), nullptr );
        }
        break;
      }

      case SEL_COMMAND : {

        break;
      }

      case SEL_INSERTED : {

        break;
      }

      case SEL_DELETED : {

        break;
      }
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

/**************************************************************************************************/
void Runner::LoadHistory( )
{
  History *history = GetHistory( );
  FXint num = history->no( );
  FXString cmd;
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

Task* Runner::MakeTask( )
{
  Task *task = nullptr;
  FXString err_str = "";     // Hlaseni uzivatelseke chyby
  FXbool   err_flg = false;     // Priznak uzivatelske chyby

  // Get command and working path
  FXString cmd = m_cmdfield->getText( );

  // Set command
  if( !cmd.empty( ) ) {
    task = GetHistory(  )->add( cmd, true );

    // Set properties
    Check_property( task );
  }
  else {
      err_str = "The field for entering the command must not be empty!\nPlease enter the required command to run";
      FXMessageBox::error( this, MBOX_OK, "Incorrect input", err_str.text( ) );
  }

  return task;
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
