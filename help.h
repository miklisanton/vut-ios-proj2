//
// Created by Антон Миклис on 23.04.22.
//

#ifndef VUT_IOS_PROJ2_HELP_H
#define VUT_IOS_PROJ2_HELP_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
    int oxygenNumber;
    int hydrogenNumber;
    int waitTime;   // time before going to queue in milliseconds
    int createTime; // maximum time in milliseconds needed to create a molecule
}Arguments;

void sleep_random(int max);
FILE* create_file();
Arguments* readArguments(int argc, char **argv);

#endif //VUT_IOS_PROJ2_HELP_H
