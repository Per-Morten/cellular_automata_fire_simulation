#pragma once
#include <cp_cell.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct
{
    size_t   count;
    cp_cell* read;
    cp_cell* write;
    sem_t*   wait_sync; // Wait on this variable to sync.
    sem_t*   finish_sync; // Signal this variable when finished to sync.
    bool*    should_continue;
} cpu_thread_args;
