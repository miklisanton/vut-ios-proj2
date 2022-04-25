//
// Created by Антон Миклис on 22.04.22.
//

#include "resources.h"

SharedResources *sharedData;
Semaphores semaphores;

int initShared(FILE *fPtr, Arguments args){
    key_t key = ftok("../main.c", 1222);
    int memoryId = shmget(key, sizeof(SharedResources), IPC_CREAT | 0664);
    if(memoryId < 0){
        perror("shmget error.");
        return 1;
    }
    sharedData = shmat(memoryId, NULL, 0);
    if (sharedData == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }
    sharedData->hydrogenCount = 0;
    sharedData->oxygenCount = 0;
    sharedData->oxygenNumber = args.oxygenNumber;
    sharedData->hydrogenNumber = args.hydrogenNumber;
    sharedData->output = fPtr;
    sharedData->lineCount = 1;
    sharedData->memId = memoryId;
    sharedData->moleculeCount = 0;
    return 0;
}

void cleanShared(){
    int memId = sharedData->memId;
    shmdt(sharedData);
    shmctl(memId, IPC_RMID, 0);
}

int initBarrier(pthread_barrier_t *barrier, unsigned int count){
    pthread_barrierattr_t attributes;
    if((pthread_barrierattr_init(&attributes)) != 0){
        return -1;
    }
    pthread_barrierattr_setpshared(&attributes, PTHREAD_PROCESS_SHARED);
    if((pthread_barrier_init(barrier, &attributes, count)) != 0){
        return -1;
    }
    pthread_barrierattr_destroy(&attributes);
    return 0;
}

int initSemaphores(){
    semaphores.mutex = sem_open("xmikli05-mutex", O_CREAT | O_EXCL, 0644, 1);
    if(semaphores.mutex == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    semaphores.oxygenQueue = sem_open("xmikli05-hydrogen", O_CREAT | O_EXCL, 0644, 0);
    if(semaphores.oxygenQueue == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    semaphores.hydrogenQueue = sem_open("xmikli05-oxygen", O_CREAT | O_EXCL, 0644, 0);
    if(semaphores.hydrogenQueue == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    semaphores.barrier = sem_open("xmikli05-barrier", O_CREAT | O_EXCL, 0644, 0);
    if(semaphores.barrier == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    if(initBarrier(&semaphores.sBarrier, 3) == -1){
        perror("Failed to open barrier");
        return -1;
    }
    return 0;
}

void destroySemaphores(){
    sem_close(semaphores.hydrogenQueue);
    sem_unlink("xmikli05-hydrogen");

    sem_close(semaphores.oxygenQueue);
    sem_unlink("xmikli05-oxygen");

    sem_close(semaphores.mutex);
    sem_unlink("xmikli05-mutex");

    sem_close(semaphores.barrier);
    sem_unlink("xmikli05-barrier");

    pthread_barrier_destroy(&semaphores.sBarrier);
}


