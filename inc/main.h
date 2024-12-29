#ifndef MAIN
#define MAIN

#include "log.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define BUILD 3

#ifdef DEBUG
#define NO_IMPL LOG_MESG(LOG_FATAL, "no implementation");
#endif

#define CLOCK_DIVIDER 4

// Clock-Speed: 4.194304MHz
#define CLOCK_SPEED (4194304 / CLOCK_DIVIDER) // Hz
#define MS_ELLAPSED_FOR_CLOCK (0.0002384185791015625 * CLOCK_DIVIDER)

#define BASE_PATH_MAX 512
#define ROM_NAME_MAX 2048

extern char *main_base_path;

#endif