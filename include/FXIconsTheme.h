/*******************************************************************************
* File:      FXIconsTheme                                                      *
* Project:   FXDesktop                                                         *
* Copyright: Copyriright (C) 2019 - 2022 by D.A.Tiger GNU GPL v3               *
* Author(s): D.A.Tiger ( drakarax@seznam.cz )                                  *
* Note:      Application icon manager (dictionary - pattern method )           *
*******************************************************************************/
#include<fox-1.7/fx.h>
#include<map>

#include<main.h>

#ifndef __FXICONTHEME_H
#define __FXICONTHEME_H

typedef FXDictionaryOf<FXint>     FXSizesMap;
typedef std::map<FXString, FXint> __SizeMap; 
typedef FXArray<FXString>         FXStringList;

class FXIconsTheme {
  FXString t_name;		// Theme name
  FXString t_prefix;	// Ikon files exponent
  FXString t_path;		// Root path to the ikons theme
  FXString t_pattern;	// Pattern for composing paths to icon files in a theme

  FXStringList        t_list;      // List dictionaries (sections)
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

#endif /* __FXICONTHEME_H */
/*** END ******************************************************************************************/
