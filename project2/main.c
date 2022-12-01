#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define __NR_get_task_mm 334
#define __NR_get_phys_addr 335

// char *global_str = "Global variable char *";
// char *BSS_str;

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
    // // dynamically allocated variable(s) in main
    // char *heap_str = (char *) malloc(sizeof(char) * 100);
    // // local variables in main()
    // char *str = "main";
    // strncpy(heap_str, str, 5);
    // unsigned long phys_addr;
    // unsigned long *addr_ptr = &phys_addr;

    pid_t child_pid, w;
    int wait_status;
    
    child_pid = fork();
    if (child_pid == -1) {
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        printf("Child process PID is %d\n", (int) getpid());
        sleep(2);
        long l = get_task_mm_syscall();
        printf("System call returned %ld\n", l);
    } else {
        printf("Parent process PID is %d\n", (int) getpid());
        sleep(1);
        long l = get_task_mm_syscall();
        printf("System call returned %ld\n", l);
        do {
            w = waitpid(child_pid, &wait_status, WUNTRACED);

        } while (!WIFEXITED(wait_status) && !WIFSIGNALED(wait_status));
    }

    return 0;
}
