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
#include "windows/SettingsDialog.h"

FXDEFMAP( SettingsDialog ) SettingsBoxMap[ ] = {
  FXMAPFUNC( SEL_CLOSE,   0,                         SettingsDialog::onCmdCancel ),
  FXMAPFUNC( SEL_COMMAND, SettingsDialog::ID_CLOSE,  SettingsDialog::onCmdCancel ),
  FXMAPFUNC( SEL_CHORE,   SettingsDialog::ID_CANCEL, SettingsDialog::onCmdCancel ),
  FXMAPFUNC( SEL_TIMEOUT, SettingsDialog::ID_CANCEL, SettingsDialog::onCmdCancel ),
  FXMAPFUNC( SEL_COMMAND, SettingsDialog::ID_CANCEL, SettingsDialog::onCmdCancel ),
};
FXIMPLEMENT( SettingsDialog, FXSecondaryWindow, SettingsBoxMap, ARRAYNUMBER( SettingsBoxMap ) )

/**************************************************************************************************/
SettingsDialog::SettingsDialog( FXWindow *own )
              : FXSecondaryWindow( own, "Fox Runner", WINDOW_STATIC, 0, 0, 800, 350 )
{
  setWMDecorations( WM_DECOR_SECONDARY );

  Application  *app = ( Application * ) this->getApp( );
  FXIconsTheme *icons = app->get_iconstheme( );
  setIcon( icons->get_icon( "settings" ) );
  getHeader( )->setText( "Settings" );

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
  FXSecondaryWindow::create( );
  show( PLACEMENT_SCREEN );
}

long SettingsDialog::onCmdCancel( FXObject *sender, FXSelector sel, void *data )
{
   Application *app = static_cast<Application*>( getApp( ) );

   if( app && app->is_changed( ) ) {
     FXWindow *win = getOwner( );
     if( win ) {
       win->handle( this, FXSEL( SEL_CHANGED, Runner::ID_RECONFIGURE), NULL );
     }
   }

   return FXSecondaryWindow::onCmdCancel( sender, sel, data );
}

/*** END ******************************************************************************************/