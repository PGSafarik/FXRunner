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

#ifndef FXRUNNER_UTILS_H
#define FXRUNNER_UTILS_H
/*************************************************************************
* Utils.h                                                                 *
*                                                                        *
* Deklarace zakladnich datovych struktur a hlavickovych souboru          *
* Copyright (c) 18/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "defs.h"

/* String list class */
class FXStringList : public FXArray<FXString> {

public :
  FXStringList( ) = default;
  virtual ~FXStringList( ) = default;

  /* Operations */
  FXString join( const FXString &sep = "", FXint n = 0 );       // Join content in this array
  FXint    split( const FXString &str, const FXString &sep );   // Splits the specified string into individual substrings separated by a separator
  void     sort( );

  /* Flags */
  enum {
    Search_forward = 0,  // Prohledavat od zacatku
    Search_backward,     // Prohledavat od konce
  };

  /* String List Iterator */
  class Iterator {
    FXStringList &list;
    FXival act;

  public:
    Iterator( FXival pos, FXStringList &reflist ) : list( reflist ), act( pos ) {};
    ~Iterator( ) = default;

    FXString operator*( ) { return list.at( act ); }

    void operator++( ) { if( act < list.no( ) ) { act++; } }
    void operator--( ) { if( act > 0 ) { act--; } }

    FXbool operator==( Iterator &other ) { return list.at( act ) == other.list.at( act ); }
    FXbool operator!=( Iterator &other ) { return list.at( act ) != other.list.at( act ); }
  };

  Iterator begin( ) { return Iterator( 0, *this ); }
  Iterator end( )   { return Iterator( no( ) - 1, *this ); }

protected :
};

/* Adition stream operators for compatibility with std::iostream and std::string */
extern FX::FXString& operator <<( FX::FXString &dest, const std::string &source );
extern std::string& operator <<( std::string &dest, const FX::FXString &source );
extern std::ostream& operator <<( std::ostream &store, const FX::FXString &str );
extern std::istream& operator >>( std::istream &store, FX::FXString &str );


#endif /* FXRUNNER_UTILS_H */
