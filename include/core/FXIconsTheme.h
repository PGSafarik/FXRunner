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

#ifndef FXRUNNER_FXICONTHEME_H
#define FXRUNNER_FXICONTHEME_H
/*************************************************************************
* FXIconsTheme.h                                                         *
*                                                                        *
* Application icon manager (dictionary - pattern method )                *
* Copyright (c) 18/03/2019 D.A.Tiger <drakarax@seznam.cz>                *
*************************************************************************/
#include "core/defs.h"
//? #include "core/StringUtils.h"

typedef FXDictionaryOf<FXint>     FXSizesMap;
typedef std::map<FXString, FXint> __SizeMap;

class FXIconsTheme {
  FXString t_name;		// Theme name
  FXString t_prefix;	// Ikon files exponent
  FXString t_path;		// Root path to the ikons theme
  FXString t_pattern;	// Pattern for composing paths to icon files in a theme

  //? FXStringList        t_list;      // List dictionaries (sections)
  FXStringDictionary  t_dict;	     // Dictionary (section)
  FXIconCache        *t_cache;     // Icons cache 
  FXSizesMap          t_sizes;     // Sizes map

  __SizeMap _sizes;

public:
  FXIconsTheme( FXApp *app );
  ~FXIconsTheme( );

  /* Access methods */
  FXString get_path( )   { return t_path; }
  FXString get_prefix( ) { return t_prefix; }

  /* Operators */
  FXString operator[ ]( const FXString &input );       // FXString iOpen_str = ic_theme[ "Open:16" ];

  /* Operations method */
  FXString at( const FXString &name, int size );
  FXIcon* get_icon( const FXString &name, FXint size = 16 ) { return t_cache->insert( this->get_path( ) + "/" + this->at( name, size ) ); }
  FXIcon* get_icon( const FXString &name, const FXString &size_alias );

  void load( const FXString &themefile, const FXString &name );
};

#endif /* FXRUNNER_FXICONTHEME_H */
/*** END ******************************************************************************************/
