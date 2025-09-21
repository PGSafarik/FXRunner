//
// Created by gabriel on 23.08.25.
//

#include "windows/HistoryView.h"

FXDEFMAP( HistoryView ) HW_MAP[ ] = {
  FXMAPFUNC(  SEL_UPDATE,  HistoryView::ID_REFRESH, HistoryView::on_Window ),
  FXMAPFUNC(  SEL_UPDATE,  HistoryView::ID_HELP,    HistoryView::on_Window ),
  FXMAPFUNCS( SEL_COMMAND, HistoryView::ID_SELECT,  HistoryView::ID_TOP, HistoryView::on_List ),
};
FXIMPLEMENT( HistoryView, FXSecondaryWindow, HW_MAP, ARRAYNUMBER( HW_MAP ) )

/**************************************************************************************************/
HistoryView::HistoryView( FXTopWindow *w )
           : FXSecondaryWindow( w, "Fox Runner", WINDOW_STATIC, 0, 0, 770, 180 )
{
  m_parent = w;
  m_app = dynamic_cast<Application*>( getApp( ) );
  FXIconsTheme *icons = m_app->get_iconstheme( );
  this->getHeader(  )->setText( "History view" );

  FXHorizontalFrame *content_frame = new FXHorizontalFrame( this, FRAME_NONE | LAYOUT_FILL );
  m_view = new FXIconList( content_frame, nullptr, 0, ICONLIST_EXTENDEDSELECT | ICONLIST_DETAILED | LAYOUT_FILL );
  m_view->setHeaders( "Command\nWork path\nBackground mode\nSuper user mode\nTerminal", 120 );
  m_view->setHeaderSize( 0, 250 );
  m_view->setHeaderSize( 1, 200 );
  m_view->setHeaderSize( 4, 60 );

  FXWindowHeader *w_header = getHeader( );
  m_up = new FXButton(  w_header, "\t\t Move top", icons->get_icon( "history_top", "HeaderBar" ), this, ID_TOP, BUTTON_TOOLBAR );
  new FXVerticalSeparator( w_header );
  m_remove = new FXButton( w_header, "\t\t Remove", icons->get_icon( "history_delete", "HeaderBar" ), this, ID_REMOVE, BUTTON_TOOLBAR );
  m_clear = new FXButton( w_header, "\t\t Clear",  icons->get_icon( "history_clear", "HeaderBar" ), this, ID_CLEAR, BUTTON_TOOLBAR );
}

HistoryView::~HistoryView( )
{
  m_parent->handle( this, FXSEL( SEL_COMMAND, ID_SHOW ), nullptr );
}

/**************************************************************************************************/
void HistoryView::create( )
{
  m_parent->handle( this, FXSEL( SEL_COMMAND, ID_HIDE ), nullptr );
  FXSecondaryWindow::create( );
}

FXuint HistoryView::execute( FXuint placement )
{
  UpdateList( );
  return FXSecondaryWindow::execute( placement );
}

/**************************************************************************************************/
long HistoryView::on_List( FXObject *sender, FXSelector selector, void *ptr )
{
  long         resh  = 0;
  FXuint       _msg  = FXSELID( selector );
  History     *hist  = m_app->get_History( );
  FXint        index = m_view->getCurrentItem( );

  switch( _msg ) {
    case ID_TOP: {
      if( index >= 0 && hist->current( index, false ) ) { UpdateList( ); }
      break;
    }

    case ID_REMOVE:
    {
      if( index >= 0 ) {
        FXuint answer = FXMessageBox::question( this, MBOX_YES_NO, "History view", "Are you sure you want to delete the selected items?" );
        if( answer == MBOX_CLICKED_YES ) {
          if( hist->remove( index, true ) ) { UpdateList( ); }
        }
      }
      break;
    }

    case ID_CLEAR: {
      FXuint answer = FXMessageBox::question( this, MBOX_YES_NO, "History view", "Are you sure you want to delete all command history?" );
      if( answer == MBOX_CLICKED_YES ) {
        hist->clear( );
        UpdateList( );
      }
    }
  }

   return resh;
}

long HistoryView::on_Window( FXObject *sender, FXSelector selector, void *ptr )
{

}

/**************************************************************************************************/
FXString HistoryView::TaskToString( Task *task )
{
  FXString str = FXString::null;

  if( task ) {
    str = task->cmd + "\t";
    str += task->wpth + "\t";
    str += ( task->prop->unblock ? "true" : "false" );  str += "\t";
    str += ( task->prop->suaccess ? "true" : "false" ); str += "\t";
    str += ( task->prop->term ? "true" : "false" );     str += "\t";
  }

  return str;
}

Task* HistoryView::TaskFromString( const FXString &str, Task *task )
{
  Task *t = nullptr;
  FXString segment;

  if( !str.empty( ) ) {
    t = ( task ? task : new Task );
    for( FXint pos = 0; pos != 5; pos++ ) {
      segment = str.section( "\t", 0 );
      switch( pos ) {
        case 0 : t->cmd            = segment; break;
        case 1 : t->wpth           = segment; break;
        case 2 : t->prop->unblock  = ( segment == "true" ); break;
        case 3 : t->prop->suaccess = ( segment == "true" ); break;
        case 4 : t->prop->term     = ( segment == "true" ); break;
      }
    }
  }

  return t;
}

void HistoryView::UpdateList( )
{
  History     *hist = nullptr;

  if( ( hist = m_app->get_History( ) ) != nullptr ) {
    //std::cout << "LOAD HISTORY TO DIALOG" << std::endl;
    FXint num = static_cast<FXint>( hist->no( ) );

    m_view->clearItems( );

    for( FXint i = 0; i != num; i++ ) {
      FXString record = TaskToString( hist->at( i ) );
      m_view->appendItem( record );
    }
  }
}


/*** END ******************************************************************************************/