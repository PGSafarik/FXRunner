// FXBoxFrame.cpp; Copyright (c) 01/11/2017;  D.A.Tiger; GNU GPL 3
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
