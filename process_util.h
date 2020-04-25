

typedef struct {
	char name[32];
	int pid;
	int ready_t, exec_t;
}process;

// run unittime t times 
void run_unitime();

// create a new task
int fork_process(process p);

// set process's cpu
void set_cpu(int pid, int cpu_num);

// control process to run or not
void stop_process(int pid);
void run_process(int pid);

