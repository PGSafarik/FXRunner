/*************************************************************************
* FXBoxFrame.cpp Copyright (c) 2017 - 2022 by  D.A.Tiger                     *
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
#include<FXBoxFrame.h>

FXIMPLEMENT( FXBoxFrame, FXHorizontalFrame, NULL, 0 )

/*************************************************************************************************/
FXBoxFrame::FXBoxFrame( FXComposite *p, FXObject *d_target, FXuint opts )
          : FXHorizontalFrame( p, FRAME_NONE | PACK_UNIFORM_HEIGHT | LAYOUT_FILL_X, 0, 0, 0, 0,  0, 0, 0, 0,  2, 0 )
{
  f_delegator = new FXDelegator;
  f_delegator->setDelegate( d_target );

  f_frameOpts = opts;
  f_pl = f_pr = 0;
  f_hs        = DEFAULT_SPACING;
  f_boxsize   = 22;
  f_menusize  = 16;
}

FXBoxFrame::~FXBoxFrame( )
{
  set_Delegate( NULL );
  delete f_delegator;
}

/*************************************************************************************************/
void FXBoxFrame::create( )
{
  FXHorizontalFrame::create( );
}

void FXBoxFrame::Load( const FXString &list_file )
{
  /// FIXME: BOXES_01
}

void FXBoxFrame::Config( FXComposite *p )
{
  /// FIXME: BOXES_02
}

/*** END ******************************************************************************************/
