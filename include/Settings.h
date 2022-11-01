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
#ifndef __SETTINGS_H
#define __SETTINGS_H
/*************************************************************************
* Runner.h                                                               *
*                                                                        *
* Deklarace tridy hlavniho okna aplikace                                 *
* Copyright (c) 22/09/2012 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>
#include<Application.h>
#include<History.h>
#include<Boxes.h>

class Settings : public FXVerticalFrame {
FXDECLARE( Settings )
  FXbool m_changed;  // indicate change settings state
  FXbool m_revert;   // indicate revert back is available 

  /* Terminal emulator */
  FXComboBox   *tecb_enable;     // [enable]       - Enable/on request/disable 
  FXTextField  *tetf_command;    // [command]      - Terminal emulator program path
  FXTextField  *tetf_execprm;    // [prm_exec]     - Argument for executable command with TE 
  FXTextField  *tetf_disclosprm; // [prm_disclose] - Argument for disable closse with exit run command in TE
  FXTextField  *tetf_workdirprm; // [prm_workdir]  - Argument for set workdir for shell in TE

  /* Super user access */
  FXCheckButton *such_enable;      // [enable]  - enable / disable
  FXTextField   *sutf_askpassprm;  // [askpass] - Argument for using askpass 
  
  /* UI settings */
  FXComboBox *uicb_IconsTheme;     // [icons_theme] - icons theme name
 
public :
  Settings( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = FRAME_NONE | LAYOUT_FILL );
  virtual ~Settings( );

  /* Access methods */
  FXbool isChanged( ) { }
  FXbool isBackup( )  { }
  

  /* Operations */
  virtual void create( );

  void read_config( );
  void write_config( );  

  /* GUI messages & handlers */ 
  enum {
    SETTINGS_SAVE = FXVerticalFrame::ID_LAST,
    SETTINGS_RESTORE,
    SETTINGS_DEFAULT,
    ID_CHANGE,
    ID_LAST,
  };

  //long onCmd_Select( FXObject *sender, FXSelector sel, void *data );
  long onCmd_Settings( FXObject *sender, FXSelector sel, void *data );
  long onUpd_Settings( FXObject *sender, FXSelector sel, void *data );
  long onCmd_Update( FXObject *sender, FXSelector sel, void *data );

  

protected:
  Settings( ) { }

  /* Helper methods */
  void Notify( ) { if( target ) { target->tryHandle( this, FXSEL( SEL_CHANGED, message ), NULL ); } }
};

#endif /* __SETTINGS_H */
/*** END ****************************************************************/
