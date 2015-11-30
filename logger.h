#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdarg.h>

class logger {
    public:
        logger(const char *prgName, const char *prefix, FILE *logFile);
        logger(const char *prgName, FILE *logFile);
        logger(const char *prgName);
        logger(void);
        void setLogFile(FILE *logfile);
        void setVerbose(bool on);
        void setDebug(bool on);
        void setError(bool on);
        void setInfo(bool on);
        void verbose(const char *fmt, ...);
        void debug(const char *fmt, ...);
        void error(const char *fmt, ...);
        void info(const char *fmt, ...);
    private:
        FILE *logFile;
        bool verboseOn;
        bool debugOn;
        bool errorOn;
        bool infoOn;
        const char *prgmName;
        const char *logPrefix;
        void initLogger(void);
        void logg(const char *levelstr, 
                  const char *fmt, va_list vargs);
};


#endif
