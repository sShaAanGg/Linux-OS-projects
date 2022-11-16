#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/pgtable.h>

SYSCALL_DEFINE2(get_phys_addr, unsigned long, virt_addr, unsigned long *, addr_ptr)
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

    // printk("virtual adddress: %lx    physical address: %lx\n", virt_addr, phys_addr);
    *addr_ptr = phys_addr;
    return 0;
}
