//
// Created by Антон Миклис on 23.04.22.
//

#include "help.h"

void sleep_random(int max){
    if(max > 0){
        srand(getpid());
        usleep(rand() % max * 1000);
    }
}

FILE* create_file(){
    FILE *fPtr;
    fPtr = fopen("proj2.out", "w");
    if (fPtr == NULL){
        fprintf(stderr, "Error occurred while creating output file.\n");
    }
    return fPtr;
}

Arguments* readArguments(int argc, char **argv){
    if (argc != 5){
        fprintf(stderr, "Wrong number of arguments.\n");
        return NULL;
    }
    Arguments *args = malloc(sizeof(*args));
    if (args == NULL){
        fprintf(stderr, "Error occurred while allocating memory for arguments structure.\n");
        return NULL;
    }
    char *ptr;
    args->oxygenNumber = strtol(argv[1], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    args->hydrogenNumber = strtol(argv[2], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    args->waitTime = strtol(argv[3], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    args->createTime = strtol(argv[4], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    if (args->createTime > 1000 || args->waitTime > 1000  || args->createTime < 0 || args->waitTime < 0){
        fprintf(stderr, "3rd or 4th argument is out of bounds.\n");
        return NULL;
    }
    if (args->oxygenNumber < 0 || args->hydrogenNumber < 0){
        fprintf(stderr, "1st or 2nd argument is negative.\n");
        return NULL;
    }
    return args;
}

int moleculesToCreate(int oxygen, int hydrogen){
    int moleculeCount = 0;
    while (hydrogen >= 2 && oxygen >= 1){
        moleculeCount++;
        hydrogen -= 2;
        oxygen -= 1;
    }
    return moleculeCount;
}
