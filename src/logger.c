#include "logger.h"

void log_path_init(char* path)
{
    if(path == NULL) return;

    int pathLen = strlen(path);
    logPath = (char*)malloc(pathLen * sizeof(char));
    strncpy(logPath, path, pathLen);
    logPath[pathLen] = '\0';
}


char* log_path_get()
{
    return logPath;
}


void log_path_free()
{
    if(logPath != NULL)
    {
        free(logPath);
        logPath = NULL;
    }
}


void firstRunChecker(void)
{
    static bool firstRun = true;
    if(firstRun)
    {
        remove("Debug.log");
        firstRun = false;
    }
}


void actTime2Console(void)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(stderr, "%d.%02d.%02d %02d:%02d:%02d\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}


void actTime2File(FILE* fp)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, "%d.%02d.%02d %02d:%02d:%02d\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}



#ifdef RUNABLE
int main(int argc, char* argv[])
{
    printf("Number of arguments: %d\n", argc);
    for(int i = 0; i < argc; i++)
    {
        printf("Arguments: %s\n", argv[i]);
    }

    LOG("This is the LOG part at line: %d\n", __LINE__);
    DEBUG("This is the DEBUG part at line: %d\n", __LINE__);
    WARNING("This is the WARNING part at line: %d\n", __LINE__);
    ERROR("This is the ERROR part at line: %d\n", __LINE__);

    printf("Size of stderr: %lld\n", sizeof(stderr));

    return 0;
}
#endif