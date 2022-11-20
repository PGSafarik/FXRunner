/*************************************************************************
* Runner.cpp Copyright (c) 2019 - 2022 by  D.A.Tiger                     *
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
#include "FXIconsTheme.h"
#include<iostream>

FXIconsTheme::FXIconsTheme( FXApp *app )
{
  t_cache = new FXIconCache( app, FXSystem::getHomeDirectory( ) );
}

FXIconsTheme::~FXIconsTheme( )
{ }

/*************************************************************************************************/
FXString FXIconsTheme::operator[ ]( const FXString &input )
{
  /*NOTHING*/
  return FXString::null;
}


FXString FXIconsTheme::at( const FXString &name, int size )
{
  FXString resh = t_pattern;
  FXString tr = t_dict[ name ];

  if( !tr.empty( ) ) {
    FXint pos = tr.find( "/" );
    FXString i_sect = tr.left( pos );
    FXString i_name = tr.mid( pos + 1, tr.length( ) );

    // We have everything we need - We will start replacing the pattern with values
    resh.substitute( "${SIZE}", FXString::value( size ) );
    resh.substitute( "${TYPE}", i_sect );
    resh.substitute( "${NAME}", i_name );
    resh.substitute( "${EXP}",  t_prefix ); 
  }
  
  return resh;
}

FXIcon* FXIconsTheme::get_icon( const FXString &name, const FXString &size_alias )
{
   FXint _size = 16;
   if( !_sizes.empty( ) ) { _size = _sizes.at( size_alias ); }
   return get_icon( name, _size );
}

void FXIconsTheme::load( const FXString &themefile, const FXString &name )
{
  FXSettings data;
  FXString   head;

  t_dict.clear( );
  t_sizes.clear( );
  t_cache->clear( );
  t_list.clear( );

  if( FXStat::exists( themefile ) && data.parseFile( themefile ) ) {
    std::cout << "Load icons theme \'" << name.text( ) << "\' from \'" << themefile.text( ) << "\' mapfile" << std::endl;

    if ( data.existingSection( "Sizes" ) ) {
      FXStringDictionary sm = data.at( "Sizes" );
      for( FXint i = 0; i != sm.no( ); i++ ) {
        FXString k = sm.key( i );
        if( !k.empty( ) ) {
          FXint value =  sm.data( i ).toInt( );
          _sizes.insert( std::pair<FXString, int>( k, value ) );
        }
      }
    } 

    // Read base informations about icon theme
    t_name    = name; 
    if( data.existingSection( t_name ) ) {
      t_prefix  = data.readStringEntry( t_name, "map.type", "png" );
      t_path    = data.readStringEntry( t_name, "map.path" );
      t_pattern = data.readStringEntry( t_name, "map.pattern" );

      // Read a icons dictionary
      t_dict = data.at( t_name ); 
    }
  } 
  else { std::cout << "Load icons theme \'" << themefile.text( ) << "\' is NOT LOADED!" << std::endl;}

  std::cout.flush( );
}

/*** END ******************************************************************************************/
