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

#ifndef __FXICON_THEME_H
#define __FXICON_THEME_H
/*************************************************************************
* FXIcon_Theme.h                                                         *
* PROBLEMATIC                                                                       *
* Sprava Ikon/tematu                                                     *
* 22/09/2012; Copyright D.A.Tiger; GNU GPL 3                             *
*************************************************************************/
#include<main.h>

class FXIcon_Map : public FXDictionary {
  FXApp    *m_app; // Instace aplikace
  FXString  m_id;  // Nazev mapy

public :
  FXIcon_Map( FXApp *a );
  virtual ~FXIcon_Map( );

  ////////////////////////////////////////////////
  /// Operace
  ///
  FXIconCache* insert( const FXString &type, FXIconCache *ics );
  FXIconCache* insert( const FXString &type, const FXString &path );

  FXIconCache* find( const FXString &type );
  FXIconCache* data( FXint id );

protected :
 // FXIconCache* make_dict( const FXString &path );
};


/*************************************************************************************************/
class FXIcon_Theme : public FXDictionary
{
  FXApp     *th_app;
  FXString   th_name;
  FXSettings th_rc;

public :
  FXIcon_Theme( FXApp *a, const FXString &theme_name );
  virtual ~FXIcon_Theme( );

  FXIcon_Map* insert( const FXString &name, FXIcon_Map *m = NULL );
  FXIcon_Map* remove( const FXString &name );
  FXIconCache* find( const FXString &name, const FXString &type );
  FXIcon_Map* find( const FXString &name );
  FXIconCache* data( const FXString &name, FXint id_type );
  FXIconCache* data( FXint id_map, FXint id_type );
  FXIcon_Map* data( FXint id );

  FXbool read( const FXString &pth, FXbool all = false );
  //FXbool write( const FXString &pth );

  FXIcon*  icon_get( const FXString &map, const FXString &type, const FXString &name, FXbool );
  FXIcon*  icon_copy( const FXString &map, const FXString &type, const FXString &name, FXbool );
  //FXIcon*  icon_index( ) { }
  //FXIcon*  icon_insert( ) { }
  FXString icon_path( const FXString &map, const FXString &type, const FXString &name = FXString::null );

protected:
  FXIcon_Theme( ) {}

  inline FXString_List __substrs( const FXString &str, const FXString &sep, FXint num = 0 )
  {
    FXString_List buff;
    FXint n, counter, i;

    i = 1;
    if( ( n = str.contains( sep ) ) > 0 ) {
      if( ( n > num ) && ( num > 0 ) ) { n = num; }
      for( counter = 0; counter != n; counter++ ) {
        buff.push( str.section( sep, counter, i ) );
      }
    }
    else { buff.push( str ); }

    return buff;
  }
};

#endif /* __FXICON_THEME_H */
/*** END ****************************************************************/
