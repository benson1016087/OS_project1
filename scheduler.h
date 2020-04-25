#include <stdio.h>
#include "process_util.h"

void scheduler(int p, process todo[], int process_num);

int get_next_process(process todo[], int policy, int last_idx);

int min(int a, int b);
