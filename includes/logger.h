#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


static char* logPath = NULL;
static FILE* fp = NULL;
void log_config_init(char* path);
void log_config_free();

#define LOG(fmt, ...) \
    actTime2Console();   \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+");   \
    actTime2File(fp);   \
    fprintf(fp, fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#ifdef DEBUGLOGON
#define DEBUG(fmt, ...) \
    actTime2Console();   \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+");   \
    actTime2File(fp);   \
    fprintf(fp, "DEBUG:  \t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#define WARNING(fmt, ...)   \
    actTime2Console();   \
    fprintf(stderr, "\e[93m" fmt "\e[0m", ##__VA_ARGS__);   \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+"); \
    actTime2File(fp);   \
    fprintf(fp, "WARNING:\t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#define ERROR(fmt, ...)     \
    actTime2Console();   \
    fprintf(stderr, "\e[91m" fmt "\e[0m", ##__VA_ARGS__);   \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+"); \
    actTime2File(fp);   \
    fprintf(fp, "ERROR:  \t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#elif LOGON
FILE* fp = NULL;
#define DEBUG(fmt, ...) \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+");   \
    actTime2File(fp);   \
    fprintf(fp, "DEBUG :  \t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#define DEBUG(fmt, ...) \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+");   \
    actTime2File(fp);   \
    fprintf(fp, "DEBUG:  \t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#define WARNING(fmt, ...)   \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+"); \
    actTime2File(fp);   \
    fprintf(fp, "WARNING:\t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#define ERROR(fmt, ...)     \
    firstRunChecker();  \
    if(logPath != NULL) {   \
    fp = fopen(logPath, "a+"); \
    actTime2File(fp);   \
    fprintf(fp, "ERROR:  \t" fmt, ##__VA_ARGS__);    \
    fclose(fp);}

#elif DEBUGON
#define DEBUG(fmt, ...) \
    actTime2Console();   \
    fprintf(stderr, fmt, ##__VA_ARGS__)

#define WARNING(fmt, ...)   \
    actTime2Console();   \
    fprintf(stderr, "\e[93m" fmt "\e[0m", ##__VA_ARGS__)

#define ERROR(fmt, ...)   \
    actTime2Console();   \
    fprintf(stderr, "\e[91m" fmt "\e[0m", ##__VA_ARGS__)

#else
#define DEBUG(fmt, ...)
#define WARNING(fmt, ...)
#define ERROR(fmt, ...)

#endif


void firstRunChecker(void);
void actTime2Console(void);
void actTime2File(FILE* fp);


#endif  /* LOGGER_H */