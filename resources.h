//
// Created by Антон Миклис on 22.04.22.
//

#ifndef VUT_IOS_PROJ2_RESOURCES_H
#define VUT_IOS_PROJ2_RESOURCES_H
#include <stdio.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "help.h"
#include "pthread.h"

typedef struct{
    int oxygenCount;
    int hydrogenCount;
    int memId;
    int oxygenNumber;
    int hydrogenNumber;
    int lineCount;
    int moleculeCount;
    FILE *output;
}SharedResources;

typedef struct{
    sem_t *mutex;
    sem_t *oxygenQueue;
    sem_t *hydrogenQueue;
    sem_t *barrier;
    pthread_barrier_t sBarrier;
}Semaphores;

extern SharedResources *sharedData;
extern Semaphores semaphores;

int initShared(FILE *fPtr, Arguments args);
void cleanShared();
int initBarrier(pthread_barrier_t *barrier, unsigned int count);
int initSemaphores();
void destroySemaphores();

#endif //VUT_IOS_PROJ2_RESOURCES_H
