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

#ifndef __STORAGE_H
#define __STORAGE_H
/*************************************************************************
* Storage.h                                                              *
*                                                                        *
* Rozhrani uloziste uloh                                                 *
* Copyright (c) 10/12/2022 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include<main.h>

/*** ABSTRACT STORAGE INTERFACE ********************************************************************/
class Storage : public FXIODevice {
  FXString          m_type;
  FXString          m_name;
  //FXArray<FXString> m_entries;

public :
  Storage( const FXString &type );
  virtual ~Storage( );

  /* Access methods */
  FXString getType( ) const             { return m_type; }
  void setName( const FXString &value ) { m_name = value; }
  FXString getName( ) const             { return m_name; }

  /* Operations */
  virtual FXbool open( const FXString &name, FXuint m = FXIO::Reading );
  virtual FXival readEntry( FXArray<FXString> &buff ) = 0;
  virtual FXival writeEntry( const FXArray<FXString> &buff ) = 0; 

protected:


};


#endif /* __STORAGE_H */
/*** END ******************************************************************************************/
