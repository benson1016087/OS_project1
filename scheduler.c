#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/syscall.h>
//#include "process_util.h"
#include "scheduler.h"

/* for reminder:
#define PSJF 0
#define SJF 1
#define FIFO 2
#define RR 3
*/

int exec_num = 0; // the number of process that ready_t <= now_t
int finish_num = 0; // the number of finished process

int get_next_process(process todo[], int policy, int last_idx){
	if(policy <= 1){ // SJF or PSJF
		int min_time = 2147483247, min_idx = -1;
		for(int i = 0; i < exec_num; i++)
			if(todo[i].exec_t > 0)
				if(todo[i].exec_t < min_time){
					min_time = todo[i].exec_t;
					min_idx = i;
				}	
		return min_idx;
	}
	else if(policy == 2){
		for(int i = 0; i < exec_num; i++)
			if(todo[i].exec_t > 0)
				return i;
	}
	else{
		int next_idx;

		next_idx = (last_idx + 1) % exec_num;
		for(int t = 0; t < exec_num; t++){
			if(todo[next_idx].exec_t > 0){
				//fprintf(stderr, "select idx = %d\n", next_idx);
				return next_idx;
			}
			next_idx = (next_idx + 1) % exec_num;
		}
	}
	return -1;
}

void scheduler(int policy, process todo[], int process_num){
	set_cpu(getpid(), 0);
	int now_t = 0;
	int running_idx = -1; 
	int finish_t = -1; // the finish time of this process, not used in non-preemptive 

	while(1){
		/*
		if(running_idx >= 0)
			fprintf(stderr, "idx = %d, remiain %d unit time; finish_t = %d, now_t = %d; exec_num = %d, finish_num = %d\n", running_idx, todo[running_idx].exec_t, finish_t, now_t, exec_num, finish_num);
		*/
		// start of a unitime 
		if(finish_num == process_num)
			break;

		// if some process is ready, fork and execute it
		for(int i = 0; i < process_num; i++)
			if(todo[i].ready_t == now_t){
				//fprintf(stderr, "i = %d, now_t = %d\n", i, now_t);
				exec_num ++;
				todo[i].pid = fork_process(todo[i]);
			}

		// get next process's idx
		if(finish_num < exec_num && (policy == 0 || finish_t <= now_t)){
			running_idx = get_next_process(todo, policy, running_idx);
			if(running_idx >= 0){
				run_process(todo[running_idx].pid);

				// setting finish_t for RR & non-preemptive
				if(policy == 3)
					finish_t = now_t + min(todo[running_idx].exec_t, 500);
				else if(policy > 0) // non-preemptive
					finish_t = now_t + todo[running_idx].exec_t;
			}
		}
		
		run_unitime();
		now_t ++;

		// end of a unitime
		if(running_idx >= 0){
			todo[running_idx].exec_t --;
			if(todo[running_idx].exec_t == 0){
				//long t = syscall(334);
				//fprintf(stderr, "finish %d (pid = %d), now_t = %d; t = %ld\n", running_idx, todo[running_idx].pid, now_t, t);				
				
				finish_num ++;
			}
			if((policy == 3 && finish_t == now_t) || policy == 0)
				if(todo[running_idx].exec_t > 0)
					stop_process(todo[running_idx].pid);
		}
	}
	
	// wait for children
	int status;
	for(int i = 0; i < process_num; i++)
		wait(&status);
}

int min(int a, int b){
	return a < b ? a : b;
}
