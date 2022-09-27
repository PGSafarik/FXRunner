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

#ifndef __BOXES_H
#define __BOXES_H
/*************************************************************************
* Boxes.cpp                                                              *
* EXPERIMENTAL                                                           *
* Male prvky (boxiky, widgety) do Toolbaru aplikace                      *
* Copyright (c) 01/11/2017 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<FXBoxFrame.h>
#include<Application.h>
#include<Runner.h>

/*** Toolbar widget *****************************************************/
class FXBaseBox : public FXVerticalFrame {
FXDECLARE( FXBaseBox )
  FXWindowHeader *b_frame;  // Ramec pro boxiky
  FXbool      b_separe; // Separovat ( ramec vlozi separator za boxik)
  FXbool      b_nohide; // Neskryvat

public:
  FXBaseBox( FXWindowHeader *bf, FXuint opts = LAYOUT_CENTER_Y );
  virtual ~FXBaseBox( );

  // Operations
  virtual void create( );

  // Access Methods
  FXWindowHeader* getBoxFrame( ) { return b_frame;  }
  FXbool getNohide( )            { return b_nohide; }
  void setNohide( FXbool value ) { b_nohide = value; }
  FXbool getSepare( )            { return b_separe; }
  void setSepare( FXbool value ) { b_separe = value; }

protected:
  FXBaseBox( ) { }
  

};

/*** Menu aplikace ******************************************************/
class FXMenuBox : public FXBaseBox {
FXDECLARE( FXMenuBox )
  FXMenuButton *m_button;
  
public :
  FXMenuBox( FXWindowHeader *p, FXIcon *ic );
  virtual ~FXMenuBox( ) { }

  // Operations
  virtual void create( );

protected :
  FXMenuBox( ) { }
  FXMenuPane* make_menu( );
  FXMenuCommand* makeCommand( FXMenuPane *pane, const FXString &Title, const FXString &icon, FXSelector sel );
};

/*** Titulek aplikace ***************************************************
class FXTitleBox : public FXBaseBox {
FXDECLARE( FXTitleBox )
   FXLabel *m_label;      // Prostor pro text titulku

   FXString m_title;      // Bufffer Text hlavniho titlku.
   FXbool   m_handled;    // Indikator zachyceni okna mysi

public:
  FXTitleBox( FXComposite *p, const FXString &title );
  virtual ~FXTitleBox( ) { }

  // Operations
  virtual void create( );

  // Access
  void setSubtext( const FXString &text );
  //FXString getSubtext( ) const;

protected:
  FXTitleBox( ) { }

};*/

#endif /*__BOXES_H */

/*** END ****************************************************************/
