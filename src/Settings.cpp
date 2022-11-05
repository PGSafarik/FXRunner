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
  FXMAPFUNC( SEL_COMMAND, Settings::ID_CHANGE,        Settings::onCmd_Update   )
};
FXIMPLEMENT( Settings, FXVerticalFrame, SETTINGS_MAP, ARRAYNUMBER( SETTINGS_MAP ) )

/*************************************************************************************************/
Settings::Settings( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts )
        : FXVerticalFrame( p, opts ) 
{
  MakeTitle( this, "Terminal emulator"  );
  //FXMatrix *mte = new FXMatrix( this, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X ); 

  new FXLabel( this, "Enable: ", NULL, LABEL_STYLE );
  tecb_enable = new FXComboBox( this, 51, this, Settings::ID_CHANGE, COMBOBOX_NORMAL | LAYOUT_FILL_X );
  tecb_enable->appendItem ( "Enable"  /*, FXptr ptr=nullptr, FXbool notify=false */);
  tecb_enable->appendItem ( "On request" );
  tecb_enable->appendItem ( "Disable" );
  tecb_enable->setNumVisible( 3 );
  //tecb_enable->setCurrentItem( 1 );

  new FXLabel( this, "Command: ", NULL, LABEL_STYLE);
  tetf_command = new FXTextField( this, 51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 

  new FXLabel( this, "Argument for exec : ", NULL, LABEL_STYLE );
  tetf_execprm = new FXTextField( this, 51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 

  new FXLabel( this, "Argument for disable close : ", NULL, LABEL_STYLE );
  tetf_disclosprm = new FXTextField( this,  51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 

  new FXLabel( this, "Argument for set workpath : ", NULL, LABEL_STYLE );
  tetf_workdirprm = new FXTextField( this,  51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 

  MakeTitle( this, "Super user access" );
  such_enable  = new FXCheckButton( this, "Enable using sudo", this, Settings::ID_CHANGE );
  such_askpass = new FXCheckButton( this, "Enable using askpass - must be installed!", this, Settings::ID_CHANGE );

  MakeTitle( this, "User interface" );

  new FXLabel( this, "Icons Theme: ", NULL, LABEL_STYLE );
  uicb_IconsTheme = new FXComboBox( this, 5, this, Settings::ID_CHANGE, COMBOBOX_NORMAL | LAYOUT_FILL_X );
  uicb_IconsTheme->setNumVisible( 5 );
  uicb_IconsTheme->appendItem( "Oxygen" );
  uicb_IconsTheme->appendItem( "Gnome" );
  uicb_IconsTheme->appendItem( "Adwaita" );
  uicb_IconsTheme->appendItem( "Faenza" );

  new FXLabel( this, "Cache directory : ", NULL, LABEL_STYLE );
  tetf_execprm = new FXTextField( this, 51, this, Settings::ID_CHANGE, TEXTFIELD_NORMAL | LAYOUT_FILL_X ); 
  
  target   = tgt;
  message = sel;
}

Settings::~Settings( )
{ }

void Settings::create( )
{
  read_config( );
  m_change = false;

  FXVerticalFrame::create( );
}

/*************************************************************************************************/
void Settings::read_config( )
{
  FXint    cfg_id;
  FXString cfg_prefix;

  if ( getApp( )->reg( ).used( ) < 1 ) { getApp( )->reg( ).read( ); }

  cfg_prefix = CFG_UI_PREFIX;
  FXString icth_str = getApp( )->reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".IconsTheme", "Faenza" );
  cfg_id =  uicb_IconsTheme->findItem( icth_str );
  if( cfg_id >= 0 ) { uicb_IconsTheme->setCurrentItem( cfg_id ); } 

  cfg_prefix = CFG_SUDO_PREFIX;
  FXbool sudo_enable  = getApp( )->reg( ).readBoolEntry(   CFG_RUNNER, cfg_prefix + ".Enable", true );
  FXbool sudo_askpass = getApp( )->reg( ).readBoolEntry(   CFG_RUNNER, cfg_prefix + ".Askpass", false );
  such_enable->setCheck( sudo_enable );
  such_askpass->setCheck( sudo_askpass );

  cfg_prefix = CFG_TERM_PREFIX;
  FXString tenable_str = getApp( )->reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".Enable", "On request" );
  FXString tcmd     = getApp( )->reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".Command", "/usr/bin/xterm" );
  FXString texecprm = getApp( )->reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".arg_exec", "-e" );
  FXString tnoclprm = getApp( )->reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".arg_disclose", "+hold" );
  FXString tworkprm = getApp( )->reg( ).readStringEntry( CFG_RUNNER, cfg_prefix + ".arg_workdir", FXString::null );

  cfg_id =  tecb_enable->findItem( tenable_str );
  if( cfg_id >= 0 && !tcmd.empty( ) ) { 
    tecb_enable->setCurrentItem( cfg_id ); 
    tetf_command->setText( tcmd );
    tetf_execprm->setText( texecprm );
    tetf_disclosprm->setText( tnoclprm );
    tetf_workdirprm->setText( tworkprm );
  } 
  else { tecb_enable->setCurrentItem( 2 ); }
}

void Settings::write_config( )  
{
  FXint    cfg_id;
  FXString cfg_prefix;

  if( getApp( )->reg( ).existingSection( CFG_RUNNER ) ) { m_revert = getApp( )->reg( ).at( CFG_RUNNER ); }
 
  cfg_prefix = CFG_UI_PREFIX;
  cfg_id = uicb_IconsTheme->getCurrentItem( );  
  getApp( )->reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".IconsTheme", uicb_IconsTheme->getItem( cfg_id ).text( ) );

  cfg_prefix = CFG_SUDO_PREFIX;
  getApp( )->reg( ).writeBoolEntry(   CFG_RUNNER, cfg_prefix + ".Enable",  such_enable->getCheck( ) );
  getApp( )->reg( ).writeBoolEntry(   CFG_RUNNER, cfg_prefix + ".Askpass", such_askpass->getCheck( ) );

  cfg_prefix = CFG_TERM_PREFIX;
  cfg_id = tecb_enable->getCurrentItem( );
  getApp( )->reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".Enable",       tecb_enable->getItem( cfg_id ).text( ) );
  getApp( )->reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".Command",      tetf_command->getText( ).text( ) );
  getApp( )->reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".arg_exec",     tetf_execprm->getText( ).text( ) );
  getApp( )->reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".arg_disclose", tetf_disclosprm->getText( ).text( ) );
  getApp( )->reg( ).writeStringEntry( CFG_RUNNER, cfg_prefix + ".arg_workdir",  tetf_workdirprm->getText( ).text( ) );

  getApp( )->reg( ).write( );
}

/*************************************************************************************************/
long Settings::onCmd_Settings( FXObject *sender, FXSelector sel, void *data )
{

  FXuint id = FXSELID( sel );

  switch( id ) {
    case Settings::SETTINGS_SAVE :
    {
      write_config( ); 
      m_change = false;
      Notify( );

      break;
    }
    case Settings::SETTINGS_RESTORE :
    {
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
      break; 
    }
    case Settings::SETTINGS_DEFAULT :
    {
      if( FXMessageBox::question( this, MBOX_YES_NO, "Predefined FoxGHI settings", "Do you really want predefined settings?" ) == MBOX_CLICKED_YES ) {
        if( getApp( )->reg( ).existingSection( CFG_RUNNER ) ) { getApp( )->reg( ).deleteSection( CFG_RUNNER ); }
        read_config( );
        m_change = true;
        Notify( );
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

long Settings::onCmd_Update( FXObject *sender, FXSelector sel, void *data )
{
  
  m_change = true;
  return 0;
}

/**************************************************************************************************/
void Settings::MakeTitle( FXComposite *p, const FXString &text, FXIcon *ic )
{
  FXLabel *label = new FXLabel( p, text, ic, LABEL_NORMAL | LAYOUT_FILL_X  );
  label->setBackColor( getApp( )->getShadowColor( ) );
}

/**************************************************************************************************/
/* SETTINGS DIALOG */
FXIMPLEMENT( SettingsDialog, FXGDialogBox, NULL, 0 )

/**************************************************************************************************/
SettingsDialog::SettingsDialog( FXApp *a )
              : FXGDialogBox( a, "Configure", WINDOW_STATIC, 0, 0, 550, 450 )
{
  
  Application  *app = ( Application * ) this->getApp( );
  FXIconsTheme *icons = app->get_iconstheme( );

  setIcon( icons->get_icon( "settings" ) );

  Settings *config = new Settings( this, this, SettingsDialog::ID_RECONFIGURE );

  new FXStatusBar( this, FRAME_RAISED | LAYOUT_SIDE_BOTTOM | LAYOUT_BOTTOM | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0  );

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
