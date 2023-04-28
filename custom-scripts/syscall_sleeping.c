#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

#define SYSCALL_SLEEPINGPROCESSES	386

int main(int argc, char** argv){  
	char buf[12400];
	long ret;
	
	printf("Invoking 'listSleepingProcesses' system call.\n");
         
	ret = syscall(SYSCALL_SLEEPINGPROCESSES, buf, sizeof(buf)); 
         
	if(ret > 0) {
		/* Success, show the process info. */
		printf("%s\n", buf);
	}
	else {
		printf("System call 'listSleepingProcess' did not execute as expected error %d\n", ret);
	}
          
	return 0;
}
