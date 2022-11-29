#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define __NR_get_task_mm 334
#define __NR_get_phys_addr 335

long get_task_mm_syscall(void)
{
    return syscall(__NR_get_task_mm);
}

void get_phys_addr_syscall(unsigned long virt_addr, unsigned long *addr_ptr)
{
    long l = syscall(__NR_get_phys_addr, virt_addr, addr_ptr);
    if (l != 0) {
        printf("system call return %ld\n", l);
        exit(l);
    }
    return;
}

int main()
{

    return 0;
}
