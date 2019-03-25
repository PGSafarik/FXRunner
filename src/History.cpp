// History.cpp; Copyright (c) 18/03/2019;  D.A.Tiger; GNU GPL 3
#include<History.h>

History_b::History_b( FXint num, const FXString name ) : h_name( name ), h_num( num ), h_hash( 0 ), h_change( false )
{ }

History_b::~History_b( )
{ clear( ); }

/**************************************************************************************************/

void History_b::dump( )
{
  std::cout << "Dumping array: " << h_name.text( ) << " with " << no( ) << " items" << std::endl;
  for( FXint i = 0; i != no( ); i++ ) { std::cout << at( i ).text( ) << std::endl; }
}

void History_b::insert( const FXString &data )
{
  FXint size_buff = no( );
  for( FXint i = 0; i != size_buff; i++ ) { if( data == at( i ) ) { erase( i ); } }

  push( data );
  h_change = true;

}

void History_b::_clear( )
{
  h_change = true;
  this->clear( );
}

FXbool History_b::save( const FXString &file, FXbool mk )
{
  FXString tmp = "";

  if( h_change == false ) { return true; }
  if( file != FXString::null ) {
    if( FXStat::exists( file ) == false ) {
      if( mk != true ) {
        std::cout << "Cache file not found!" << std::endl;
        return false;
      }
      else{ FXFile::create ( file ); }
    }

    for( FXint i = 0; i != no( ); i++ ) {
       tmp += at( i ) + "\n";
    }


    FXFile fd;
    if( tmp.empty( ) ) { std::cout << "Cache is empty!" << std::endl; }
    if( fd.open( file, FXIO::Writing ) == true ) {
      std::cout << "save cache " << h_name.text( ) << ": " << file.text( ) << std::endl;
      FXint n = fd.writeBlock( tmp.text( ), tmp.length( ) );
      std::cout << "write data of size: " << n << std::endl;
      fd.close( );
    }
    else {
      std::cout << "Cache file not open!" << std::endl;
      return false;
    }
  }
  h_change = false;
  return true;
}

FXbool History_b::load( const FXString &file )
{
  if( no( ) > 0 ) {
    clear( );
    h_hash = 0;
  }

  if( !file.empty( ) ) {
    FXint    i;
    FXString tmp, data;
    FXFile   fd;

    if( FXStat::exists( file ) == true ) { //FXDir::create ( file ); }
      if( fd.open( file ) != true ) { std::cout << "File not open!" << std::endl; return false; }
      std::cout << "load cache \'" << h_name.text( ) << "\': " << file.text( ) << std::endl;
      tmp.length( fd.size( ) );
      if( fd.readBlock( tmp.text( ), fd.size( ) ) > 0 ) {
        for( i = 0; i != tmp.length( ); i++ ) {
          char c = tmp[ i ];
          if( c == '\n' ) { FXArray<FXString>::push( data ); data = ""; }
          else { data.insert( data.length( ), c ); }
        }
        if( !data.empty( ) ) { FXArray<FXString>::push( data ); }
      }
      else { std::cout << "File is not read!" << std::endl; return false; }
      fd.close( );
    }
    else { std::cout << "File not exist!" << std::endl; return false; }
  }
  return true;
}

/*** END ******************************************************************************************/
