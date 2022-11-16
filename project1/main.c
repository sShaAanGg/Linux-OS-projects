#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#define __NR_get_task_mm 334
#define __NR_get_phys_addr 335

__thread int thread_i;
char *global_str = "Global variable char *";
char *BSS_str;

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

void *start_routine(void *arg)
{
    char *heap_str = (char *) malloc(sizeof(char) * 100);
    char *str;
    thread_i = *(int *) arg;
    unsigned long phys_addr;
    unsigned long *addr_ptr = &phys_addr;
        
    if (thread_i == 1) {
        str = "t1";
        strncpy(heap_str, str, 3);
        // heap_str = "t1";
    }
    else {
        str = "t2";
        strncpy(heap_str, str, 3);
        // heap_str = "t2";
    }
    
    printf("thread: %s\n", str);
    get_phys_addr_syscall((unsigned long) &thread_i, addr_ptr);
    printf("The value of thread_i in %s: %d (address: %p, pa: %lx)\n", str, thread_i, &thread_i, phys_addr);
    get_phys_addr_syscall((unsigned long) &str, addr_ptr);
    printf("The address of char *str in %s       [stack]: %p, pa: %lx\n", str, &str, phys_addr);
    get_phys_addr_syscall((unsigned long) heap_str, addr_ptr);
    printf("The value of char *heap_str in %s    [heap|shared_memory]: %p, pa: %lx\n", heap_str, heap_str, phys_addr);
    get_phys_addr_syscall((unsigned long) &global_str, addr_ptr);
    printf("The address of global variable char *global_str: %p, pa: %lx\n", &global_str, phys_addr);
    get_phys_addr_syscall((unsigned long) &BSS_str, addr_ptr);
    printf("The address of uninitialized variable char *BSS_str: %p, pa: %lx\n", &BSS_str, phys_addr);
    printf("\n");
    sleep(1);

    if (thread_i == 2)
        free(heap_str);
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    int *arg1, *arg2;
    int i1 = 1, i2 = 2;
    arg1 = &i1, arg2 = &i2;
    unsigned long phys_addr;
    unsigned long *addr_ptr = &phys_addr;

    pthread_create(&t1, NULL, start_routine, (void *) arg1);
    sleep(1);
    pthread_create(&t2, NULL, start_routine, (void *) arg2);
    sleep(1);

    // dynamically allocated variable(s) in main
    char *heap_str = (char *) malloc(sizeof(char) * 100);
    // local variables in main()
    char *str = "main";
    strncpy(heap_str, str, 5);

    printf("thread: main\n");
    get_phys_addr_syscall((unsigned long) &thread_i, addr_ptr);
    printf("The value of thread_i in %s: %d (va: %p, pa: %lx)\n", str, thread_i, &thread_i, phys_addr);
    get_phys_addr_syscall((unsigned long) &str, addr_ptr);
    printf("The address of char *str in %s    [stack]: %p, pa: %lx\n", str, &str, phys_addr); // str is a local variable
    get_phys_addr_syscall((unsigned long) heap_str, addr_ptr);
    printf("The value of char *heap_str in %s [heap]: %p, pa: %lx\n", heap_str, heap_str, phys_addr); // heap_str is a local variable; it's the area it points to allocated in heap
    get_phys_addr_syscall((unsigned long) &global_str, addr_ptr);
    printf("The address of global variable char *global_str: %p, pa: %lx\n", &global_str, phys_addr);
    get_phys_addr_syscall((unsigned long) &BSS_str, addr_ptr);
    printf("The address of uninitialized variable char *BSS_str: %p, pa: %lx\n", &BSS_str, phys_addr);
    printf("\n");

    long l;
    l = get_task_mm_syscall();
    printf("system call return %ld\n", l);
    
    char *s;
    scanf("%s", s); // For gdb analyzing purpose

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    free(heap_str);
    return 0;
}
