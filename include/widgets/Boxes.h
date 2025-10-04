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
#ifndef FXRUNNER_BOXES_H
#define FXRUNNER_BOXES_H

/*************************************************************************
* Boxes.cpp                                                              *
* EXPERIMENTAL                                                           *
* Male prvky (boxiky, widgety) do Header baru aplikace                   *
* Copyright (c) 01/11/2017 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "core.h"
#include "windows.h"
#include "RunModes.h"

/*** Menu aplikace ********************************************************************************/
class FXMenuBox : public FXHeaderBox {
FXDECLARE( FXMenuBox )
  FXMenuButton *m_button;
  
public :
  FXMenuBox( FXWindowHeader *p, FXIcon *ic );
  virtual ~FXMenuBox( ) { }

  /* Operations */
  virtual void create( );

  /* Access methods */

protected :
  FXMenuBox( ) { }
  
  /* Helpers methods */
  FXMenuPane* make_menu( );
  FXMenuCommand* makeCommand( FXMenuPane *pane, const FXString &Title, const FXString &icon, FXSelector sel );
};

/*** Run Button with menu *************************************************************************/
class RunBox : public FXHeaderBox {
FXDECLARE( RunBox )
  Application  *m_app;
  FXButton     *m_action_btn;
  FXMenuButton *m_menu_btn;
  FXPopup      *m_popup;
  RunModes     *m_modes;
  FXTextField  *m_dir_text;

public:
  explicit RunBox( FXWindowHeader *p );
  virtual ~RunBox( ) { }

  /* Access methods */
  RunModes* get_Modes( ) { return m_modes; }

  /* Operations methods */
  void create( ) override;

  /* Messages & Handlers */
  enum {
    ID_NOTIFY = FXHeaderBox::ID_LAST, //
    ID_SELECT,                      // Open the dialog for select command
    ID_HISTORY,                     // Switch to the history window
    ID_WORKDIR,                     //

    ID_LAST
  };
  long OnCmd_Open( FXObject *tgt, FXSelector sel, void *data );
  long on_Notify( FXObject *tgt, FXSelector sel, void *data );

protected:
  RunBox( ) { }

};

#endif /* FXRUNNER_BOXES_H */

/*** END ****************************************************************/
