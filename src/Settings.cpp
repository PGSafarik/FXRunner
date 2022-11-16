/*************************************************************************
* Settings.cpp Copyright (c) 2022 by  D.A.Tiger                          *
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
#include<Settings.h>

/* SETTINGS FRAME */
#define LABEL_STYLE JUSTIFY_LEFT | ICON_BEFORE_TEXT | LAYOUT_FILL_X

FXDEFMAP( Settings ) SETTINGS_MAP[ ] = { 
  FXMAPFUNC( SEL_COMMAND, Settings::SETTINGS_SAVE,    Settings::onCmd_Settings ),
  FXMAPFUNC( SEL_UPDATE,  Settings::SETTINGS_SAVE,    Settings::onUpd_Settings ),
  FXMAPFUNC( SEL_COMMAND, Settings::SETTINGS_RESTORE, Settings::onCmd_Settings ),
  FXMAPFUNC( SEL_UPDATE,  Settings::SETTINGS_RESTORE, Settings::onUpd_Settings ),
  FXMAPFUNC( SEL_COMMAND, Settings::SETTINGS_DEFAULT, Settings::onCmd_Settings ),
  FXMAPFUNCS( SEL_COMMAND, Settings::SELECT_DIRECTORY, Settings::SELECT_FILE, Settings::onCmd_Select ),
  FXMAPFUNC( SEL_COMMAND, Settings::ID_CHANGE,        Settings::onCmd_Update   )
};
FXIMPLEMENT( Settings,FXScrollWindow, SETTINGS_MAP, ARRAYNUMBER( SETTINGS_MAP ) )

/*************************************************************************************************/
Settings::Settings( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts )
        :FXScrollWindow( p, VSCROLLER_ALWAYS | LAYOUT_FILL, 0, 0, 0, 0 ) 
{
  content  = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL ); 

  MakeTitle( "User interface" );
  uicb_IconsTheme = MakeComboBox( "Icons Theme: " ); 
  uicb_IconsTheme->setNumVisible( 5 );
  uicb_IconsTheme->appendItem( "Oxygen" );
  uicb_IconsTheme->appendItem( "Gnome" );
  uicb_IconsTheme->appendItem( "Adwaita" );
  uicb_IconsTheme->appendItem( "Faenza" );
  uitf_cache = MakeSelector( "Cache directory : ", this, Settings::SELECT_DIRECTORY ); 
  uich_aexit = MakeCheckButton( "Exit after run application");
  uich_sexit = MakeCheckButton( "Not to require confirmation of program termination" );

  MakeTitle( "Terminal emulator"  );
  tecb_enable = MakeComboBox( "Enable: " ); 
  tecb_enable->appendItem ( "Always" );     // Alway  all commands run in TE
  tecb_enable->appendItem ( "On request" ); // Use TE only on request ( also for sudo when askpass is not available ) 
  tecb_enable->appendItem ( "Disable" );    // Disable TE
  tecb_enable->setNumVisible( 3 );
  tetf_command    = MakeSelector( "Command: ", this, Settings::SELECT_FILE );
  tetf_execprm    = MakeTextField( "Argument for exec : " );
  tetf_disclosprm = MakeTextField(  "Argument for disable close : " );
  tetf_workdirprm = MakeTextField(  "Argument for set workpath : " );
  
  MakeTitle( "Super user access" );
  such_enable  = MakeCheckButton( "Enable using sudo" ); 
  such_askpass = MakeCheckButton( "Enable using askpass - must be installed!" ); 

  target   = tgt;
  message = sel;
}

Settings::~Settings( )
{ }

void Settings::create( )
{
  //read_config( );
  check( );
  m_change = false;

 FXScrollWindow::create( );
}

/*************************************************************************************************/
void Settings::check( )
{
  FXint        cfg_id;
  Application *app = dynamic_cast<Application*>( getApp( ) );

  if( app ) {
    cfg_id =  uicb_IconsTheme->findItem( app->a_cfg->icons_name );
    if( cfg_id >= 0 ) { uicb_IconsTheme->setCurrentItem( cfg_id ); } 
    uitf_cache->setText( app->a_cfg->cache_dir );
    uich_aexit->setCheck( app->a_cfg->auto_exit );
    uich_sexit->setCheck( app->a_cfg->silent_exit );

    such_enable->setCheck( app->a_cfg->sudo );
    such_askpass->setCheck( app->a_cfg->askpass );

    cfg_id = tecb_enable->findItem( app->a_cfg->term_enable );
    if( cfg_id >= 0 && !app->a_cfg->term.empty( ) ) { 
      tecb_enable->setCurrentItem( cfg_id ); 
      tetf_command->setText( app->a_cfg->term );
      tetf_execprm->setText( app->a_cfg->term_run );
      tetf_disclosprm->setText( app->a_cfg->term_noclose );
      tetf_workdirprm->setText( app->a_cfg->term_work );
    } 
    else { tecb_enable->setCurrentItem( 2 ); }
  }
}

void Settings::apply( )
{
  FXint        cfg_id;
  Application *app = dynamic_cast<Application*>( getApp( ) );

  cfg_id = uicb_IconsTheme->getCurrentItem( );  
  app->a_cfg->icons_name  = uicb_IconsTheme->getItem( cfg_id );
  app->a_cfg->cache_dir   = uitf_cache->getText( );
  app->a_cfg->auto_exit   = uich_aexit->getCheck( );
  app->a_cfg->silent_exit = uich_sexit->getCheck( );

  app->a_cfg->sudo    = such_enable->getCheck( );
  app->a_cfg->askpass = such_askpass->getCheck( );

  cfg_id = tecb_enable->getCurrentItem( );
  app->a_cfg->term_enable  = tecb_enable->getItem( cfg_id ).text( );
  app->a_cfg->term         = tetf_command->getText( ).text( );
  app->a_cfg->term_run     = tetf_execprm->getText( ).text( );
  app->a_cfg->term_noclose = tetf_disclosprm->getText( ).text( );
  app->a_cfg->term_work    = tetf_workdirprm->getText( ).text( );

  app->a_cfg->change = true; 
}

/*************************************************************************************************/
long Settings::onCmd_Settings( FXObject *sender, FXSelector sel, void *data )
{

  FXuint id = FXSELID( sel );

  switch( id ) {
    case Settings::SETTINGS_SAVE :
    {
      //write_config( ); 
      apply( );
      m_change = false;
      Notify( );

      break;
    }
    case Settings::SETTINGS_RESTORE :
    {
      /* 
      if( isBackup( ) )
      {
        if( FXMessageBox::question( this, MBOX_YES_NO, "Restore FoxGHI settings", "Do you really want to restore the original settings?" ) == MBOX_CLICKED_YES ) {
          getApp( )->reg( ).at( CFG_RUNNER ) = m_revert;
          m_revert.clear( );  
          read_config( );
          m_change = true;
          Notify( );
          
        }  
      }
      else { FXMessageBox::warning( this, MBOX_OK, "Restore FoxGHI settings", "Unfortunately there is no data available for recovery." ); } 
      */
      break; 
    }
    case Settings::SETTINGS_DEFAULT :
    {
      if( FXMessageBox::question( this, MBOX_YES_NO, "Predefined FoxGHI settings", "Do you really want predefined settings?" ) == MBOX_CLICKED_YES ) {
        /*
        if( getApp( )->reg( ).existingSection( CFG_RUNNER ) ) { getApp( )->reg( ).deleteSection( CFG_RUNNER ); }
        read_config( );
        m_change = true;
        Notify( );
        */
      }
      break;
    }
  }

  return 1;
}



long Settings::onUpd_Settings( FXObject *sender, FXSelector sel, void *data )
{
  FXWindow *actor = static_cast<FXWindow*>( sender );
  
  switch( FXSELID( sel ) ) {
    case Settings::SETTINGS_SAVE : 
    { 
      isChanged( ) ? actor->enable( ) : actor->disable( );
      break;
    }
    case Settings::SETTINGS_RESTORE :  
    { 
      isBackup( ) ? actor->enable( ) : actor->disable( );
      break;
    }
  }

  return 1;
}

long Settings::onCmd_Select( FXObject *sender, FXSelector sel, void *data )
{
  long resh = 1;
  FXButton    *btn = static_cast<FXButton*>( sender );
  FXTextField *tf  = static_cast<FXTextField*>( btn->getUserData( ) );

  switch( FXSELID( sel ) ) {
    case Settings::SELECT_DIRECTORY :
    {
      FXDirDialog dirdlg( this, "Select directory:" );
      if( dirdlg.execute( ) ) { tf->setText( dirdlg.getDirectory( ) ); } 
      break;
    } 
     
    case Settings::SELECT_FILE :
    {
      FXFileDialog filedlg( this, "Select file:" );
      if( filedlg.execute( ) ) { tf->setText( filedlg.getFilename( ) ); }
      break;
    }
  }

  return resh;
}

long Settings::onCmd_Update( FXObject *sender, FXSelector sel, void *data )
{
  
  m_change = true;
  return 0;
}

/**************************************************************************************************/
void Settings::MakeTitle( const FXString &text, FXIcon *ic )
{
  FXLabel *label = new FXLabel( content, text, ic, LABEL_NORMAL | LAYOUT_FILL_X  );
  label->setBackColor( getApp( )->getShadowColor( ) );
}

FXCheckButton* Settings::MakeCheckButton( const FXString &label )
{
  return new FXCheckButton( content, label, this, Settings::ID_CHANGE );
}

FXComboBox* Settings::MakeComboBox( const FXString &label )
{
 new FXLabel( content, label, NULL, LABEL_STYLE );
 FXHorizontalFrame *frame = new FXHorizontalFrame( content, FRAME_SUNKEN | LAYOUT_FILL_X, 0, 0, 0, 0,  1, 1, 1, 1 );
 return new FXComboBox( frame, 51, this, Settings::ID_CHANGE, COMBOBOX_NORMAL | LAYOUT_FILL_X );
}

FXTextField* Settings::MakeTextField( const FXString &label )
{
  new FXLabel( content, label, NULL, LABEL_STYLE);
  return new FXTextField( content, 51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 
}

FXTextField* Settings::MakeSelector( const FXString &label, FXObject *_tgt, FXSelector _sel )
{
  new FXLabel( content, label, NULL, LABEL_STYLE);
  FXHorizontalFrame *frame = new FXHorizontalFrame( content, FRAME_NONE | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0 );
  FXTextField *field = new FXTextField( frame, 51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 
  FXButton *button = new FXButton( frame, " ... ", NULL, _tgt, _sel, BUTTON_NORMAL );
  button->setUserData( field );

  return field;
}

/**************************************************************************************************/
/* SETTINGS DIALOG */
FXIMPLEMENT( SettingsDialog, FXGDialogBox, NULL, 0 )

/**************************************************************************************************/
SettingsDialog::SettingsDialog( FXApp *a )
              : FXGDialogBox( a, "Configure", WINDOW_STATIC, 0, 0, 550, 480 )
{
  
  Application  *app = ( Application * ) this->getApp( );
  FXIconsTheme *icons = app->get_iconstheme( );

  setIcon( icons->get_icon( "settings" ) );
  
  FXVerticalFrame *content = new FXVerticalFrame( this, FRAME_NONE | LAYOUT_FILL );
  Settings *config = new Settings( content, this, SettingsDialog::ID_RECONFIGURE );
  FXHorizontalSeparator( content, FRAME_GROOVE | LAYOUT_FILL_X );
  new FXStatusBar( content, FRAME_RAISED | LAYOUT_SIDE_BOTTOM | LAYOUT_BOTTOM | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0  );

  new FXButton( getHeader( ), "\t\t Accept", icons->get_icon( "accept", "HeaderBar" ),   config, Settings::SETTINGS_SAVE,  BUTTON_TOOLBAR | LAYOUT_LEFT ); 
  new FXVerticalSeparator( getHeader( ), SEPARATOR_GROOVE | LAYOUT_FILL_Y ); 
  new FXButton( getHeader( ), "\t\t Discard", icons->get_icon( "discard", "HeaderBar" ), config, Settings::SETTINGS_RESTORE,  BUTTON_TOOLBAR | LAYOUT_LEFT );
  new FXButton( getHeader( ), "\t\t Default", icons->get_icon( "default", "HeaderBar" ), config, Settings::SETTINGS_DEFAULT,  BUTTON_TOOLBAR | LAYOUT_LEFT );
}

SettingsDialog::~SettingsDialog( )
{ }

void SettingsDialog::create( )
{
  FXGDialogBox::create( );
  show( PLACEMENT_SCREEN );
}

/**************************************************************************************************/
/* END */
