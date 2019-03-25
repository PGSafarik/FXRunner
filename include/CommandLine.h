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

#ifndef __COMMANDLINE_H
#define __COMMANDLINE_H
/*************************************************************************
* CommandLine.h                                                          *
*                                                                        *
* Widget pro zadavani prikazu a vyber z historie                         *
* Copyright (c) 23/11/2015 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>

class CommandLine : public FXComboBox {
  FXArray<Task> cmd_list;

public :
  CommandLine( FXComposite *p, FXObject* tgt = NULL, FXSelector sel = 0, FXuint opts = COMBOBOX_NORMAL );
  virtual ~CommandLine( );

  virtual void create( );
  virtual void save( FXStream &store )
  {
    FXint size = cmd_list.no( );
    store << size;

    if( size > 0 ) {
      for( FXint i = 0; i != size; i++ ) {
        cmd_list[ i ]->save( store );  // alt: store << ( *cmd_list[ i ] );
      }
    }
  }

  virtual void load( FXStream &store )
  {
    FXint size;
    store >> size;

    if( size < 0 ) {
      cmd_list.clear( );
      for( i = 0; i != size; i++ ) {
        Task *cmd = new Task;
        cmd->load( store );   // alt: store >> ( *cmd );
        cmd_list.append( cmd );
      }
    }
  }
};

#endif /* __COMMANDLINE_H */
/*** END ****************************************************************/
