#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

__thread int thread_i;

void break_point() {} // For gdb analyzing purpose

void *start_routine(void *arg)
{
    char *str;
    // printf("%i\n", *(int *) arg);
    thread_i = *(int *) arg;
    if (*(int *) arg == 1)
        str = "t1";
    else
        str = "t2";
    
    printf("thread: %s\n", str);
    printf("thread_i in %s: %d (address: %p)\n", str, thread_i, &thread_i);
    printf("The address of char *str in %s: %p\n", str, &str);

    printf("\n");
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
    printf("thread_i in %s: %d (address: %p)\n", str, thread_i, &thread_i);
    printf("The address of char *str in %s: %p\n", str, &str);
    printf("\n");

    char *s;
    scanf("%s", s); // For gdb analyzing purpose
    break_point(); // For gdb analyzing purpose

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
