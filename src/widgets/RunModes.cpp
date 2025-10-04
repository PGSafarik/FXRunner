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
#include "widgets/RunModes.h"

/**************************************************************************************************/
FXDEFMAP( RunModes ) PMAP[ ] = {
  FXMAPFUNCS( SEL_COMMAND, RunModes::MODE_CHANGE, RunModes::MODE_RESET, RunModes::onCmd_Mode ),
  FXMAPFUNC(  SEL_COMMAND, RunModes::ID_WORKDIR, RunModes::onCmd_Workdir ),
};
FXIMPLEMENT( RunModes, FXVerticalFrame, PMAP, ARRAYNUMBER( PMAP ) )

/**************************************************************************************************/
RunModes::RunModes( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts )
              : FXVerticalFrame( p, opts )
{
  m_app = dynamic_cast<Application*>( getApp( ) );
  m_target = tgt;
  m_message = sel;

  FXIconsTheme *icons = m_app->get_iconstheme( );
  FXint ic_size = 22;

  FXHorizontalFrame *path_fr = new FXHorizontalFrame( this, FRAME_NONE | LAYOUT_FILL_X );
  m_dir_text = new FXTextField( path_fr, 50, this, ID_WORKDIR, TEXTFIELD_NORMAL | LAYOUT_FILL );
  new FXButton( path_fr, "\t\t Select workdir",   icons->get_icon( "directory", 16 ), this, ID_WORKDIR, BUTTON_NORMAL );
  new FXHorizontalSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );

  m_su_check = new FXCheckButton( this, "Root mode", this, MODE_CHANGE );
  m_nblock_check = new FXCheckButton( this, "Blocked mode", this, MODE_CHANGE );
  m_rexit_check = new FXCheckButton( this, "Do not terminate after running the command", this, MODE_CHANGE );
  new FXHorizontalSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );

  m_rterm_check = new FXCheckButton( this, "Run command with terminal", this, MODE_CHANGE );
  m_ntexit_check = new FXCheckButton( this, "Not closed terminal with  ", this, MODE_CHANGE );
  new FXHorizontalSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );

  new FXButton( this, "Set property", nullptr, nullptr, 0, BUTTON_NORMAL | LAYOUT_FILL_X );

  Reset( );
}

RunModes::~RunModes( )
{

}

void RunModes::create( )
{
  FXVerticalFrame::create( );
}

/**************************************************************************************************/
long RunModes::onCmd_Workdir( FXObject *tgt, FXSelector sel, void *data )
{
  Notify( SEL_OPENED ); // The directory dialog opened -> parent must close menu
  FXDirDialog *dir = new FXDirDialog ( this, "Select workdir:" );
  if( dir->execute( ) ) {
    FXString new_dir = dir->getDirectory( );
    if( m_dir_text->getText( ) != new_dir ) {
      m_dir_text->setText( new_dir );
      m_change = true;
    }
  }
  Notify( SEL_CLOSED ); // The directory dialog closed -> parent must open menu

  return 0;
}

long RunModes::onCmd_Mode(FXObject *tgt, FXSelector sel, void *data)
{
  long result = 1;
  FXuint msg = FXSELID( sel );

  switch( msg ) {
    case MODE_CHANGE :
    {
      m_change = true;
      result = 0;
      break;
    }
    case MODE_UPDATE :
    {
      Task *task = static_cast<Task*>( data );
      if( task ) {
        if( task->wpth.empty( ) ) { task->wpth = FXSystem::getHomeDirectory( ); }
        if( m_dir_text->getText( ) != task->wpth ) { m_dir_text->setText( task->wpth ); }
        m_su_check->setCheck( task->prop->suaccess );
        m_nblock_check->setCheck( task->prop->unblock );
        m_rterm_check->setCheck( task->prop->term );
        m_ntexit_check->setCheck( task->prop->nocloseterm );

        m_change = false;
      break;
    }
  }

  return result;
}

/**************************************************************************************************/
void RunModes::Reset()
{
  m_dir_text->setText( FXSystem::getHomeDirectory( ) );
  m_su_check->setCheck( false );
  m_nblock_check->setCheck( false );
  m_rexit_check->setCheck( false );
  m_rterm_check->setCheck( false );
  m_ntexit_check->setCheck( false );

  m_change = false;
}

long RunModes::Notify( FXuint _type, void *_data ) {
  long result = 1;
  if( m_target ) { result = m_target->handle( this, FXSEL( _type, m_message ), _data ); }

  return result;
}

/*** END ******************************************************************************************/