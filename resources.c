//
// Created by Антон Миклис on 22.04.22.
//

#include "resources.h"

SharedResources *sharedData;
Semaphores semaphores;
Barrier barrier;

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
    sharedData->moleculeCount = 1;
    return 0;
}

void cleanShared(){
    int memId = sharedData->memId;
    shmdt(sharedData);
    shmctl(memId, IPC_RMID, 0);
}

int initBarrier(){
    barrier.mutex = sem_open("xmikli05-barrier-mutex", O_CREAT | O_EXCL, 0644, 1);
    if(barrier.mutex == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    barrier.turnstile = sem_open("xmikli05-barrier-turnstile", O_CREAT | O_EXCL, 0644, 0);
    if(barrier.turnstile == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    barrier.turnstile2 = sem_open("xmikli05-barrier-turnstile2", O_CREAT | O_EXCL, 0644, 1);
    if(barrier.turnstile2 == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    sharedData->barrierCount = 0;
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
    semaphores.barrier = sem_open("xmikli05-barrier", O_CREAT | O_EXCL, 0644, 1);
    if(semaphores.barrier == (void *) -1){
        perror("Failed to open semaphore.");
        return -1;
    }
    if(initBarrier(3) == -1){
        perror("Failed to open barrier");
        return -1;
    }
    return 0;
}

void destroyBarrier(){
    sem_close(barrier.mutex);
    sem_unlink("xmikli05-barrier-mutex");

    sem_close(barrier.turnstile);
    sem_unlink("xmikli05-barrier-turnstile");

    sem_close(barrier.turnstile2);
    sem_unlink("xmikli05-barrier-turnstile2");
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

    destroyBarrier();
}


