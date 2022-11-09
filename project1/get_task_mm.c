#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm_types.h>
#include <linux/syscalls.h>

asmlinkage long sys_get_task_mm(void)
{
    struct pid *cur_pid = find_get_pid(current->pid);
    struct task_struct *cur_task = NULL;
    struct mm_struct *cur_mm = NULL;
    struct vm_area_struct *cur_mmap = NULL;
    struct mm_struct *mm;

    cur_task = pid_task(cur_pid, PIDTYPE_PID);

    mm = cur_task->mm;
    if (mm)
    {
        if (cur_task->flags & PF_KTHREAD)
            mm = NULL;
        else
            atomic_inc(&mm->mm_users);
    }
    cur_mm = mm;
    cur_mmap = cur_mm->mmap;

    printk("start_code of mm_struct is %lx\n", cur_mm->start_code);
    printk("end_code of mm_struct is %lx\n", cur_mm->end_code);
    
    printk("start_data of mm_struct is %lx\n", cur_mm->start_data);
    printk("end_data of mm_struct is %lx\n", cur_mm->end_data);
    
    printk("start_brk of mm_struct is %lx\n", cur_mm->start_brk);
    printk("brk of mm_struct is %lx\n\n", cur_mm->brk);
    
    printk("mmap_base of mm_struct is %lx\n\n", cur_mm->mmap_base);
    
    printk("start_stack of mm_struct is %lx\n", cur_mm->start_stack);
    printk("arg_start of mm_struct is %lx\n", cur_mm->arg_start);
    printk("arg_end of mm_struct is %lx\n\n", cur_mm->arg_end);

    printk("env_start of mm_struct is %lx\n", cur_mm->env_start);
    printk("env_end of mm_struct is %lx\n\n", cur_mm->env_end);

    unsigned long vm_area_size;
    while (cur_mmap->vm_next != NULL) // iterate through the list of VMAs
    {
        /* code */
        printk("vm_start of vm_area_struct is %lx\n", cur_mmap->vm_start);
        printk("vm_end of vm_area_struct is %lx\n", cur_mmap->vm_end);
        vm_area_size = cur_mmap->vm_end - cur_mmap->vm_start;
        printk("the size of the vm_area (vm_end - vm_start) is %lx\n\n", vm_area_size);
        cur_mmap = cur_mmap->vm_next;
    }
    printk("vm_start of vm_area_struct is %lx\n", cur_mmap->vm_start);
    printk("vm_end of vm_area_struct is %lx\n\n", cur_mmap->vm_end);
    vm_area_size = cur_mmap->vm_end - cur_mmap->vm_start;
    printk("the size of the vm_area (vm_end - vm_start) is %lx\n\n", vm_area_size);

    return 0;
}
