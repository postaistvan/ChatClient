#ifndef __RADLY_LIB_LOGGER_H__
#define __RADLY_LIB_LOGGER_H__

#define LOG_SIZE_S  128
#define LOG_SIZE_M  512
#define LOG_SIZE_L  1024
#define LOG_SIZE_XL 2048

#define LOG_NORMAL   1
#define LOG_VERBOSE  2
#define LOG_DEBUG    4

#define OUTPUT_LOG 	0
#define OUTPUT_TERMINAL 1

/**
 *	Macros to define terminal colors.
 */
#define TERMINAL_COLOR_RED	 	"\033[1;31m"
#define TERMINAL_COLOR_GREEN 	"\033[1;32m"
#define TERMINAL_COLOR_YELLOW 	"\033[1;33m"
#define TERMINAL_COLOR_BLUE		"\033[1;34m"
#define TERMINAL_COLOR_END 		"\033[0m"

/**
 *	different labels to the output messages ( with colors ).
 */
#define LOG_WARNING_C		TERMINAL_COLOR_YELLOW	"[ WARNING ]"  	TERMINAL_COLOR_END
#define LOG_INFO_C			TERMINAL_COLOR_BLUE 	"[ INFO ]"	 	TERMINAL_COLOR_END
#define LOG_ERROR_C			TERMINAL_COLOR_RED  	"[ ERROR ]"		TERMINAL_COLOR_END
#define LOG_SUCCESS_C		TERMINAL_COLOR_GREEN	"[ SUCCESS ]" 	TERMINAL_COLOR_END

#define _WARNING_CPP	<< WARNING 	<<
#define _ERROR_CPP		<< ERROR 	<<
#define _INFO_CPP		<< INFO 	<<
#define _CONSOLE_CPP_	<< CONSOLE 	<<
#define _CONSOLE_CPP	<< CONSOLE



#define	LEVEL_ERROR 	1
#define LEVEL_WARNING 	2
#define LEVEL_PRINT 	3

#define LOG_LEVEL_ALL 			1
#define LOG_LEVEL_ONLY_ERROR 	2
#define LOG_LEVEL_NORMAL		3

#define LOG_	( LOG_LEVEL_ALL )

#if defined(NO_LOG)
#define LOG_LEVEL ( 0 )
#else
 #define LOG_LEVEL ((LOG_NORMAL)|(LOG_DEBUG)|(LOG_VERBOSE))
#endif

#define COVER( flag ) ( ( (LOG_LEVEL) & (flag) ) == (flag) ? (true) : (false) )

#define MULTIPLE_CLIENTS_LOG_STORE


#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

#include <iostream>

using namespace std;

class Logger
{
public:
    /*
     * Name of the file where to save the logs
     */
    static const char* logfilename_prints;
    static const char* logfilename_warnings;
    static const char* logfilename_errors;

    static uint64_t log_id;
    static uint64_t client_id;
    /*
     * All methods try to write to the logfile the message given as a parameter,
     * or to the console if the file cannot be opened. Use this functions to emphasize
     * the level of the log. Tag is used to specify where the message comes from.
     * Ex: Logger::print("MyClass", "This is a print");
     */
    static void print	( const char* tag, const char* message );
    static void warning	( const char* tag, const char* message );
    static void error	( const char* tag, const char* message );
	static void debug	( const char* tag, const char* message );

	static void console_info 	( const char* tag, const char* warning );
	static void console_warning ( const char* tag, const char* warning );
	static void console_error	( const char* tag, const char* error );
	static void console_success	( const char* tag, const char* debug );

//	C++
	static void console_info 	( string tag, string warning );
	static void console_warning ( string tag, string warning );
	static void console_error	( string tag, string error );
	static void console_success	( string tag, string debug );


	/*system path to logfile*/
    static void setAppSharedDataPath(char const * _appSharedDataPath);
    static void setLogId(uint64_t logId);
private:
    /*
     * Lables of each log level
     */
    static const char* print_level_text;
    static const char* warning_level_text;
    static const char* error_level_text;
	static const char* debug_level_text;
    static char appSharedDataPath[512];
    static void log_to_file( const char* logLevel, const char* tag, const char* message, char type );
    static void log_to_console ( const char* logLevel, const char* tag, const char* message );
    static void log_to_console ( string logLevel, string tag, string message );
    static char* get_time();
    static unsigned long GetFileSize(char* filename);
};


#endif /* __RADLY_LIB_LOGGER_H__ */
