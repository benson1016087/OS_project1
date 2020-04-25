#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "process_util.h"
#include "scheduler.h"

#define PSJF 0
#define SJF 1
#define FIFO 2
#define RR 3

int cmp(const void *a, const void *b) {
	return ((process *)a)->ready_t - ((process *)b)->ready_t;
}

int main(){
	char policy[5];
	int process_num, policy_num;
	scanf("%s%d", policy, &process_num);

	process todo[256]; // all the task that need to be done
	for(int i = 0; i < process_num; i++)
		scanf("%s %d %d", todo[i].name, &todo[i].ready_t, &todo[i].exec_t);
	// ensure that the order is according to ready_t
	qsort(todo, process_num, sizeof(process), cmp);

	if(!strcmp(policy, "FIFO"))
		policy_num = FIFO;
	else if (!strcmp(policy, "SJF"))
		policy_num = SJF;
	else if (!strcmp(policy, "PSJF"))
		policy_num = PSJF;
	else if(!strcmp(policy, "RR"))
		policy_num = RR;
	else{
		fprintf(stderr, "policy string error.\n");
		exit(0);
	}
	scheduler(policy_num, todo, process_num);
	return 0;
}


