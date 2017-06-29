#pragma once
/////////////////////////////////////////////////////////////////////////////
//////// Update minor/major versions with more significant changes //////////
#define VERSION_MAJOR 0
#define VERSION_MINOR 3
/////////////////////////////////////////////////////////////////////////////
//////// Update build version for every change that goes to SVN /////////////
#define system_version_build 9
/////////////////////////////////////////////////////////////////////////////

// Nicer name alias (system_version_build name is required by build_count.exe)
#define VERSION_BUILD system_version_build

// Stringifying macros
#define STR_EXPAND(input) #input
#define STR(input) STR_EXPAND(input)

// Handy expand macros
#define VERSION_NUMBER VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD
#define VERSION_NUMBER_STR STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_BUILD)
