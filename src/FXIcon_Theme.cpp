// FXIcon_Theme; Copyright (c) 22/09/2012 D.A.Tiger; GNU GPL 3
#include<FXIcon_Theme.h>

/**************************************************************************************************/
FXIcon_Map::FXIcon_Map( FXApp *a )
          : FXDictionary( ), m_app( a )
{ }

FXIcon_Map::~FXIcon_Map( )
{ }

/**************************************************************************************************/
FXIconCache* FXIcon_Map::insert( const FXString &type, FXIconCache *ics )
{
  FXIconCache* res = NULL;
  res = ( FXIconCache* ) FXDictionary::insert( type.text( ), ( void* ) ics );
  /// operace s res
  return res;
}

FXIconCache* FXIcon_Map::insert( const FXString &type, const FXString &path )
{
   FXIconCache *res = ( FXIconCache* ) FXDictionary::insert( type.text( ), new FXIconCache( m_app, path ) );
   /// operace res
   return res;
}

FXIconCache* FXIcon_Map::find( const FXString &type )
{
  return ( FXIconCache* ) FXDictionary::find( type.text( ) );
}

FXIconCache* FXIcon_Map::data( FXint id )
{
  return ( FXIconCache* ) FXDictionary::data( id );
}

/**************************************************************************************************/
FXIcon_Theme::FXIcon_Theme( FXApp *a, const FXString &theme_name )
            : FXDictionary( ), th_app( a ), th_name( theme_name )
{ }

FXIcon_Theme::~FXIcon_Theme( )
{ }

/**************************************************************************************************/
FXIcon_Map* FXIcon_Theme::insert( const FXString &name, FXIcon_Map *icm )
{
   FXIcon_Map *res = NULL;

   if( ( res = find( name ) ) == NULL ) {
     res = ( FXIcon_Map* ) FXDictionary::insert( name.text( ), ( ( icm != NULL ) ? icm : new FXIcon_Map( th_app ) ) );
   }
   return res;
}

FXIcon_Map* FXIcon_Theme::remove( const FXString &name )
{
  return ( FXIcon_Map* ) FXDictionary::remove( name.text( ) );
}

FXIcon_Map* FXIcon_Theme::find( const FXString &name )
{
  return ( FXIcon_Map* ) FXDictionary::find( name.text( ) );
}

FXIconCache* FXIcon_Theme::find( const FXString &name, const FXString &type )
{
  /// Priklad: FXIconsDict *apps_icons = my_theme->find( "22x22", "apps" );
  FXIconCache *res = NULL;
  FXIcon_Map *map = NULL;

  if( ( map = find( name ) ) != NULL ) { res = map->find( type ); }
  return res;
}

FXIcon_Map* FXIcon_Theme::data( FXint id )
{
  return ( FXIcon_Map* ) FXDictionary::data( id );
}

FXIconCache* FXIcon_Theme::data( FXint id_map, FXint id_type )
{
  FXIconCache *res = NULL;
  FXIcon_Map *map = NULL;

  if( ( map = data( id_map ) ) != NULL ) { res = map->data( id_type ); }
  return res;
}

FXIconCache* FXIcon_Theme::data( const FXString &name, FXint id_type )
{
  FXIconCache *res = NULL;
  FXIcon_Map *map = NULL;

  if( ( map = find( name ) ) != NULL ) { res = map->data( id_type ); }
  return res;
}

/**************************************************************************************************/
FXIcon*  FXIcon_Theme::icon_get( const FXString &map, const FXString &type, const FXString &name, FXbool )
{
  /// Priklad: FXIcon *opera_ic = my_theme->icon_get( "22x22", "apps", "opera-browser", true );
  FXIconCache *dict = NULL;
  FXIcon     *ic   = NULL;

  if( ( dict = find( map, type ) ) != NULL ) { ic = dict->find( name.text( ) ); }
  if( ic != NULL ) {
    /*operace s ikonou*/
  }
  return ic;
}

FXIcon*  FXIcon_Theme::icon_copy( const FXString &map, const FXString &type, const FXString &name, FXbool )
{
  /// Priklad: FXIcon *opera_ic = my_theme->icon_copy( "22x22", "apps", "opera-browser", true );
  FXIcon *ic = NULL;

  if( ( ic = icon_get( map, type, name, false ) ) != NULL ) {
    // operace s ikonou
  }
  return ic;
}

FXString  FXIcon_Theme::icon_path( const FXString &map, const FXString &type, const FXString &name )
{
  /// Priklad: std::cout << my_theme->icon_path( "22x22", "apps", "opera-browser" ).text( ) << std::endl;
  FXString    pth  = FXString::null;
  FXIconCache *dict = NULL;

  if( ( dict = find( map, type ) ) != NULL ) { pth = dict->getIconPath( ); }
  return pth;
}

/*************************************************************************************************/
FXbool FXIcon_Theme::read( const FXString &pth, FXbool all )
{
   FXString dir, form, size_min, size_max, glob, head = "Theme";
   FXString mxpath, mnpath, frm;
   FXString_List paths, subtheme, supports, sizes;
   FXString_List p_all, p_index, p_query, p_none;

   FXString src_file = pth;
   if( pth[ pth.length( ) + 1 ] != PATHSEP ) { src_file += PATHSEPSTRING; }
   src_file += th_name + ".rc";
   FXbool ret = th_rc.parseFile( src_file );


   if( ret != false ) {
     paths    = __substrs( th_rc.readStringEntry( head, "path_find", FXString::null ), ";" );
     subtheme = __substrs( th_rc.readStringEntry( head, "sub_list",  FXString::null ), ";" );

     head = subtheme.at( 0 );
     dir      = th_rc.readStringEntry( head, "root",         FXString::null );
     glob     = th_rc.readStringEntry( head, "global_index", FXString::null );
     form     = th_rc.readStringEntry( head, "map_format",   FXString::null );
     size_max = th_rc.readStringEntry( head, "size_big",     "64"           );
     size_min = th_rc.readStringEntry( head, "size_small",   "16"           );

     sizes    = __substrs( th_rc.readStringEntry( head, "size_map",      FXString::null ), ";" );
     supports = __substrs( th_rc.readStringEntry( head, "support",       FXString::null ), ";" );
     p_all    = __substrs( th_rc.readStringEntry( head, "preload_all",   FXString::null ), ";" );
     p_index  = __substrs( th_rc.readStringEntry( head, "preload_index", FXString::null ), ";" );
     p_query  = __substrs( th_rc.readStringEntry( head, "preload_query", FXString::null ), ";" );
     p_none   = __substrs( th_rc.readStringEntry( head, "preload_none",  FXString::null ), ";" );


   }
   else { std::cerr << ret << " Nothing icon theme indexfile " << src_file.text( ) << std::endl; }

   return ret;
}

//FXbool FXIcon_Theme::write( const FXString &pth )
//{
//
//}

/*************************************************************************************************/
void Preload( FXString pth, const FXString_List &size_map )
{
  //for( )

}

FXString FullPath( const FXString &directory, const FXString_List &pth )
{
  FXString tmp;

  for( FXint i = 0; i != pth.no( ); i++ ){
    tmp = pth.at( i );
    if( tmp[ tmp.length( ) + 1 ] != '/' ) { tmp += "/"; }
    tmp += directory;
    if( FXStat::exists( tmp ) ) { break; }
  }

  return tmp;
}

/*** END ******************************************************************************************/











