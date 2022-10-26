#include <linux/kernel.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main()
{
    long sys = syscall(333);

	printf("sys_hello() returned %ld\n", sys);
    return 0;
}

