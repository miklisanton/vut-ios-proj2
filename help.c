//
// Created by Антон Миклис on 23.04.22.
//

#include "help.h"

void sleep_random(int max){
    srand(getpid());
    usleep(rand() % max * 1000);
}

FILE* create_file(){
    FILE *fPtr;
    fPtr = fopen("proj2.out", "w");
    if (fPtr == NULL){
        fprintf(stderr, "Error occurred while creating output file.");
    }
    return fPtr;
}

Arguments* readArguments(int argc, char **argv){
    if (argc != 5){
        fprintf(stderr, "Wrong number of arguments.");
        return NULL;
    }
    Arguments *args = malloc(sizeof(*args));
    if (args == NULL){
        fprintf(stderr, "Error occurred while allocating memory for arguments structure.");
        return NULL;
    }
    char *ptr;
    args->oxygenNumber = strtoul(argv[1], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    args->hydrogenNumber = strtoul(argv[2], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    args->waitTime = strtoul(argv[3], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    args->createTime = strtoul(argv[4], &ptr, 10);
    if (*ptr != '\0'){
        fprintf(stderr, "Wrong argument.");
        return NULL;
    }
    if (args->createTime > 1000 || args->waitTime > 1000 ){
        fprintf(stderr, "3rd or 4th argument is out of bounds.");
        return NULL;
    }
    return args;
}
