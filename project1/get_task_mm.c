#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm_types.h>

// #include <linux/syscalls.h>

asmlinkage long sys_get_task_mm(void)
{
    struct pid *cur_pid = find_get_pid(current->pid);
    struct task_struct *cur_task = NULL;
    struct mm_struct *cur_mm = NULL;
    // struct mm_struct *mm;

    cur_task = pid_task(cur_pid, PIDTYPE_PID);

    // mm = cur_task->mm;
    // if (mm)
    // {
    //     if (cur_task->flags & PF_KTHREAD)
    //         mm = NULL;
    //     else
    //         atomic_inc(&mm->mm_users);
    // }
    cur_mm = get_task_mm(cur_task);

    printf("start_code of mm_struct is %lx\n", cur_mm->start_code);
    printf("end_code of mm_struct is %lx\n", cur_mm->end_code);
    printf("start_data of mm_struct is %lx\n", cur_mm->start_data);
    printf("end_data of mm_struct is %lx\n", cur_mm->end_data);
    printf("start_brk of mm_struct is %lx\n", cur_mm->start_brk);
    printf("brk of mm_struct is %lx\n", cur_mm->brk);
    printf("start_stack of mm_struct is %lx\n", cur_mm->start_stack);
    printf("arg_start of mm_struct is %lx\n", cur_mm->arg_start);
    return 0;
}
