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
  FXMAPFUNC( SEL_COMMAND, RunModes::ID_WORKDIR, RunModes::onCmd_Workdir ),
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
  m_dir_text->setText( FXSystem::getHomeDirectory( ) );
  new FXHorizontalSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );

  m_su_check = new FXCheckButton( this, "Root mode", NULL, 0 );
  m_nblock_check = new FXCheckButton( this, "Blocked mode", NULL, 0 );
  m_rexit_check = new FXCheckButton( this, "Do not terminate after running the command", NULL, 0 );
  new FXHorizontalSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );

  m_rterm_check = new FXCheckButton( this, "Run command with terminal", NULL, 0 );
  m_ntexit_check = new FXCheckButton( this, "Not closed terminal with  ", NULL, 0 );
  new FXHorizontalSeparator( this, SEPARATOR_GROOVE | LAYOUT_FILL_X );

  new FXButton( this, "Set property", nullptr, nullptr, 0, BUTTON_NORMAL | LAYOUT_FILL_X );
}

RunModes::~RunModes( )
{

}

void RunModes::create( )
{
  FXVerticalFrame::create( );
}

/**************************************************************************************************/
long RunModes::onCmd_Workdir( FXObject *tgt, FXSelector sel, void *ptr )
{
  Notify( SEL_OPENED ); // The directory dialog opened -> parent must close menu
  FXDirDialog *dir = new FXDirDialog ( this, "Select workdir:" );
  if( dir->execute( ) ) { m_dir_text->setText( dir->getDirectory( ) ); }
  Notify( SEL_CLOSED ); // The directory dialog closed -> parent must open menu

  return 0;
}

/**************************************************************************************************/
long RunModes::Notify( FXuint _type, void *_data ) {
  long result = 1;
  if( m_target ) { result = m_target->handle( this, FXSEL( _type, m_message ), _data ); }

  return result;
}

/*** END ******************************************************************************************/