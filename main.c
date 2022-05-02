#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processes.h"
#include "resources.h"
#include "help.h"
#include <sys/wait.h>

int main(int argc, char **argv) {
    Arguments *args = readArguments(argc, argv);
    if(args == NULL){
        goto main_free_args;
    }
    fflush(stdout);
    FILE *fPtr = create_file();
    if(fPtr == NULL){
        goto main_fclose;
    }
    if(initShared(fPtr, *args) == -1){
        goto main_clean_shared;
    }
    if(initSemaphores() == -1){
        goto main_destroy_semaphores;
    }
    if(hydrogenProcess(*args, fPtr) == -1){
        goto main_destroy_semaphores;
    }
    if(oxygenProcess(*args, fPtr) == -1){
        goto main_destroy_semaphores;
    }
    while(wait(NULL)>=0);

    //exit default
    destroySemaphores();
    cleanShared();
    fclose(fPtr);
    free(args);
    return 0;

    //exit when error occurred
    main_destroy_semaphores: destroySemaphores();
    main_clean_shared: cleanShared();
    main_fclose: fclose(fPtr);
    main_free_args: free(args);
    return 1;
}
