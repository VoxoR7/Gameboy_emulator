#include "get_rom.h"
#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __WIN
    #include <windows.h>
    #include <minwinbase.h>
#else
   #include <dirent.h> 
#endif

#define GET_ROM_MAX_PATH 2048

uint16_t rom_count = 0;
char **available_rom_path = NULL;
char **available_rom_name = NULL;

void get_rom_destroy( void) {

    if ( available_rom_path != NULL) {

        for ( uint16_t i = 0; i < rom_count; i++) {

            if ( available_rom_path[i] != NULL)
                free( available_rom_path[i]);
        }

        free( available_rom_path);
    }

    if ( available_rom_name != NULL) {

        for ( uint16_t i = 0; i < rom_count; i++) {

            if ( available_rom_name[i] != NULL)
                free( available_rom_name[i]);
        }

        free( available_rom_name);
    }

    return;
}

void get_rom_count( char *bpath) {

    // TODO check la taille des fichiers (bo)

    rom_count = 0;

    #ifdef __WIN
        WIN32_FIND_DATA fdFile;
        HANDLE hFind = NULL;

        char path[GET_ROM_MAX_PATH];

        sprintf( path, "%s\\*.*", bpath);

        if(( hFind = FindFirstFile( path, &fdFile)) == INVALID_HANDLE_VALUE) {

            #ifdef __DEBUG
                printf( "[FATAL] Path not found %s\n", path);
            #endif
            exit( EXIT_FAILURE);
        }

        do {
            if( strcmp( fdFile.cFileName, ".") != 0 && strcmp( fdFile.cFileName, "..") != 0) {

                sprintf( path, "%s\\%s", bpath, fdFile.cFileName);

                if( fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    get_rom_count( path);
                else {

                    // fprintf( stdout, "[TEST] file name : %s\\%s\n", path, fdFile.cFileName);

                    char tmp_name_file[GET_ROM_MAX_PATH];
                    strcpy( tmp_name_file, fdFile.cFileName);

                    const uint32_t file_name_size = strlen( tmp_name_file);

                    if ( file_name_size + strlen( path) + 2 < GET_ROM_MAX_PATH) {

                        const char *last = &(tmp_name_file[ file_name_size - 2]);

                        if ( !strncmp( last, "gb", 2))
                            rom_count++;
                    }
                }
            }
        } while( FindNextFile( hFind, &fdFile));

        FindClose(hFind);
    #elif __LIN
        DIR *d;
        struct dirent *dir;

        d = opendir(bpath);

        if (d) {
            while ( (dir = readdir(d)) != NULL) {

                if ( strlen( dir->d_name) < GET_ROM_MAX_PATH - 1) {

                    char last[2];
                    strncpy( last, dir->d_name + strlen( dir->d_name) - 2, 2);

                    if ( !strncmp( last, "gb", 2))
                        rom_count++;
                }
            }
            closedir(d);
        }
    #endif
}

void get_rom_fill( char *bpath) {

    // TODO check la taille des fichiers (bo) + check malloc result

    rom_count = 0;

    #ifdef __WIN
        WIN32_FIND_DATA fdFile;
        HANDLE hFind = NULL;

        char path[GET_ROM_MAX_PATH];

        sprintf( path, "%s\\*.*", bpath);

        if(( hFind = FindFirstFile( path, &fdFile)) == INVALID_HANDLE_VALUE) {

            #ifdef __DEBUG
                printf( "[FATAL] Path not found %s\n", path);
            #endif
            exit( EXIT_FAILURE);
        }

        do {
            if( strcmp( fdFile.cFileName, ".") != 0 && strcmp( fdFile.cFileName, "..") != 0) {

                sprintf( path, "%s\\%s", bpath, fdFile.cFileName);

                if( fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    get_rom_count( path);
                else {

                    // fprintf( stdout, "[TEST] file name : %s\\%s\n", path, fdFile.cFileName);

                    char tmp_name_file[GET_ROM_MAX_PATH];
                    strcpy( tmp_name_file, fdFile.cFileName);

                    const uint32_t file_name_size = strlen( tmp_name_file);

                    if ( strlen( path) + 2 < GET_ROM_MAX_PATH) {

                        const char *last = &(tmp_name_file[ file_name_size - 2]);

                        if ( !strncmp( last, "gb", 2)) {

                            available_rom_path[rom_count] = malloc( sizeof( char) * (strlen( path) + 2));
                            strcpy( available_rom_path[rom_count], path);

                            available_rom_name[rom_count] = malloc( sizeof( char) * (file_name_size + 2));
                            strcpy( available_rom_name[rom_count], fdFile.cFileName);

                            // fprintf( stdout, "[TEST] file name : %s (effective size %llu, malloced size %llu)\n", available_rom_path[rom_count], strlen( available_rom_path[rom_count]), strlen( path) + 2);

                            rom_count++;
                        }
                    }
                }
            }
        } while( FindNextFile( hFind, &fdFile));

        FindClose(hFind);
    #elif __LIN
        DIR *d;
        struct dirent *dir;
        d = opendir(bpath);

        char path[GET_ROM_MAX_PATH];

        sprintf( path, "%s/", bpath);

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                
                if ( strlen( bpath) + strlen( dir->d_name) < GET_ROM_MAX_PATH - 1) {

                    char tmp_name_file[GET_ROM_MAX_PATH];

                    sprintf( path, "%s/%s", bpath, dir->d_name);
                    strcpy(tmp_name_file, dir->d_name);
                    uint32_t file_name_size = strlen( tmp_name_file);

                    char last[2];
                    strncpy( last, dir->d_name + strlen( dir->d_name) - 2, 2);

                    if ( !strncmp( last, "gb", 2)){
                        available_rom_path[rom_count] = malloc( sizeof( char) * (strlen( path) + 2));
                        strcpy( available_rom_path[rom_count], path);

                        available_rom_name[rom_count] = malloc( sizeof( char) * (file_name_size + 2));
                        if ( available_rom_name[rom_count] == NULL) {

                            #ifdef __DEBUG
                                fprintf( stderr, "[FATAL] no more memory %s:%d", __FILE__, __LINE__);
                            #endif

                            exit( EXIT_FAILURE);
                        }
                        strcpy( available_rom_name[rom_count], dir->d_name);

                        // fprintf( stdout, "[TEST] file name : %s (effective size %lu, malloced size %lu)\n", available_rom_path[rom_count], strlen( available_rom_path[rom_count]), strlen( path) + 2);

                        rom_count++;
                    }
                }
            }
            closedir(d);
        }
    #endif
}

void get_rom_table_init( void) {

    char base_rom_directory[GET_ROM_MAX_PATH];

    strcpy( base_rom_directory, main_base_path);
    strcat( base_rom_directory, "rom");

    get_rom_count( base_rom_directory);

    available_rom_path = malloc( sizeof( char *) * rom_count);

    if ( available_rom_path == NULL) {

        #ifdef __DEBUG
            fprintf( stderr, "[FATAL] no more memory %s:%d", __FILE__, __LINE__);
        #endif

        exit( EXIT_FAILURE);
    }

    for ( uint16_t i = 0; i < rom_count; i++)
        available_rom_path[i] = NULL;

    available_rom_name = malloc( sizeof( char *) * rom_count);

    if ( available_rom_name == NULL) {

        #ifdef __DEBUG
            fprintf( stderr, "[FATAL] no more memory %s:%d", __FILE__, __LINE__);
        #endif

        exit( EXIT_FAILURE);
    }

    for ( uint16_t i = 0; i < rom_count; i++)
        available_rom_name[i] = NULL;

    get_rom_fill( base_rom_directory);
}

extern void get_rom_init( void) {

    atexit( get_rom_destroy);

    get_rom_table_init();

    #ifdef __DEBUG
        fprintf( stdout, "[INFO] %"PRIu16" available counted rom\n", rom_count);
    #endif
}

extern uint16_t get_rom_number( void) {

    return rom_count;
}

extern void get_rom_next( char **rom_path, char **rom_name, uint8_t order) {

    static uint16_t rom_number = 0;

    if ( order == NEXT) {

        rom_number++;
        rom_number %= rom_count;
    } else if ( order == BACK) {

        if ( rom_number == 0)
            rom_number = rom_count - 1;
        else
            rom_number--;
    }

    if ( rom_path != NULL)
        *rom_path = available_rom_path[rom_number];
    
    if ( rom_name != NULL)
        strcpy( *rom_name, available_rom_name[rom_number]);
}