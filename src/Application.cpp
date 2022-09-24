// Application.cpp; Copyright (c) 22/09/2012 D.A.Tiger; GNU GPL 3
#include<Application.h>

FXDEFMAP( Application ) APPLICATION_MAP[ ] = { };
FXIMPLEMENT( Application, FXApp, APPLICATION_MAP, ARRAYNUMBER( APPLICATION_MAP ) )

/*************************************************************************************************/
Application::Application( )
           : FXApp( "Runner", "TigerDesktopTools" )
{
  std::cout << "=== FXRunner ========================================" << std::endl;
  std::cout << "Copyright " << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << "  D.A.Tiger <drakarax@seznam.cz>, GNU GPL 3" << std::endl;
  std::cout << "App version: "<< AutoVersion::MAJOR<< "."<< AutoVersion::MINOR << "." << AutoVersion::REVISION;
  std::cout << " [" << AutoVersion::STATUS << "]" << std::endl;
  std::cout << "lib Fox    : " << FOX_MAJOR << "." << FOX_MINOR << "." << FOX_LEVEL << std::endl;
  std::cout << "=== Message =========================================" << std::endl;
  std::cout.flush( );

  a_cfg = new app_config;
  a_icdict = new FXIconCache( this );

  settings_load( );
  icons_make( );
}

Application::~Application( )
{
  settings_save( );
  std::cout << "=== End =============================================" << std::endl;
}

/*************************************************************************************************/
FXint Application::task_exec( Task *cmd )
{
  FXint resh = -1;
  FXString _command = "";
  FXString _cmd = "";
  FXString _term = "";

  std::cout << "Running: " << cmd->cmd.text( ) << std::endl;
/*
  std::cout << cmd->cmd.text( ) << std::endl;
  std::cout << cmd->wpth.text( ) << std::endl;
  std::cout << cmd->su << std::endl;
  std::cout << cmd->ow << std::endl;
  std::cout << cmd->te << std::endl;
*/
  // Spustit s pravy uzivatele root?
  if( cmd->su == true ) { _command += a_cfg->su + " "; }
  // Spustit v terminalu
  if( cmd->te == true ) {
    _term = a_cfg->term + " ";
    // Nastavit pracovni cestu v terminalu
    if( !cmd->wpth.empty( ) ) {
      if( cmd->te ) { _term += "--workdir " + cmd->wpth + " "; }
      else { _cmd += "cd " + cmd->wpth + "; "; }
    }
    // Nezavirat terminal
    if( cmd->lt == true ) { _term += a_cfg->term_noclose + " "; }
    _term += a_cfg->term_run + " ";
  }
  // Prikaz
  if( !cmd->cmd.empty( ) ) { _cmd += cmd->cmd; }
  // Parametry
  if( !cmd->prm.empty( ) ) { _cmd += cmd->prm; }
  // Parametry
  if( cmd->ow == true ) { _cmd += "&"; }
  //if( cmd->te == true ) { _cmd = "\"" + _cmd + "\""; }

  if( _cmd.empty( ) != true  ) {
    _command += _term + _cmd;
    std::cout << _command.text( ) << std::endl;
    resh = system( _command.text( ) );
  }
  /*
    FXProcess proc;
    proc.start( _cmd.text( ), _args.data( ) );
    a_processes->insert( &proc );
  */
  //std::cout<< "Execute resulth " << resh << std::endl;

  return resh;
}

void Application::task_write( Task *cmd, const FXString &pth )
{
  if( ( pth.empty( ) == true ) || ( cmd == NULL )  ) { return; }

  std::cout << "writing " << pth.text( ) << std::endl;

  FXSettings desk_file;
  FXString   desk_head = "Desktop Entry";
  FXString   command, name = FXPath::name( cmd->cmd );

  if( cmd->su == true ) { command += a_cfg->su + " "; }
  command += cmd->cmd;
  if( !cmd->prm.empty( ) ) { command += " " + cmd->prm; }

  desk_file.writeStringEntry( desk_head, "name",     name.text( ) );
  desk_file.writeStringEntry( desk_head, "TryExec",  cmd->cmd.text( ) );
  desk_file.writeStringEntry( desk_head, "Exec",     command.text( ) );
  desk_file.writeStringEntry( desk_head, "Path",     cmd->wpth.text( ) );
  desk_file.writeBoolEntry(   desk_head, "Terminal", cmd->te );

  desk_file.writeStringEntry( desk_head, "Comment", FXString::null );
  desk_file.writeStringEntry( desk_head, "Icon",    FXString::null );
  desk_file.writeStringEntry( desk_head, "Notyfy",  FXString::null );
  desk_file.writeStringEntry( desk_head, "Type",    "Application" );

  desk_file.unparseFile( pth + "/" + name + ".desktop" );
}

FXIcon* Application::icon_copy( const FXString &ic_name )
{
  FXIcon *ic = NULL;
  FXIcon *source = NULL;
  FXColor *_data = NULL;
  FXint w, h, size;

  if( ( source = a_icdict->find( ic_name.text( ) ) ) != NULL ) {
    source->restore( );
    w = source->getWidth( );
    h = source->getHeight( );
    size = w * h;
    callocElms( _data, size );
    copyElms( _data, source->getData( ), size );
    ic = new FXIcon( this, _data, source->getTransparentColor( ), source->getOptions( ) | IMAGE_KEEP | IMAGE_SHMI, w, h );
  }

  return ic;
}

void Application::settings_load( )
{
   if( reg( ).no( ) <= 0 ) { reg( ).read( ); }

   a_cfg->su           = reg( ).readStringEntry( getClassName( ), "Change user",  "kdesudo" );
   a_cfg->term         = reg( ).readStringEntry( getClassName( ), "Terminal",     "konsole" );
   a_cfg->term_noclose = reg( ).readStringEntry( getClassName( ), "No close term", "--noclose" );
   a_cfg->term_run     = reg( ).readStringEntry( getClassName( ), "Term run",      "-e" );
   a_cfg->icon_path    = reg( ).readStringEntry( getClassName( ), "Icons path",    "/usr/share/icons/oxygen/base/22x22/actions" );
   //a_cfg-> = reg( ).readStringEntry( getClassName( ), "", FXString::null ); */
}

void Application::settings_save( )
{
  reg( ).writeStringEntry( getClassName( ), "Change user",   a_cfg->su.text( ) );
  reg( ).writeStringEntry( getClassName( ), "Terminal",      a_cfg->term.text( ) );
  reg( ).writeStringEntry( getClassName( ), "No close term", a_cfg->term_noclose.text( ) );
  reg( ).writeStringEntry( getClassName( ), "Term run",      a_cfg->term_run.text( ) );
  reg( ).writeStringEntry( getClassName( ), "Icons path",    a_cfg->icon_path.text( ) );
  //reg( ).writeStringEntry( getClassName( ), "", a_cfg->  );

  if( reg( ).isModified( ) == true ) { reg( ).write( ); }
}

FXbool Application::icons_make( )
{
  /// nastaveni soarvce ikon
  FXString path = a_cfg->icon_path;
  a_icdict->setIconPath( path );

  ///nacteni ikon
  FXint num;
  FXbool resh = false;
  FXString *filelist;
  FXString file, im_path;
  FXString exp = "*.CUR|*.BMP|*.GIF|*.ICO|*.IFF|*.JPEG|*.PCX|*.PNG|*.PPM|*.RAS|*.RGB|*.TGA|*.TIFF|*.XBM|*.XPM";
  exp += "|*.cur|*.bmp|*.gif|*.ico|*.iff|*.jpeg|*.pcx|*.png|*.ppm|*.ras|*.rgb|*.tga|*.tiff|*.xbm|*.xpm";

  if( ( path.empty( ) != true ) && ( FXStat::isDirectory( path ) == true ) ) {
    if( path[ path.length( ) ] != PATHSEP ) { im_path = path + PATHSEP; }
    else { im_path = path; }

    if( ( num = FXDir::listFiles( filelist, im_path, exp ) ) > 0 ) {
      for( FXint i = 0; i != num; i++ ) {
        a_icdict->insert( FXPath::name( filelist[ i ] ).text( ) );
        //if( verb == true ) { std::cout << filelist[ i ].text( ) << std::endl; }
      }
      resh = true;
    }
    else { std::cout << getClassName( ) << "::icons_make - Path " << path.text( ) << " not found " << std::endl; }
  }

  return resh;
}

/*************************************************************************************************/
/* END */
