//
// Created by Антон Миклис on 22.04.22.
//

#ifndef VUT_IOS_PROJ2_PROCESSES_H
#define VUT_IOS_PROJ2_PROCESSES_H
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include "help.h"
#include "resources.h"
int oxygenProcess(Arguments arguments);
int hydrogenProcess(Arguments arguments);

#endif //VUT_IOS_PROJ2_PROCESSES_H
