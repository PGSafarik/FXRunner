/*************************************************************************
* Settings.cpp Copyright (c) 2022 by  D.A.Tiger                          *
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
#include<Settings.h>

FXDEFMAP( Settings ) SETTINGS_MAP[ ] = { };
FXIMPLEMENT( Settings, FXApp, SETTINGS_MAP, ARRAYNUMBER( SETTINGS_MAP ) )

/*************************************************************************************************/
Settings::Settings( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts )
{


}

Settings::~Settings( )
{


}

void Settings::create( )
{


}

/*************************************************************************************************/
void Settings::read_config( )
{

}

void Settings::write_config( )  
{


}

/*************************************************************************************************/
long Settings::onCmd_Settings( FXObject *sender, FXSelector sel, void *data )
{


}

long Settings::onUpd_Settings( FXObject *sender, FXSelector sel, void *data )
{


}

long onCmd_Update( FXObject *sender, FXSelector sel, void *data )
{


}

/*************************************************************************************************/
/* END */
