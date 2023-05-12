#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "sleepingProcesses.h"
  
asmlinkage long sys_listSleepingProcesses(const char __user *buf, int size) {
	struct task_struct *proces;
	unsigned char kbuf[256];
	int bufsz;
	int index = 0;
	/* Find the process */
	for_each_process(proces) {
		if(proces->state == TASK_INTERRUPTIBLE || proces->state == TASK_UNINTERRUPTIBLE) {
			kbuf[index] = (long)task_pid_nr(proces);
			index = index + 1;
		}
	}
	
	bufsz = sizeof(kbuf);	/* User buffer is too small */
	if(bufsz > size){
	return -1;
	}

	/* Processes not found */
	if(index == 0) {
		return 0;	
	}

	/* success */
	copy_to_user((void*)buf, (void*)kbuf, bufsz);

	return index;
}

