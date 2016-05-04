#ifndef __COMMON__
#define __COMMON__

enum LOGLEVELS {
    DEBUG=2,
    INFO=1,
    ERROR=0
};

#define LOGLEVEL DEBUG

#define BUF_SIZE 1024

#define dbFilePath "/var/tmp/databaseFile.db"

extern int counter;
extern FILE* fpDb;

#define LOG(x, y, ...) if(LOGLEVEL >= x) \
                                 printf(y, ## __VA_ARGS__);

#endif /* __COMMON__ */
