#pragma once

#include <stdbool.h>

#include "thread.h"

bool init_scheduler(void);

void schedule(void);
void kick_off(void);

void enqueue_ready_list(thread_struct_t* thread);
thread_struct_t* pop_ready_list(void);
