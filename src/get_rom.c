#include "get_rom.h"
#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
    #include <dirent.h> 
#elifdef _WIN32
    #include <windows.h>
    #include <minwinbase.h>
#endif

uint16_t rom_count = 0;
char **available_rom_path = NULL;
char **available_rom_name = NULL;

static void get_rom_destroy(void) {
    if (available_rom_path != NULL) {
        for (uint16_t i = 0; i < rom_count; i++) {
            free(available_rom_path[i]);
        }

        free(available_rom_path);
    }

    if (available_rom_name != NULL) {
        for (uint16_t i = 0; i < rom_count; i++) {
            free(available_rom_name[i]);
        }

        free(available_rom_name);
    }

    return;
}

static void count_rom(char *bpath) {
    // TODO check la taille des fichiers (bo)

    rom_count = 0;

    #ifdef _WIN32
        WIN32_FIND_DATA fdFile;
        HANDLE hFind = NULL;

        char path[ROM_NAME_MAX];

        sprintf(path, "%s\\*.*", bpath);

        if((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE) {
            LOG_MESG(LOG_FATAL, "Path not found %s", path);
            exit(EXIT_FAILURE);
        }

        do {
            if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
                sprintf(path, "%s\\%s", bpath, fdFile.cFileName);
                if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    count_rom(path);
                else {
                    char tmp_name_file[ROM_NAME_MAX];
                    strcpy(tmp_name_file, fdFile.cFileName);

                    const uint32_t file_name_size = strlen(tmp_name_file);

                    if (file_name_size + strlen(path) + 2 < ROM_NAME_MAX) {
                        const char *last = &(tmp_name_file[file_name_size - 2]);
                        if (!strncmp(last, "gb", 2))
                            rom_count++;
                    }
                }
            }
        } while(FindNextFile(hFind, &fdFile));

        FindClose(hFind);
    #elifdef __linux__
        DIR *d;
        struct dirent *dir;

        d = opendir(bpath);

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strlen(dir->d_name) < ROM_NAME_MAX - 1) {

                    char last[2];
                    memcpy(last, dir->d_name + strlen(dir->d_name) - 2, 2);

                    if (!strncmp(last, "gb", 2))
                        rom_count++;
                }
            }
            closedir(d);
        }
    #else
        #error "unknow target"
    #endif
}

static void fill_rom(char *bpath) {
    // TODO check la taille des fichiers (bo) + check malloc result

    rom_count = 0;

    #ifdef _WIN32
        WIN32_FIND_DATA fdFile;
        HANDLE hFind = NULL;

        char path[ROM_NAME_MAX];

        sprintf(path, "%s\\*.*", bpath);

        if((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE) {
            LOG_MESG(LOG_FATAL, "Path not found %s\n", path);
            exit(EXIT_FAILURE);
        }

        do {
            if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
                sprintf(path, "%s\\%s", bpath, fdFile.cFileName);

                if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    count_rom(path);
                else {
                    char tmp_name_file[ROM_NAME_MAX];
                    strcpy(tmp_name_file, fdFile.cFileName);

                    const uint32_t file_name_size = strlen(tmp_name_file);

                    if (strlen(path) + 2 < ROM_NAME_MAX) {
                        const char *last = &(tmp_name_file[file_name_size - 2]);
                        if (!strncmp(last, "gb", 2)) {
                            available_rom_path[rom_count] = malloc(sizeof(char) * (strlen(path) + 2));
                            strcpy(available_rom_path[rom_count], path);

                            available_rom_name[rom_count] = malloc(sizeof(char) * (file_name_size + 2));
                            strcpy(available_rom_name[rom_count], fdFile.cFileName);

                            rom_count++;
                        }
                    }
                }
            }
        } while(FindNextFile(hFind, &fdFile));

        FindClose(hFind);
    #elifdef __linux__
        DIR *d;
        struct dirent *dir;
        d = opendir(bpath);

        char path[ROM_NAME_MAX];

        sprintf(path, "%s/", bpath);

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                
                if (strlen(bpath) + strlen(dir->d_name) < ROM_NAME_MAX - 1) {

                    char tmp_name_file[ROM_NAME_MAX];

                    sprintf(path, "%s/%s", bpath, dir->d_name);
                    strcpy(tmp_name_file, dir->d_name);
                    uint32_t file_name_size = strlen(tmp_name_file);

                    char last[2];
                    memcpy(last, dir->d_name + strlen(dir->d_name) - 2, 2);

                    if (!strncmp(last, "gb", 2)){
                        available_rom_path[rom_count] = malloc(sizeof(char) * (strlen(path) + 2));
                        strcpy(available_rom_path[rom_count], path);

                        available_rom_name[rom_count] = malloc(sizeof(char) * (file_name_size + 2));
                        if (available_rom_name[rom_count] == NULL) {
                            LOG_MESG(LOG_FATAL, "No more memory");
                            exit(EXIT_FAILURE);
                        }
                        strcpy(available_rom_name[rom_count], dir->d_name);

                        rom_count++;
                    }
                }
            }
            closedir(d);
        }
    #else
        #error "unknow target"
    #endif
}

static void table_init(void) {
    char base_rom_directory[ROM_NAME_MAX];

    strcpy(base_rom_directory, main_base_path);
    strcat(base_rom_directory, "rom");

    count_rom(base_rom_directory);

    available_rom_path = malloc(sizeof(char *) * rom_count);

    if (available_rom_path == NULL) {
        LOG_MESG(LOG_FATAL, "No more memory");
        exit(EXIT_FAILURE);
    }

    for (uint16_t i = 0; i < rom_count; i++)
        available_rom_path[i] = NULL;

    available_rom_name = malloc(sizeof(char *) * rom_count);

    if (available_rom_name == NULL) {
        LOG_MESG(LOG_FATAL, "No more memory");
        exit(EXIT_FAILURE);
    }

    for (uint16_t i = 0; i < rom_count; i++)
        available_rom_name[i] = NULL;

    fill_rom(base_rom_directory);
}

void get_rom_init(void) {
    atexit(get_rom_destroy);
    table_init();

    #ifdef DEBUG
        LOG_MESG(LOG_DEBUG, " %"PRIu16" available counted rom", rom_count);
    #endif
}

uint16_t get_rom_number(void) {
    return rom_count;
}

void get_rom_next(char **rom_path, char **rom_name, uint8_t order) {
    static uint16_t rom_number = 0;

    if (order == GET_ROM_NEXT) {
        rom_number++;
        rom_number %= rom_count;
    } else if (order == GET_ROM_BACK) {
        if (rom_number == 0)
            rom_number = rom_count - 1;
        else
            rom_number--;
    }

    if (rom_path != NULL)
        *rom_path = available_rom_path[rom_number];
    
    if (rom_name != NULL)
        strcpy(*rom_name, available_rom_name[rom_number]);
}