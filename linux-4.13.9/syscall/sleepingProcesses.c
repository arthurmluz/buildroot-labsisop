#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "sleepingProcesses.h"
  
asmlinkage long sys_listSleepingProcesses(const char __user *buf, int size) {
	struct task_struct *proces;
	unsigned char kbuf[256];
	int bufsz;
	int ret;

	/* Find the process */
	for_each_process(proces) {
		if( ((long)proces->state) == 1 ) {
			/* Print the process info to the buffer */
			snprintf(kbuf, sizeof(kbuf), "Process: %s\n PID_Number: %ld\n Process State: %ld\n", 
					proces->comm, 
					(long)task_pid_nr(proces), 
					(long)proces->state 
					);
			bufsz = strlen(kbuf)+1;

			/* User buffer is too small */
			if(bufsz > size){
				return -bufsz;
			}

			/* success */
			ret = copy_to_user((void*)buf, (void*)kbuf, bufsz);

			return bufsz - ret;
		}
	}

	/* Process not found */
	return -2;	
}

