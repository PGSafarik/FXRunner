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
#ifndef FXRUNNER_PROPERTIES_H
#define FXRUNNER_PROPERTIES_H
/*************************************************************************
* Properties.h                                                               *
*                                                                        *
* Deklarace tridy hlavniho okna aplikace                                 *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<core.h>

class RunModes : public FXVerticalFrame {
FXDECLARE( RunModes )
  Application   *m_app;
  FXObject      *m_target;
  FXSelector    m_message;

  FXButton      *m_file_btn;
  FXButton      *m_dir_btn;
  FXTextField   *m_dir_text;

  FXCheckButton *m_su_check;
  FXCheckButton *m_nblock_check;
  FXCheckButton *m_rexit_check;
  FXCheckButton *m_rterm_check;
  FXCheckButton *m_ntexit_check;

  FXbool m_change;

public:
  RunModes( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts = FRAME_NONE | LAYOUT_FILL );
  ~RunModes( ) override;

  /* Access methods */

  /* Operations methods */
  void create( ) override;

  /* MSG & handlers */
  enum {
    MODE_CHANGE = FXVerticalFrame::ID_LAST, // The user changed the launch mode parameters
    MODE_UPDATE,                            // Update run mode parameters from current task
    MODE_APPLY,                             // Set the current task execution mode parameters
    MODE_RESET,                             // Reset execution mode variables at default values
    ID_WORKDIR,                             // Select and change work dir
  };
  long onCmd_Workdir( FXObject *tgt, FXSelector sel, void *data );
  long onCmd_Mode( FXObject *tgt, FXSelector sel, void *data );

protected:
  RunModes( ) { }
  void Reset( );    // Nastaveni vychozich modu spusteni
  long Notify( FXuint _type, void *_data = nullptr );
};

#endif //FXRUNNER_PROPERTIES_H