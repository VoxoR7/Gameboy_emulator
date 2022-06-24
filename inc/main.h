#ifndef __MAIN__
#define __MAIN__

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define BUILD 2

#ifdef __DEBUG
	#define NO_IMPL fprintf( stderr, "[CRITICAL] : NO IMPLEMENTATION : %s:%d\n", __FILE__, __LINE__);
#endif

#define CLOCK_DIVIDER 4

// Clock-Speed: 4.194304MHz
#define CLOCK_SPEED (4194304 / CLOCK_DIVIDER) // Hz
#define MS_ELLAPSED_FOR_CLOCK (0.0002384185791015625 * CLOCK_DIVIDER)

#define BASE_PATH_MAX 512
#define ROM_NAME_MAX 2048

extern char *main_base_path;

#endif