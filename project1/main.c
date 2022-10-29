#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

__thread int thread_i;
char *global_str = "Global variable char *";

// void break_point() {} // For gdb analyzing purpose

void *start_routine(void *arg)
{
    char *heap_str = (char *) malloc(sizeof(char) * 100);
    char *str;
    thread_i = *(int *) arg;
    // printf("The address of char *heap_str in %s: %p\n", "start_routine", &heap_str);
    // printf("%i\n", *(int *) arg);
    
    if (*(int *) arg == 1) {
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
    printf("The value of thread_i in %s: %d (address: %p)\n", str, thread_i, &thread_i);
    printf("The address of char *str in %s: %p\n", str, &str);
    printf("The address of char *heap_str in %s: %p\n", heap_str, &heap_str);
    printf("The address of global variable char *global_str: %p\n", &global_str);

    printf("\n");
    sleep(1);

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
    pthread_create(&t1, NULL, start_routine, (void *) arg1);
    pthread_create(&t2, NULL, start_routine, (void *) arg2);


    char *str = "main";
    printf("thread: main\n");
    printf("The value of thread_i in %s: %d (address: %p)\n", str, thread_i, &thread_i);
    printf("The address of char *str in %s: %p\n", str, &str);
    printf("\n");
    sleep(1);

    char *s;
    scanf("%s", s); // For gdb analyzing purpose
    // break_point(); // For gdb analyzing purpose

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
