#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "process_util.h"

void set_cpu(int pid, int cpu_num){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu_num, &mask);

	int ret = sched_setaffinity(pid, sizeof(mask), &mask);
	if(ret < 0){
		printf("setting cpu err\n");
		exit(0);
	}
}

void run_unitime(){
	volatile unsigned long i;
	for(i=0;i<1000000UL;i++);
}

void stop_process(int pid){
	struct sched_param param;
	param.sched_priority = 1;
	sched_setscheduler(pid, SCHED_FIFO, &param);
}

void run_process(int pid){
	struct sched_param param;
	param.sched_priority = 99;
	sched_setscheduler(pid, SCHED_FIFO, &param);
}

int fork_process(process p){
	long start_t = syscall(334);
	int pid = fork();
	if(pid == 0){ // child
		set_cpu(getpid(), 1); // let child use cpu 1
		while(p.exec_t --)
			run_unitime();
		long end_t = syscall(334);
		long st_sec = start_t / (long)1e9, st_nsec = start_t % (long)1e9;
		long ed_sec = end_t / (long)1e9, ed_nsec = end_t % (long)1e9;

		char info_opt[256];
		sprintf(info_opt, "[Project1] %d %ld.%09ld %ld.%09ld\n", getpid(), st_sec, st_nsec, ed_sec, ed_nsec);
		syscall(333, info_opt);
		//fprintf(stderr, "%s", info_opt);

		/*
		cpu_set_t mask;
		CPU_ZERO(&mask);
		sched_getaffinity(getpid(), sizeof(mask), &mask);
		for(int i = 0; i < 10; i++)
			if(CPU_ISSET(i, &mask))
				fprintf(stderr, "The child %d use cpu %d\n", getpid(), i);
		*/
		exit(0);
	}
	else{ // parent
		// set_cpu(pid, 1); // let child use cpu 1
		stop_process(pid);
		fprintf(stdout, "%s %d\n", p.name, pid);
		fflush(stdout);
		return pid;	
	}
}
