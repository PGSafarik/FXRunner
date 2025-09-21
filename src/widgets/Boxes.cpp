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
  m_button = new FXMenuButton( this, "\t\t Menu aplikace", ic, make_menu( ), BUTTON_TOOLBAR|ICON_ABOVE_TEXT|LAYOUT_FILL_Y );
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

  // Hlavni nabidka
  new FXMenuCommand( MyMenu, "Spustit", icons->get_icon( "run", "Menu" ), tgt, Runner::ID_ACCEPT );
  new FXMenuCommand( MyMenu, "Hystorie", icons->get_icon( "history", "Menu" ), tgt, Runner::HISTORY_SHOW );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
  new FXMenuCascade( MyMenu, "Moznosti spusteni", icons->get_icon( "properties", "Menu" ), ToolsMenu );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
  new FXMenuCommand( MyMenu, "Vybrat prikaz", icons->get_icon( "open", "Menu" ), tgt, Runner::OPEN_FILE );
  new FXMenuCommand( MyMenu, "Zmenit pracovni adresar", icons->get_icon( "directory", "Menu" ), tgt, Runner::OPEN_DIR );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
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

/*** OPTIONS MENU ****************************************************************************/
FXIMPLEMENT( FXOptionsBox, FXHeaderBox, NULL, 0 )

FXOptionsBox::FXOptionsBox( FXWindowHeader *p, FXIcon *ic ) : FXHeaderBox( p, nullptr, 0, LAYOUT_CENTER_Y | LAYOUT_RIGHT, 0, 0, 0 )
{
  m_button = new FXMenuButton( this, "\t\tMoznosti spusteni", ic, make_menu( ), BUTTON_TOOLBAR|ICON_ABOVE_TEXT|LAYOUT_FILL_Y );
}

void FXOptionsBox::create( )
{
  FXHeaderBox::create( );
  m_button->create( );
}

FXMenuPane* FXOptionsBox::make_menu( )
{
  FXObject     *tgt = getBoxFrame( )->getBoxTarget( );

  // Structura menu
  FXMenuPane *ToolsMenu = new FXMenuPane( this );

  // Nabidka volby
  new FXMenuCheck( ToolsMenu, "Superuzivatelska prava", tgt, Runner::ID_USER, 0 );
  new FXMenuCheck( ToolsMenu, "Neblokujici rezim", tgt, Runner::ID_ANNOUNCE, 0 );
  new FXMenuCheck( ToolsMenu, "Exit after run", tgt, Runner::ID_NOQUIT, 0 );
  new FXVerticalSeparator( ToolsMenu, SEPARATOR_GROOVE );
  new FXMenuCheck( ToolsMenu, "Spustit v terminalu", tgt, Runner::ID_TERMINAL, 0 );
  new FXMenuCheck( ToolsMenu, "Nezavirat terminal", tgt, Runner::ID_TERMLOCK, 0 );

  return ToolsMenu;
}

FXMenuCommand* FXOptionsBox::makeCommand( FXMenuPane *pane, const FXString &title, const FXString &icon, FXSelector sel )
{
   return new FXMenuCommand( pane, title, (( Application * ) this->getApp( ))->get_iconstheme( )->get_icon( icon, "Menu" ), getBoxFrame( )->getBoxTarget( ), sel );
}

/*** Tool bar *************************************************************************************/
FXIMPLEMENT( Toolbar, FXHeaderBox, NULL, 0 )

Toolbar::Toolbar( FXWindowHeader *p ) : FXHeaderBox( p, nullptr, 0, FRAME_SUNKEN | LAYOUT_LEFT, 0, 0, 0 )
{
  Application  *app = ( Application * ) this->getApp( );
  FXIconsTheme *icons = app->get_iconstheme( );
  FXObject     *tgt = getBoxFrame( )->getBoxTarget( );
  
  new FXButton( this, "\t\t Otevrit soubor", icons->get_icon( "open", "HeaderBar" ), tgt, Runner::OPEN_FILE, BUTTON_NORMAL | LAYOUT_RIGHT );
  new FXButton( this, "\t\t Zmenit pracovni adresar", icons->get_icon( "directory", "HeaderBar" ), tgt, Runner::OPEN_DIR, BUTTON_NORMAL | LAYOUT_RIGHT );
}

void Toolbar::create( )
{
  FXHeaderBox::create( );
}

/*** Run buttons **********************************************************************************/
FXDEFMAP( RunBox ) RB_MAP[ ] = {
  FXMAPFUNC( SEL_UPDATE, RunBox::ID_LIST, RunBox::onUpd_list ),
  FXMAPFUNC( SEL_COMMAND, RunBox::ID_LIST, RunBox::onCmd_select ),
};
FXIMPLEMENT( RunBox, FXHeaderBox, RB_MAP, ARRAYNUMBER( RB_MAP ) )

RunBox::RunBox( FXWindowHeader *p ) : FXHeaderBox( p, nullptr, 0, FRAME_SUNKEN | LAYOUT_LEFT, 0, 0, 0 )
{
   m_app = dynamic_cast< Application* >( this->getApp( ) );
   FXIconsTheme *icons = m_app->get_iconstheme( );
   FXObject     *tgt = getBoxFrame( )->getBoxTarget( );

   m_popup = new FXPopup( this, POPUP_VERTICAL|FRAME_RAISED|FRAME_THICK, 0, 0, 450, 232  );
   m_list  = new FXList( m_popup, this, ID_LIST, LIST_NORMAL | LAYOUT_FILL );

   m_action_btn = new FXButton( this, "\t\t Spustit", icons->get_icon( "run", "HeaderBar" ), tgt, Runner::ID_ACCEPT, BUTTON_NORMAL | LAYOUT_LEFT );
   m_menu_btn   = new FXMenuButton( this, FXString::null, icons->get_icon( "popup", 16 ), m_popup, FRAME_RAISED | FRAME_THICK | JUSTIFY_NORMAL | ICON_BEFORE_TEXT | MENUBUTTON_DOWN | LAYOUT_FILL_Y, 18 );
}

void RunBox::create( )
{
  FXHeaderBox::create( );

  onUpd_list( this, ID_UPDATE, NULL );
}

long RunBox::onUpd_list( FXObject *sender, FXSelector sel, void *data )
{
  History *hist = m_app->get_History( );
  FXint    number = hist->no( );
  FXint    count = m_app->HistoryQuickSize( );
  Task    *t = nullptr;

  if( number > 0 ) {
    m_list->clearItems( );
    for( int i = 0; i < count; i++ ) {
      if( i < number ) {
        t = hist->at( i );
        if( t ) { m_list->insertItem( i, t->cmd ); }
      }
      else { break; }
    }
  }

  return 0;
}

long RunBox::onCmd_select( FXObject *sender, FXSelector sel, void *data )
{
  FXival pos = (FXival) data;
  Task *t = m_app->get_History( )->at( (FXint ) pos );
  if( t ) {
    //std::cout << pos << ". " << t->cmd << std::endl;
    m_app->get_History( )->current( ( FXint )pos );
    m_menu_btn->handle( this, FXSEL( SEL_COMMAND, ID_UNPOST ), nullptr );
    return 1;
  }

  return 0;
}

/*** END ******************************************************************************************/
