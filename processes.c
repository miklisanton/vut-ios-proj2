//
// Created by Антон Миклис on 22.04.22.
//

#include "processes.h"
int oxygenProcess(Arguments args){
    pid_t oxygenPid;
    for (int i = 0; i < args.oxygenNumber; ++i) {
        if((oxygenPid = fork()) == 0){
            //oxygen atom creation
            sem_wait(semaphores.mutex);
            fprintf(sharedData->output, "%d: O %d: started\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sleep_random(args.waitTime);
            sharedData->lineCount++;
            sharedData->oxygenCount++;

            fprintf(sharedData->output, "%d: O %d: going to queue\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sharedData->lineCount++;

            if(sharedData->hydrogenCount >= 2){
                sem_post(semaphores.hydrogenQueue);
                sem_post(semaphores.hydrogenQueue);
                sharedData->hydrogenCount -= 2;
                sem_post(semaphores.oxygenQueue);
                sharedData->oxygenCount--;
            }else{
                sem_post(semaphores.mutex);
            }
            sem_wait(semaphores.oxygenQueue);
            //creating molecule
            fprintf(sharedData->output, "%d: O %d: creating molecule %d\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);
            sleep_random(args.createTime);
            fprintf(sharedData->output, "%d: O %d: molecule %d created\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);

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
            sem_post(semaphores.mutex);

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

            exit(0);
        } else if(oxygenPid < 0){
            fprintf(stderr, "Oxygen process error.");
            return -1;
        }
        //parent
    }
}

int hydrogenProcess(Arguments args){
    pid_t hydrogenPid;
    for (int i = 0; i < args.hydrogenNumber; ++i) {
        if((hydrogenPid = fork()) == 0){
            //hydrogen atom creation
            sem_wait(semaphores.mutex);
            fprintf(sharedData->output, "%d: H %d: started\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sleep_random(args.waitTime);
            sharedData->lineCount++;
            sharedData->hydrogenCount++;

            fprintf(sharedData->output, "%d: H %d: going to queue\n", sharedData->lineCount, i + 1);
            fflush(sharedData->output);
            sharedData->lineCount++;

            if(sharedData->hydrogenCount >= 2 && sharedData->oxygenCount >= 1){
                sem_post(semaphores.hydrogenQueue);
                sem_post(semaphores.hydrogenQueue);
                sharedData->hydrogenCount -= 2;
                sem_post(semaphores.oxygenQueue);
                sharedData->oxygenCount--;
                //todo remove barrier
            }else{
                sem_post(semaphores.mutex);
            }
            sem_wait(semaphores.hydrogenQueue);

            //creating molecule
            fprintf(sharedData->output, "%d: H %d: creating molecule %d\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);
            sleep_random(args.createTime);
            //molecule created
            fprintf(sharedData->output, "%d: H %d: molecule %d created\n",
                    sharedData->lineCount++,i + 1, sharedData->moleculeCount);
            fflush(sharedData->output);

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
            exit(0);
        } else if(hydrogenPid < 0){
            fprintf(stderr, "Hydrogen process error.");
            return -1;
        }
        //parent
    }
}
