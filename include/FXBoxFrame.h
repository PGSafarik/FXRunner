/*************************************************************************
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

#ifndef __BOXFRAME_H
#define __BOXFRAME_H
/**************************************************************************************************
* FXBoxFrame                                                                                      *
* EXPERIMENTAL                                                                                    *
* Compositor pro male boxiky, pouzivany nejcasteji jako toolbar v toplevel oknech. Obdoba menu v  *
* Gnome3 applikacich.                                                                             *
* Copyright (c) 01/11/2017 D.A.Tiger <drakarax@seznam.cz>                                         *
**************************************************************************************************/
#include<main.h>

class FXBoxFrame : public FXHorizontalFrame {
FXDECLARE( FXBoxFrame )
  FXDelegator *f_delegator;	// delgator udalosti z boxu

  //( Odstupy jednotlivych boxu jsou dany konstruktorem. Defaultne hs = DEFAULT_SPACING )
  FXuint f_frameOpts;		// Priznaky stylovani boxu ( Default: FRAME_NONE )
  FXint  f_pl, f_pr, f_hs;	// Hodnoty odstupu widgetu v boxech ( Default: f_pl = f_pr = 0, f_hs = DEFAULT_SPACING)
  FXint  f_boxsize;			// Velikost boxu (typicky ikon v boxech. Default = 22 )
  FXint  f_menusize;		// Velikost vyskakovaciho menu (typicky ikon v menu. Default = 16 )

public :
  FXBoxFrame( FXComposite *p, FXObject *d_target = NULL, FXuint opts = FRAME_NONE );
  virtual ~FXBoxFrame( );

  // Access
  FXint get_pl( )									{ return f_pl; }
  void  set_pl( FXint value = 0 )					{ f_pl = value; }
  FXint get_pr( )									{ return f_pr; }
  void  set_pr( FXint value = 0 )					{ f_pr = value; }
  FXint get_hs( )									{ return f_hs; }
  void  set_hs( FXint value = 0 )					{ f_hs = value; }
  FXObject* get_Delegate( )						    { return f_delegator->getDelegate( ); }
  void  set_Delegate( FXObject *obj )				{ f_delegator->setDelegate( obj ); }
  FXuint get_FrameStyle( )							{ return f_frameOpts; }
  void  set_FrameStyle( FXuint opts = FRAME_NONE )  { f_frameOpts = opts; }

  // Operations
  virtual void create( );
  virtual void Load( const FXString &list_file );
  virtual void Config( FXComposite *p );

protected:
  FXBoxFrame( ) { }

};


#endif /* __BOXFRAME */
/*** END ****************************************************************/
