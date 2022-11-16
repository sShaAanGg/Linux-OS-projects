#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/syscalls.h>
// #define __NR_get_phys_addr 335

unsigned long get_phys_addr_syscall(unsigned long virt_addr);

SYSCALL_DEFINE0(get_task_mm)
{
    struct pid *cur_pid = find_get_pid(current->pid);
    struct task_struct *cur_task = NULL;
    struct mm_struct *cur_mm = NULL;
    struct vm_area_struct *cur_mmap = NULL;
    struct mm_struct *mm;
    unsigned long vm_area_size;
    unsigned long phys_addr;

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
    phys_addr = get_phys_addr_syscall(cur_mm->start_code);
    printk("end_code of mm_struct is %lx\n", cur_mm->end_code);
    
    printk("start_data of mm_struct is %lx\n", cur_mm->start_data);
    phys_addr = get_phys_addr_syscall(cur_mm->start_data);
    printk("end_data of mm_struct is %lx\n", cur_mm->end_data);
    
    printk("start_brk of mm_struct is %lx\n", cur_mm->start_brk);
    phys_addr = get_phys_addr_syscall(cur_mm->start_brk);
    printk("brk of mm_struct is %lx\n\n", cur_mm->brk);
    
    printk("mmap_base of mm_struct is %lx\n\n", cur_mm->mmap_base);
    phys_addr = get_phys_addr_syscall(cur_mm->mmap_base);
    
    printk("start_stack of mm_struct is %lx\n", cur_mm->start_stack);
    phys_addr = get_phys_addr_syscall(cur_mm->start_stack);
    printk("arg_start of mm_struct is %lx\n", cur_mm->arg_start);
    printk("arg_end of mm_struct is %lx\n\n", cur_mm->arg_end);

    printk("env_start of mm_struct is %lx\n", cur_mm->env_start);
    printk("env_end of mm_struct is %lx\n\n", cur_mm->env_end);

    while (cur_mmap->vm_next != NULL) // iterate through the list of VMAs
    {
        vm_area_size = cur_mmap->vm_end - cur_mmap->vm_start;
        printk("vm_start: %lx    vm_end: %lx    size: %lx\n", cur_mmap->vm_start, cur_mmap->vm_end, vm_area_size);
        phys_addr = get_phys_addr_syscall(cur_mmap->vm_start);
        cur_mmap = cur_mmap->vm_next;
    }
    vm_area_size = cur_mmap->vm_end - cur_mmap->vm_start;
    printk("vm_start: %lx    vm_end: %lx    size: %lx\n", cur_mmap->vm_start, cur_mmap->vm_end, vm_area_size);
    phys_addr = get_phys_addr_syscall(cur_mmap->vm_start);

    atomic_dec_and_test(&mm->mm_users);
    return 0;
}

unsigned long get_phys_addr_syscall(unsigned long virt_addr)
{
    pgd_t *pgd; // entry of page global directory
    p4d_t *p4d;
    pud_t *pud; // entry of page upper directory
    pmd_t *pmd; // entry of page middle directory
    pte_t *pte; // entry of page table
    unsigned long phys_addr = 0;
    unsigned long pg_addr = 0;
    unsigned long pg_offset = 0;

    pgd = pgd_offset(current->mm, virt_addr);
    if (pgd_none(*pgd)) {
        return -1;
    }

    p4d = (p4d_t *) pgd;
    pud = pud_offset(p4d, virt_addr);
    if (pud_none(*pud)) {
        return -1;
    }

    pmd = pmd_offset(pud, virt_addr);
    if (pmd_none(*pmd)) {
        return -1;
    }

    pte = pte_offset_kernel(pmd, virt_addr);
    if (pte_none(*pte)) {
        return -1;
    }

    pg_addr = pte_val(*pte) & PTE_PFN_MASK & PAGE_MASK;
    pg_offset = virt_addr & ~PAGE_MASK;
    phys_addr = pg_addr | pg_offset;

    printk("virtual adddress: %lx    physical address: %lx\n", virt_addr, phys_addr);
    return phys_addr;
}
