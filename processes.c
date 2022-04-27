//
// Created by Антон Миклис on 22.04.22.
//
//todo oxygen/hydrogen not enough case

#include "processes.h"
int oxygenProcess(Arguments args, FILE *fPtr){
    pid_t oxygenPid;
    for (int i = 0; i < args.oxygenNumber; ++i) {
        if((oxygenPid = fork()) == 0){
            //oxygen atom creation
            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: O %d: started\n", sharedData->lineCount++, i + 1);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);

            sleep_random(args.waitTime);

            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: O %d: going to queue\n", sharedData->lineCount++, i + 1);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);

            sem_wait(semaphores.mutex);
            sharedData->oxygenQueueCount++;

            if(sharedData->hydrogenQueueCount >= 2){

                sem_post(semaphores.hydrogenQueue);
                sem_post(semaphores.hydrogenQueue);
                sharedData->hydrogenQueueCount -= 2;
                sem_post(semaphores.oxygenQueue);
                sharedData->oxygenQueueCount--;
            }else{
                sem_post(semaphores.mutex);
            }

            sem_wait(semaphores.oxygenQueue);
            //creating molecule
            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: O %d: creating molecule %d\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);
            sleep_random(args.createTime);

            // barrier
            sem_wait(barrier.mutex);
            sharedData->barrierCount++;
            if (sharedData->barrierCount == 3){
                sem_wait(barrier.turnstile2);
                sem_post(barrier.turnstile);
            }
            sem_post(barrier.mutex);

            sem_wait(barrier.turnstile);
            sem_post(barrier.turnstile);

            // critical point
            sharedData->moleculeCount++;

            sem_wait(barrier.mutex);
            sharedData->barrierCount--;
            if (sharedData->barrierCount == 0){
                sem_wait(barrier.turnstile);
                sem_post(barrier.turnstile2);
            }
            sem_post(barrier.mutex);

            sem_wait(barrier.turnstile2);
            sem_post(barrier.turnstile2);
            // end of barrier
            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: O %d: molecule %d created\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount - 1);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);
            sem_post(semaphores.mutex);

            exit(0);
        } else if(oxygenPid < 0){
            fprintf(stderr, "Oxygen process error.\n");
            return -1;
        }
        //parent
    }
    return 0;
}

int hydrogenProcess(Arguments args, FILE *fPtr){
    pid_t hydrogenPid;
    for (int i = 0; i < args.hydrogenNumber; ++i) {
        if((hydrogenPid = fork()) == 0){
            //hydrogen atom creation
            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: H %d: started\n", sharedData->lineCount++, i + 1);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);
            sleep_random(args.waitTime);

            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: H %d: going to queue\n", sharedData->lineCount++, i + 1);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);

            sem_wait(semaphores.mutex);
            sharedData->hydrogenQueueCount++;

            if(sharedData->hydrogenQueueCount >= 2 && sharedData->oxygenQueueCount >= 1){

                sem_post(semaphores.hydrogenQueue);
                sem_post(semaphores.hydrogenQueue);
                sharedData->hydrogenQueueCount -= 2;
                sem_post(semaphores.oxygenQueue);
                sharedData->oxygenQueueCount--;
            }else{
                sem_post(semaphores.mutex);
            }
            sem_wait(semaphores.hydrogenQueue);

            //creating molecule
            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: H %d: creating molecule %d\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);

            // barrier
            sem_wait(barrier.mutex);
            sharedData->barrierCount++;
            if (sharedData->barrierCount == 3){
                sem_wait(barrier.turnstile2);
                sem_post(barrier.turnstile);
            }
            sem_post(barrier.mutex);

            sem_wait(barrier.turnstile);
            sem_post(barrier.turnstile);

            // critical point

            sem_wait(barrier.mutex);
            sharedData->barrierCount--;
            if (sharedData->barrierCount == 0){
                sem_wait(barrier.turnstile);
                sem_post(barrier.turnstile2);
            }
            sem_post(barrier.mutex);

            sem_wait(barrier.turnstile2);
            sem_post(barrier.turnstile2);
            // end of barrier

            //molecule created
            sem_wait(semaphores.mutexPrint);
            fprintf(fPtr, "%d: H %d: molecule %d created\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount - 1);
            fflush(fPtr);
            sem_post(semaphores.mutexPrint);

            exit(0);
        } else if(hydrogenPid < 0){
            fprintf(stderr, "Hydrogen process error.\n");
            return -1;
        }
        //parent
    }
    return 0;
}
