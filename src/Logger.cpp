/*
 * logger.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: horatiu
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include "Logger.h"

const char* Logger::logfilename_prints = "prints.log";
const char* Logger::logfilename_warnings = "prints.log"; //"warnings.log";
const char* Logger::logfilename_errors = "prints.log";//"errors.log";
const char* Logger::print_level_text = "MESSAGE";
const char* Logger::warning_level_text = "WARNING";
const char* Logger::error_level_text = "ERROR";
const char* Logger::debug_level_text = "DEBUG";
char Logger::appSharedDataPath[512];
uint64_t Logger::log_id = 0;
uint64_t Logger::client_id;
#define MAX_LOGFILE_SIZE_ALLOWED 10*1024*1024  //200 MB
static bool was_notified = false;

void Logger::setLogId(uint64_t log_id)
{
    Logger::log_id = log_id;
}
char* Logger::get_time()
{
    time_t rawtime;
    struct tm* timeinfo;

    time( &rawtime );
    timeinfo = localtime( &rawtime );
    char* time = asctime( timeinfo );
    time[strlen( time ) - 1] = time[strlen( time )];

    return time;
}
unsigned long Logger::GetFileSize(char* filename)
{
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : 0;
}
void Logger::setAppSharedDataPath( char const* _appSharedDataPath)
{
    strcpy(appSharedDataPath, _appSharedDataPath);
}

void Logger::log_to_file( const char* log_level, const char* tag, const char* message, char type )
{
    FILE* logfile = NULL;
    char dataPath[512];

    if(strlen(appSharedDataPath)<1)
    {
#ifdef MULTIPLE_CLIENTS_LOG_STORE
        sprintf(dataPath, "%llu_%s", log_id, logfilename_prints);

        if(!was_notified) {
            was_notified = true;
        }
#else
        sprintf(dataPath, "%s",logfilename_prints);
#endif
        logfile = fopen( dataPath, "a" );
        /*
        switch ( type )
        {
        case 1:
            logfile = fopen( logfilename_prints, "a" );
            break;
        case 2:
            logfile = fopen( logfilename_prints, "a" );
            break;
        case 3:
            logfile = fopen( logfilename_prints, "a" );
            break;
        }
        */
    }
    else
    {


#ifdef MULTIPLE_CLIENTS_LOG_STORE
        sprintf(dataPath, "%s/%llu_%s", appSharedDataPath, log_id, logfilename_prints);

        if(!was_notified)
        {
            was_notified = true;
        }
#else
        sprintf(dataPath, "%s/%s",appSharedDataPath,logfilename_prints);
#endif
        logfile = fopen( dataPath, "a" );
        /*
        switch ( type )
        {
        case 1:

            sprintf(dataPath, "%s/%s",appSharedDataPath,logfilename_prints);
            logfile = fopen( dataPath, "a" );
            break;
        case 2:
            sprintf(dataPath, "%s/%s",appSharedDataPath,logfilename_prints);
            logfile = fopen( dataPath, "a" );
            break;
        case 3:
            sprintf(dataPath, "%s/%s",appSharedDataPath,logfilename_prints);
            logfile = fopen( dataPath, "a" );
            break;
        }
        */
    }
    if ( logfile == NULL )
    {
        /* Fall back to console output if unable to open file */
        printf( "Rerouted to console: %s: %s %s\n", tag, log_level, message );
        return;
    }
    else
    {
        fprintf( logfile, "[%s] %s %s: %s\n", get_time(), log_level, tag, message );
        fclose( logfile );
    }

    if(GetFileSize(dataPath)>MAX_LOGFILE_SIZE_ALLOWED)
    {
        logfile = fopen( dataPath, "w" );
        if(logfile!=NULL)
        fclose( logfile );
    }
}

void Logger::print( const char* tag, const char* message )
{
#ifndef USE_ANDROID1
#ifdef LOG_LEVEL
    log_to_file( print_level_text, tag, message, 1 );
#endif
#else
    (void)tag;
    (void)message;
#endif
}

void Logger::warning( const char* tag, const char* message )
{
#ifndef USE_ANDROID1
#ifdef LOG_LEVEL
    log_to_file( warning_level_text, tag, message, 2 );
#endif
#else
    (void)tag;
    (void)message;
#endif
}

void Logger::error( const char* tag, const char* message )
{
#ifndef USE_ANDROID1
#ifdef LOG_LEVEL
    log_to_file( error_level_text, tag, message, 3 );
#endif
#else
    (void)tag;
    (void)message;
#endif
}

void Logger::debug( const char* tag, const char* message )
{
#ifndef USE_ANDROID1
	#ifdef LOG_LEVEL
        if(COVER(LOG_DEBUG))
    		log_to_file( debug_level_text, tag, message, 1 );
	#endif
#else
    (void)tag;
    (void)message;
#endif
}

void Logger::console_info ( const char* tag, const char* message ) {
	log_to_console(LOG_INFO_C, string(tag), string(message));
}

void Logger::console_warning ( const char* tag, const char* message ) {
	log_to_console(LOG_WARNING_C, tag, message);
}

void Logger::console_error ( const char* tag, const char* message ) {
	char final_message[LOG_SIZE_L];

	sprintf(final_message, "%s%s%s", TERMINAL_COLOR_RED, message, TERMINAL_COLOR_END);
	log_to_console(LOG_ERROR_C, tag, final_message);
}

void Logger::console_success ( const char* tag, const char* message ) {
	log_to_console(LOG_SUCCESS_C, tag, message);
}

void Logger::log_to_console ( const char* logLevel, const char* tag, const char* message ) {
	printf("%s %s%s radly[ %llu ]%s : %s\n", logLevel, TERMINAL_COLOR_BLUE, tag, Logger::client_id, TERMINAL_COLOR_END, message);
}

// C++
void Logger::console_info ( string tag, string message ) {
	log_to_console(LOG_INFO_C, tag, message);
}

void Logger::console_warning ( string tag, string message ) {
	log_to_console(LOG_WARNING_C, tag, message);
}

void Logger::console_error ( string tag, string message ) {
	stringstream final_message;

	final_message << TERMINAL_COLOR_RED << message << TERMINAL_COLOR_END;

	log_to_console(LOG_ERROR_C, tag, final_message.str());
}

void Logger::console_success ( string tag, string message ) {
	log_to_console(LOG_SUCCESS_C, tag, message);
}

void Logger::log_to_console ( string logLevel, string tag, string message ) {
	cout << logLevel << ' ' << TERMINAL_COLOR_BLUE << tag << TERMINAL_COLOR_END << " : " << message << endl;
}
