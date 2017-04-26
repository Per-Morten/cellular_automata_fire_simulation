#pragma once
#include <cpu_cell.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct
{
    size_t count;
    cpu_cell** read;
    cpu_cell** write;
} cpu_double_buffer;

typedef struct
{
    bool*    should_continue;
    cpu_double_buffer* double_buffer;
    sem_t*   start_sync; // Wait on this variable to sync.
    sem_t*   finish_sync; // Signal this variable when finished to sync.
                         // Both of these also protect the double buffer and should continue
} cpu_simulation;

