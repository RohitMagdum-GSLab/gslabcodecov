#ifndef _ERROR__
#define __ERROR__

enum Error {
    SUCCESS               = 0,
    FILE_OPEN_ERROR       = -1,
    FILE_WRITE_ERROR      = -2,
    FILE_READ_ERROR       = -3,
    FILE_PERMISSION_ERROR = -4,
    FILE_CLOSE_ERROR      = -5,
    DIRECTORY_OPEN_ERROR  = -6,
    DIRECTORY_CREATE_ERROR= -7,
    ERROR_ANNOTATE        = -8,
    INIT_ERROR            = -9 
};

#endif /* _ERROR__ */
