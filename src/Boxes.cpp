// Boxes.cpp; Copyright (c) 01/11/2017 D.A.Tiger; GNU GPL 3
#include<Boxes.h>

/*** Base of boxes *******************************************************************************/
FXIMPLEMENT( FXBaseBox, FXVerticalFrame, NULL, 0 )

FXBaseBox::FXBaseBox( FXComposite *bf, FXuint opts )
         : FXVerticalFrame( bf, FRAME_NONE | opts, 0, 0, 0, 0,  0, 0, 0, 0,  HV_SPACING2, HV_SPACING2 )
{
  b_frame  = bf;
  b_separe = false;
  b_nohide = false;
}

FXBaseBox::~FXBaseBox( )
{ }

void FXBaseBox::create( )
{
  FXVerticalFrame::create( );
}


/*** APPLICATION MENU ****************************************************************************/
FXIMPLEMENT( FXMenuBox, FXBaseBox, NULL, 0 )

FXMenuBox::FXMenuBox( FXComposite *p, FXIcon *ic, FXObject *tgt ) : FXBaseBox( p )
{
  m_tgt = tgt;
  m_button = new FXMenuButton( this, "\t\t Menu aplikace", ic, make_menu( ), BUTTON_TOOLBAR|ICON_ABOVE_TEXT|LAYOUT_FILL_Y );
}

void FXMenuBox::create( )
{
  FXBaseBox::create( );
  m_button->create( );
}

FXMenuPane* FXMenuBox::make_menu( )
{
  Application *app = ( Application * ) this->getApp( );
  //FXObject    *tgt = getBoxFrame( )->get_Delegate( );

  // Structura menu
  FXMenuPane *MyMenu    = new FXMenuPane( this );
  FXMenuPane *ToolsMenu = new FXMenuPane( this );
  FXMenuPane *HelpMenu  = new FXMenuPane( this );

  // Nabidka volby
  new FXMenuCheck( ToolsMenu, "Superuzivatelska prava", m_tgt, Runner::ID_USER, 0 );
  new FXMenuCheck( ToolsMenu, "Neblokujici rezim", m_tgt, Runner::ID_ANNOUNCE, 0 );
  new FXMenuCheck( ToolsMenu, "Nezavirat Launcher", m_tgt, Runner::ID_NOQUIT, 0 );
  new FXVerticalSeparator( ToolsMenu, SEPARATOR_GROOVE );
  new FXMenuCheck( ToolsMenu, "Spustit v terminalu", m_tgt, Runner::ID_TERMINAL, 0 );
  new FXMenuCheck( ToolsMenu, "Nezavirat terminal", m_tgt, Runner::ID_TERMLOCK, 0 );

  // Roleta napovedy
  new FXMenuCommand( HelpMenu, "Manual", app->icon_copy( "help-contents.png" ), m_tgt, Runner::ID_HELP );
  new FXVerticalSeparator( HelpMenu, SEPARATOR_GROOVE );
  new FXMenuCommand( HelpMenu, "O aplikaci", app->icon_copy( "documentation.png" ), m_tgt, Runner::ID_HELP );

  // Hlavni nabidka
  new FXMenuCommand( MyMenu, "Spustit", app->icon_copy( "dialog-ok-apply.png" ), m_tgt, Runner::ID_ACCEPT );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
  new FXMenuCommand( MyMenu, "Vybrat prikaz", app->icon_copy( "run-build-file.png" ), m_tgt, Runner::ID_OPEN_FILE );
  new FXMenuCommand( MyMenu, "Zmenit pracovni adresar", app->icon_copy( "document-open-folder.png" ), m_tgt, Runner::ID_OPEN_DIR );
  new FXMenuCascade( MyMenu, "Volby spusteni", app->icon_copy( "view-task.png" ), ToolsMenu );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
  new FXMenuCommand( MyMenu, "Nastaveni", app->icon_copy( "configure.png" ), m_tgt, Runner::ID_OPTIONS );
  new FXVerticalSeparator( MyMenu, SEPARATOR_GROOVE );
  new FXMenuCascade( MyMenu, "Napoveda", app->icon_copy( "documentinfo.png" ), HelpMenu );
  new FXMenuCommand( MyMenu, "Zavrit", app->icon_copy( "dialog-close.png" ), m_tgt, Runner::ID_CANCEL );

  return MyMenu;
}

FXMenuCommand* FXMenuBox::makeCommand( FXMenuPane *pane, const FXString &title, const FXString &icon, FXSelector sel )
{
   return new FXMenuCommand( pane, title, (( Application * ) this->getApp( ))->icon_copy( icon ), m_tgt /*getBoxFrame( )->get_Delegate( )*/, sel );
}

/*** FRAME TITLE ****************************************************************************
FXIMPLEMENT( FXTitleBox, FXBaseBox, NULL, 0 )

FXTitleBox::FXTitleBox( FXBoxFrame *p, const FXString &title ) : FXBaseBox( p )
{
  m_title = title;
  m_label = new FXLabel( this, m_title, NULL, LABEL_NORMAL | LAYOUT_FILL_Y | LAYOUT_CENTER_X | LAYOUT_CENTER_Y );
}

void FXTitleBox::create( )
{
  FXBaseBox::create( );
  m_label->create( );
}

void FXTitleBox::setSubtext( const FXString &text )
{
  if( !text.empty( ) ) { m_label->setText( m_title + "\n" + text ); }
  else { m_label->setText( m_title ); }
}*/

/*** END ******************************************************************************************/
