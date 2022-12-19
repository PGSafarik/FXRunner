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
  FXString m_type;
  FXString m_name;
  FXString m_uri;       

public :
  Storage( const FXString &type );
  virtual ~Storage( );

  /* Access methods */
  FXString getType( ) const             { return m_type; }
  void setName( const FXString &value ) { m_name = value; }
  FXString getName( ) const             { return m_name; }
  FXString getUri( ) const              { return m_uri; } 
  FXbool   hasName( )                   { return !m_name.empty( ); }  
  FXbool   hasUri( )                    { return !m_uri.empty( ); }

  /* Operations */
  virtual FXbool open( const FXString &name, FXuint m = FXIO::Reading );
  virtual FXbool changeUri( const FXString &URI );
  virtual FXival readEntry( FXArray<FXString> &buff ) = 0;
  virtual FXival writeEntry( const FXArray<FXString> &buff ) = 0; 
  virtual FXbool close( ); 
  

protected:


};

/*** SIMPLE FILE STORAGE **************************************************************************/
class SimpleFileStorage : public Storage {
  FXArray<FXString> m_entries;   // Entries (file lines) list 
  FXbool            m_ready;     // Storage is ready (opened)

  FXString          m_delimiter; // The elements separator in entry
  FXString          m_filler;    // Fill in empty element in entry
  FXint             m_min;       // Minimum number elements in entry          
  

public :
  SimpleFileStorage( const FXString &path );
  ~SimpleFileStorage( );

  /* Access methods */
  FXString getSeparator( ) const { return m_delimiter; }
  void setSeparator( const FXString &sep ) { m_delimiter = sep; }
  FXString getFill( ) const      { return m_filler; }
  void setFill( const FXString &value ) { m_filler = value; }
  FXint getMinElements( )        { return m_min; }
  void setMinElement( FXint value ) { m_min = value; } 
  virtual  FXbool isOpen( )      { return m_ready; }
  virtual  FXlong size( )        { return m_entries.no( ); }
  virtual  FXint eof( );

  /* Operations */
  virtual FXbool open( const FXString &name, const FXString &delim = ";", const FXString &fill = "", FXint minel = 1, FXuint m = FXIO::Reading );
  virtual FXlong position( FXlong offset,FXuint from = FXIO::Begin );
  virtual FXival readEntry( FXArray<FXString> &buff );
  virtual FXival writeEntry( const FXArray<FXString> &buff );
  virtual FXbool flush( );
  virtual FXbool close( );  

protected:
  const FXString filename( );

};

#endif /* __STORAGE_H */
/*** END ******************************************************************************************/
