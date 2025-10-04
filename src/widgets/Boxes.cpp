/*************************************************************************
* Boxes.cpp Copyright (c) 2017 - 2022 by  D.A.Tiger                     *
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
#include "widgets/Boxes.h"

/*** APPLICATION MENU ****************************************************************************/
FXIMPLEMENT( FXMenuBox, FXHeaderBox, NULL, 0 )

FXMenuBox::FXMenuBox( FXWindowHeader *p, FXIcon *ic ) : FXHeaderBox( p, nullptr, 0, LAYOUT_CENTER_Y, 0, 0, 0 )
{
  m_button = new FXMenuButton( this, "\t\t Menu aplikace", ic, make_menu( ), BUTTON_TOOLBAR | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );
}

void FXMenuBox::create( )
{
  FXHeaderBox::create( );
  m_button->create( );
}

FXMenuPane* FXMenuBox::make_menu( )
{
  Application  *app = ( Application * ) this->getApp( );
  FXIconsTheme *icons = app->get_iconstheme( );
  FXObject     *tgt = getBoxFrame( )->getBoxTarget( );

  // Structura menu
  FXMenuPane *MyMenu    = new FXMenuPane( this );
  FXMenuPane *ToolsMenu = new FXMenuPane( this );
  FXMenuPane *HelpMenu  = new FXMenuPane( this );

  // Nabidka volby
  new FXMenuCheck( ToolsMenu, "Superuzivatelska prava", tgt, Runner::ID_USER, 0 );
  new FXMenuCheck( ToolsMenu, "Neblokujici rezim", tgt, Runner::ID_ANNOUNCE, 0 );
  new FXMenuCheck( ToolsMenu, "Exit after run", tgt, Runner::ID_NOQUIT, 0 );
  new FXVerticalSeparator( ToolsMenu, SEPARATOR_GROOVE );
  new FXMenuCheck( ToolsMenu, "Spustit v terminalu", tgt, Runner::ID_TERMINAL, 0 );
  new FXMenuCheck( ToolsMenu, "Nezavirat terminal", tgt, Runner::ID_TERMLOCK, 0 );

  // Roleta napovedy
  new FXMenuCommand( HelpMenu, "Manual", icons->get_icon( "help", "Menu" ), tgt, Runner::OPEN_HELP );
  new FXVerticalSeparator( HelpMenu, SEPARATOR_GROOVE );
  new FXMenuCommand( HelpMenu, "O aplikaci", icons->get_icon( "about", "Menu" ), tgt, Runner::OPEN_HELP );

  new FXMenuCommand( MyMenu, "Nastaveni", icons->get_icon( "settings", "Menu" ), tgt, Runner::OPEN_OPTIONS );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
  new FXMenuCascade( MyMenu, "Napoveda", icons->get_icon( "help", "Menu" ), HelpMenu );
  new FXMenuCommand( MyMenu, "Zavrit", icons->get_icon( "close", "Menu" ), tgt, Runner::ID_CANCEL );

  return MyMenu;
}

FXMenuCommand* FXMenuBox::makeCommand( FXMenuPane *pane, const FXString &title, const FXString &icon, FXSelector sel )
{
   return new FXMenuCommand( pane, title, (( Application * ) this->getApp( ))->get_iconstheme( )->get_icon( icon, "Menu" ), getBoxFrame( )->getBoxTarget( ), sel );
}

/*** Run buttons **********************************************************************************/
FXDEFMAP( RunBox ) RB_MAP[ ] = {
  FXMAPFUNCS( SEL_COMMAND, RunBox::ID_SELECT, RunBox::ID_WORKDIR, RunBox::OnCmd_Open ),
  FXMAPFUNC( SEL_OPENED, RunBox::ID_NOTIFY, RunBox::on_Notify ),
  FXMAPFUNC( SEL_CLOSED, RunBox::ID_NOTIFY, RunBox::on_Notify ),
};
FXIMPLEMENT( RunBox, FXHeaderBox, RB_MAP, ARRAYNUMBER( RB_MAP ) )

RunBox::RunBox( FXWindowHeader *p ) : FXHeaderBox( p, nullptr, 0, FRAME_SUNKEN | LAYOUT_LEFT, 0, 0, 0 )
{
  m_app = dynamic_cast< Application* >( this->getApp( ) );
  FXIconsTheme *icons = m_app->get_iconstheme( );
  FXObject     *tgt = getBoxFrame( )->getBoxTarget( );

  m_popup = new FXPopup( this, POPUP_VERTICAL | POPUP_SHRINKWRAP | FRAME_RAISED | FRAME_THICK );
  m_popup->setBaseColor( getApp( )->getBackColor( ) );
  FXHorizontalFrame *buttons_fr = new FXHorizontalFrame( m_popup, FRAME_NONE | LAYOUT_FILL_X | PACK_UNIFORM_WIDTH );
  new FXButton( buttons_fr, "Find command", icons->get_icon( "open",    16 ), this, ID_SELECT, BUTTON_NORMAL | LAYOUT_FILL );
  new FXButton( buttons_fr, "Show History", icons->get_icon( "history", 16 ), this, ID_HISTORY, BUTTON_NORMAL | LAYOUT_FILL  );

  m_modes = new RunModes( m_popup, this, ID_NOTIFY );

  m_action_btn = new FXButton( this, "\t\t Spustit", icons->get_icon( "run", "HeaderBar" ), tgt, Runner::ID_ACCEPT, BUTTON_NORMAL | LAYOUT_LEFT );
  m_menu_btn   = new FXMenuButton( this, FXString::null, icons->get_icon( "popup", 16 ), m_popup, FRAME_RAISED | FRAME_THICK | JUSTIFY_NORMAL | ICON_BEFORE_TEXT | MENUBUTTON_DOWN | LAYOUT_FILL_Y, 18 );
}

void RunBox::create( )
{
  FXHeaderBox::create( );

}

long RunBox::OnCmd_Open( FXObject *tgt, FXSelector sel, void *data )
{
  FXlong result = 0;

  m_menu_btn->handle( this, FXSEL( SEL_COMMAND, FXMenuButton::ID_UNPOST ), nullptr );
  FXObject     *box_tgt = getBoxFrame( )->getBoxTarget( );
  switch( FXSELID( sel )  ) {
    case ID_SELECT :
    {
      result = box_tgt->handle( this, FXSEL( SEL_COMMAND, Runner::OPEN_FILE ), nullptr );
      break;
    }
    case ID_HISTORY :
    {
      result = box_tgt->handle( this, FXSEL( SEL_COMMAND, Runner::HISTORY_SHOW ), nullptr );
      break;
    }
  }

  return result;
}

long RunBox::on_Notify( FXObject *tgt, FXSelector sel, void *data )
{
  long result = 0;

  switch( FXSELTYPE( sel ) ) {
    case SEL_OPENED : {
      result = m_menu_btn->handle( this, FXSEL( SEL_COMMAND, FXMenuButton::ID_UNPOST ), nullptr );
      break;
    }
    case SEL_CLOSED : {
      result = m_menu_btn->handle( this, FXSEL( SEL_COMMAND, FXMenuButton::ID_POST ), nullptr );
      break;
    }
  }

  return result;
}
/*** END ******************************************************************************************/
