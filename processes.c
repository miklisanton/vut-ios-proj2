//
// Created by Антон Миклис on 22.04.22.
//

#include "processes.h"
int oxygenProcess(){
    pid_t oxygenPid;
    for (int i = 0; i < sharedData->oxygenNumber; ++i) {
        if((oxygenPid = fork()) == 0){
            //oxygen atom creation
            sem_wait(semaphores.mutex);
            fprintf(sharedData->output, "%d: O %d: started\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sharedData->lineCount++;
            sharedData->oxygenCount++;
            sleep_random(100);

            printf("zastryal?\n");
            fflush(stdout);
            printf("net\n");
            fflush(stdout);
            fprintf(sharedData->output, "%d: O %d: going to queue\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sharedData->lineCount++;

            if(sharedData->hydrogenCount >= 2){
                sem_post(semaphores.hydrogenQueue);
                sem_post(semaphores.hydrogenQueue);
                sharedData->hydrogenCount -= 2;
                sem_post(semaphores.oxygenQueue);
                sharedData->oxygenCount--;
                sharedData->moleculeCount++;
                //todo remove under barrier
            }else{
                sem_post(semaphores.mutex);
            }
            sem_wait(semaphores.oxygenQueue);
            //creating molecule
            fprintf(sharedData->output, "%d: O %d: creating molecule %d\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);
            sleep_random(100);
            fprintf(sharedData->output, "%d: O %d: molecule %d created\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);
            sem_post(semaphores.mutex);
            pthread_barrier_wait(&semaphores.sBarrier);
            exit(0);
        } else if(oxygenPid < 0){
            fprintf(stderr, "Oxygen process error.");
            return -1;
        }
        //parent
    }
}

int hydrogenProcess(){
    pid_t hydrogenPid;
    for (int i = 0; i < sharedData->hydrogenNumber; ++i) {
        if((hydrogenPid = fork()) == 0){
            //hydrogen atom creation
            sem_wait(semaphores.mutex);
            fprintf(sharedData->output, "%d: H %d: started\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sharedData->lineCount++;
            sharedData->hydrogenCount++;
            sleep_random(100);

            fprintf(sharedData->output, "%d: H %d: going to queue\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sharedData->lineCount++;

            if(sharedData->hydrogenCount >= 2 && sharedData->oxygenCount >= 1){
                sem_post(semaphores.hydrogenQueue);
                sem_post(semaphores.hydrogenQueue);
                sharedData->hydrogenCount -= 2;
                sem_post(semaphores.oxygenQueue);
                sharedData->oxygenCount--;
                sharedData->moleculeCount++; //todo remove under barrier
                //todo remove barrier
            }else{
                sem_post(semaphores.mutex);
            }
            sem_wait(semaphores.hydrogenQueue);

            //creating molecule
            fprintf(sharedData->output, "%d: H %d: creating molecule %d\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);
            sleep_random(100);
            fprintf(sharedData->output, "%d: H %d: molecule %d created\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);
            fprintf(stdout,"HAHA");
            fflush(stdout);
            pthread_barrier_wait(&semaphores.sBarrier);
            fprintf(stdout,"HAHA");
            fflush(stdout);
            exit(0);
        } else if(hydrogenPid < 0){
            fprintf(stderr, "Hydrogen process error.");
            return -1;
        }
        //parent
    }
}
