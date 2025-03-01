#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2025";
	static const char UBUNTU_VERSION_STYLE[] =  "25.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR    = 0;
	static const long MINOR    = 4;
	static const long REVISION = 0;
	static const long BUILD = 0;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1;
	#define RC_FILEVERSION 0,4,0,0
	#define RC_FILEVERSION_STRING "0, 4, 0, 0\0"
	static const char FULLVERSION_STRING [] = "25.03 0.4.0.0 Alpha";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
}
#endif //VERSION_H
