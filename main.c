#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void strip(char *array) {
    if (array == NULL) {
        perror("array is null");
        exit(-99);
    }

    int len = strlen(array), x = 0;

    while (array[x] != '\0' && x < len) {
        if (array[x] == '\r') {
            array[x] = '\0';
        } else if (array[x] == '\n') {
            array[x] = '\0';
        }
        x++;
    }
}

int main(int argc, char** argv) {
    char config[100];
    strcpy(config, getenv("HOME"));
    strcat(config, "/.config/trash/config");
    char dir[100];
    strcpy(dir, getenv("HOME"));
    strcat(dir, "/.config/trash");
    char trash[100];
    strcpy(trash, getenv("HOME"));
    strcat(trash, "/.local/share/Trash/files");

    FILE * file;
    struct stat st = {0};

    if(stat("trash", &st) == -1) {
        mkdir(dir, 0777);
    }

    file = fopen(config, "r+");
    if(file == NULL) {
        file = fopen(config, "w+");
        fprintf(file, "%s\n", trash);
    } else {
        fgets(trash, 100, file);
        strip(trash);
    }

    if(argv[2] != NULL) {
        printf("Trash:\tInvalid command\nUse 'trash --help' for help\n");
    } else if(argv[1] != NULL && ((strcmp(argv[1], "-s") == 0) || strcmp(argv[1], "--set") == 0)) {//set trash dest
        if(stat(argv[2], &st) == -1) {
            printf("Trash: given path to trash: %s is not valid", argv[2]);
        } else {
            rewind(file);
            fputs(argv[2], file);
        }
    } else if(argv[1] != NULL && ((strcmp(argv[1], "-e") == 0) || strcmp(argv[1], "--empty") == 0)) {//empty trash
        char rm[100];
        strcpy(rm, "rm -rf ");
        strcat(rm, trash);
        system(rm);
        mkdir(trash, 0777);
        printf("Trash: trash emptied\n");
    } else if(argv[1] != NULL && ((strcmp(argv[1], "-r") == 0) || strcmp(argv[1], "--reset") == 0)) {//reset trash directory
        rewind(file);
        strcpy(trash, getenv("HOME"));
        strcat(trash, "/.local/share/Trash/files");
        fputs(trash, file);
    } else if(argv[1] != NULL && ((strcmp(argv[1], "-d") == 0) || strcmp(argv[1], "--directory") == 0)) {//print current trash dest
        printf("Trash: current trash destination - %s\n", trash);
    } else if(argv[1] != NULL && ((strcmp(argv[1], "-l") == 0) || strcmp(argv[1], "--list") == 0)) {//print current trash contents
        char list[100];
        strcpy(list, "ls -Al ");
        strcat(list, trash);
        strcat(list, " | grep -v '^total'");
        printf("Trash list:\n\n");
        system(list);
    } else if((argv[1] == NULL) || (strcmp(argv[1], "--help") == 0)) {//print help message
        printf("Trash: help\n"
               "Usage: trash [DIRECTORY -OR- OPTION]\n"
               "A utility for sending files to the trash and emptying the trash from the terminal.\n"
               "Password may be needed to send write protected files to the trash\n\n"
               "Options:\n"
               "\t-l, --list\tList the contents of the trash"
               "\t-e, --empty\tEmpty the trash.\n"
               "\t-s, --set\tSet the trash location(For if you trash location is something other than ~/.local/share/Trash/files).\n"
               "\t-r, --reset\tReset the trash location back to ~/.local/share/Trash/files.\n"
               "\t-d, --directory\tDisplay the current location of the trash for this app.\n"
               "\t    --help\tDisplay this help text.\n"
               "\t    --version\tDisplay the version information\n");
    } else if(argv[1] != NULL && ((strcmp(argv[1], "--version") == 0))) {//print version number
        printf("Trash:\tversion 1.0(probably forever lol)\n"
               "\t GitHub for trash: blah\n");
    } else if(argv[1] != NULL && stat(argv[1], &st) != -1) {//trash
        char mv[100];
        strcpy(mv, "sudo mv -f --backup=t ");
        strcat(mv, argv[1]);
        strcat(mv, " ");
        strcat(mv, trash);
        system(mv);
        printf("Trash:\tsent %s to trash\n", argv[1]);
    } else {
        printf("Trash:\tInvalid command\nUse 'trash --help' for help\n");
    }
}