#include "logger.h"
#include <stdio.h>


void logger::initLogger (void)
{
    verboseOn = false;
    debugOn = false;
    errorOn = false;
}

logger::logger (const char *prgName,
                const char *prefix,
                FILE *logFile) 
{
    this->logFile = logFile;
    this->logPrefix = prefix;
    this->prgmName = prgName;
    initLogger();
}

logger::logger (const char *prgName,
                FILE *logFile) 
{
    this->logFile = logFile;
    this->logPrefix = "Logger";
    this->prgmName = prgName;
    initLogger();
}

logger::logger (const char *prgName)
{
    this->logFile = stdout;
    this->logPrefix = "Logger";
    this->prgmName = prgName;
    initLogger();
}

logger::logger () 
{
    this->logFile = stdout;
    this->logPrefix = "Logger";
    this->prgmName = "";
    initLogger();
}

void logger::setLogFile (FILE *logfile)
{
    this->logFile = logfile;
}

void logger::setVerbose (bool on)
{
    this->verboseOn = on;
}

void logger::setDebug (bool on)
{
    this->debugOn = on;
}

void logger::setError (bool on)
{
    this->errorOn = on;
}

void logger::setInfo (bool on)
{
    this->infoOn = on;
}
void logger::logg (const char *levelstr, 
                   const char *fmt, va_list vargs)
{
    fprintf(logFile, "[%s] [%-7s] - ", prgmName, levelstr);
    vfprintf(logFile, fmt, vargs);
}

void logger::verbose (const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs,fmt); 
    
    if (verboseOn) {
        logg("Verbose",fmt, vargs);
    }
    va_end(vargs);
}

void logger::debug (const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs,fmt); 
    
    if (debugOn) {
        logg("Debug", fmt, vargs);
    }
}


void logger::error (const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs,fmt); 
    
    if (errorOn) {
        logg("Error",fmt, vargs);
    }
}

void logger::info (const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs,fmt); 
    
    if (infoOn) {
        logg("Info", fmt, vargs);
    }
}
