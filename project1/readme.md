## 多執行緒程式實作
### Environment
> Linux kernel version: 4.15.1
Distribution: Ubuntu 16.04.7
gcc: 5.4.0 
gdb: 8.3
### Kernel Space Code (New System Call)
#### 1. get_task_mm.c
引用自 [Process Descriptor - Understanding the Linux Kernel](https://www.oreilly.com/library/view/understanding-the-linux/0596002130/ch03s02.html)
> This is the role of the process descriptor — a **task_struct** type structure whose fields contain all the information related to a single process.

引用自 [Memory mapping - The Linux Kernel documentation](https://linux-kernel-labs.github.io/refs/pull/222/merge/labs/memory_mapping.html)
> struct mm_struct encompasses all memory areas associated with a process.

> struct vm_area_struct holds information about a contiguous virtual memory area. The memory areas of a process can be viewed by inspecting the maps attribute of the process via procfs

> [img src1](http://esos.hanyang.ac.kr/tc/2015gradproject2/i/entry/2)
> [img src2](https://stackoverflow.com/questions/63497757/why-does-the-vm-area-struct-have-start-code-end-code-field)
![img link](https://i.stack.imgur.com/0kIuB.jpg)

此系統呼叫能透過 pid 得到 current process 的 task_struct，再透過 task_struct 得到其成員 mm_struct，藉此存取 mm_struct 的成員 **mmap (a list of vm_area_struct)**, start_code,end_code, start_data, end_data ...  
[Link to Source Code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/get_task_mm.c)

#### 2. get_phys_addr.c

> [Kconfig - arch/x86/Kconfig - Linux source code (v4.15.1) - Bootlin](https://elixir.bootlin.com/linux/v4.15.1/source/arch/x86/Kconfig#L331)
```
config PGTABLE_LEVELS
	int
	default 5 if X86_5LEVEL
	default 4 if X86_64
	default 3 if X86_PAE
	default 2
```
在 X86_64 沒有開啟 PAE (physical address extension) 與 X86_5LEVEL 的情況下 page table 應為四層 (pgd, pud, pmd, pte)
```
➜  project1 git:(main) ✗ cat /boot/config-`uname -r` | grep 5LEVEL        
# CONFIG_X86_5LEVEL is not set
```
> [pgtable.h - arch/x86/include/asm/pgtable.h - Linux source code (v4.15.1) - Bootlin](https://elixir.bootlin.com/linux/v4.15.1/source/arch/x86/include/asm/pgtable.h)

`pgd_offset()`, `pud_offset()` 等 functions 定義在上述 header file，透過這些 functions & `current->mm->pgd` 我們就能確定 kernel 有無對某 virtual address 分配 page frame，沒有的話以下新增的 system call 會傳回 -1，若有則會透過傳入的 `unsigned long *addr_ptr` 將傳入的 virtual address 在 page tables 中對應的 physical address 儲存在 caller 的 stack (local variable) 裡面 (實際程式碼在下方的 user space code `main.c` 當中)

[Link to Source Code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/get_phys_addr.c)

### 新增 system call 與編譯核心過程
> [Adding A System Call To The Linux Kernel](https://dev.to/jasper/adding-a-system-call-to-the-linux-kernel-5-8-1-in-ubuntu-20-04-lts-2ga8)
### User Space Code
[Link to Source Code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/main.c)

編譯時須加上 -lpthread 選項
```gcc main.c -lpthread -o main```
### Ouput of `./main`
在當前目錄下輸入 `./main`
會得到以下輸出 (執行緒間輸出的順序不固定)
```
thread: t1
The value of thread_i in t1: 1 (address: 0x7fbd7cee66fc, pa: f0386fc)
The address of char *str in t1       [stack]: 0x7fbd7cee5f28, pa: cc551f28
The value of char *heap_str in t1    [heap|shared_memory]: 0x7fbd780008c0, pa: c98c8c0
The address of global variable char *global_str: 0x6020a0, pa: 41690a0
The address of uninitialized variable char *BSS_str: 0x6020b0, pa: 41690b0

thread: t2
The value of thread_i in t2: 2 (address: 0x7fbd77fff6fc, pa: e2736fc)
The address of char *str in t2       [stack]: 0x7fbd77ffef28, pa: d3f52f28
The value of char *heap_str in t2    [heap|shared_memory]: 0x7fbd700008c0, pa: d55918c0
The address of global variable char *global_str: 0x6020a0, pa: 41690a0
The address of uninitialized variable char *BSS_str: 0x6020b0, pa: 41690b0

thread: main
The value of thread_i in main: 0 (va: 0x7fbd7d6d56fc, pa: 3d5c16fc)
The address of char *str in main    [stack]: 0x7ffeb831da70, pa: 1b85ba70
The value of char *heap_str in main [heap]: 0x249e270, pa: 11c47270
The address of global variable char *global_str: 0x6020a0, pa: 41690a0
The address of uninitialized variable char *BSS_str: 0x6020b0, pa: 41690b0

system call return 0
```
- 可以發現全域變數 `__thread int thread_i` 在**不同執行緒裡都有一份 (virtual address, physical address 不同)**
符合 [Thread-Local Storage](https://web.mit.edu/rhel-doc/3/rhel-gcc-en-3/thread-local.html) 裡面對 thread-local variable 的說明

- 不管有沒有起始初始值，全域變數都是完全相同的，在**不同執行緒裡都是存取到同一變數 (virtual address, physical address 相同)**，因此可以確認 data segment, BSS segment 確實是共享的

### Output of `dmesg`
此時再輸入 `dmesg` 會得到以下輸出
```
[  221.596637] start_code of mm_struct is 400000 (pa: d542d000)
[  221.596639] end_code of mm_struct is 401364 (pa: 6c9e364)
[  221.596639] start_data of mm_struct is 601e00 (pa: cfefce00)
[  221.596640] end_data of mm_struct is 6020a8 (pa: 41690a8)
[  221.596640] start_brk of mm_struct is 249e000 (pa: 11c47000)
[  221.596641] brk of mm_struct is 24bf000 (pa: ffffffffffffffff)

[  221.596641] mmap_base of mm_struct is 7fbd7d6f6000 (pa: ffffffffffffffff)

[  221.596642] start_stack of mm_struct is 7ffeb831db90 (pa: 1b85bb90)
[  221.596642] arg_start of mm_struct is 7ffeb831ee36
[  221.596643] arg_end of mm_struct is 7ffeb831ee3d
[  221.596643] env_start of mm_struct is 7ffeb831ee3d
[  221.596643] env_end of mm_struct is 7ffeb831fff1

[  221.596645] vm_start: 400000 (pa: d542d000)    vm_end: 402000    size: 2000
[  221.596645] vm_start: 601000 (pa: cfefc000)    vm_end: 602000    size: 1000
[  221.596646] vm_start: 602000 (pa: 4169000)    vm_end: 603000    size: 1000
[  221.596647] vm_start: 249e000 (pa: 11c47000)    vm_end: 24bf000    size: 21000
[  221.596648] vm_start: 7fbd70000000 (pa: d5591000)    vm_end: 7fbd70021000    size: 21000
[  221.596649] vm_start: 7fbd70021000 (pa: ffffffffffffffff)    vm_end: 7fbd74000000    size: 3fdf000
[  221.596650] vm_start: 7fbd777ff000 (pa: ffffffffffffffff)    vm_end: 7fbd77800000    size: 1000
[  221.596650] vm_start: 7fbd77800000 (pa: ffffffffffffffff)    vm_end: 7fbd78000000    size: 800000
[  221.596651] vm_start: 7fbd78000000 (pa: c98c000)    vm_end: 7fbd78021000    size: 21000
[  221.596652] vm_start: 7fbd78021000 (pa: ffffffffffffffff)    vm_end: 7fbd7c000000    size: 3fdf000
[  221.596653] vm_start: 7fbd7c4d0000 (pa: 10bb67000)    vm_end: 7fbd7c4e6000    size: 16000
[  221.596653] vm_start: 7fbd7c4e6000 (pa: ffffffffffffffff)    vm_end: 7fbd7c6e5000    size: 1ff000
[  221.596654] vm_start: 7fbd7c6e5000 (pa: 119c7000)    vm_end: 7fbd7c6e6000    size: 1000
[  221.596655] vm_start: 7fbd7c6e6000 (pa: ffffffffffffffff)    vm_end: 7fbd7c6e7000    size: 1000
[  221.596656] vm_start: 7fbd7c6e7000 (pa: ffffffffffffffff)    vm_end: 7fbd7cee7000    size: 800000
[  221.596656] vm_start: 7fbd7cee7000 (pa: 11fe3f000)    vm_end: 7fbd7d0a7000    size: 1c0000
[  221.596657] vm_start: 7fbd7d0a7000 (pa: ffffffffffffffff)    vm_end: 7fbd7d2a7000    size: 200000
[  221.596658] vm_start: 7fbd7d2a7000 (pa: 119f7000)    vm_end: 7fbd7d2ab000    size: 4000
[  221.596659] vm_start: 7fbd7d2ab000 (pa: 55e00000)    vm_end: 7fbd7d2ad000    size: 2000
[  221.596659] vm_start: 7fbd7d2ad000 (pa: d228f000)    vm_end: 7fbd7d2b1000    size: 4000
[  221.596660] vm_start: 7fbd7d2b1000 (pa: 11fe1a000)    vm_end: 7fbd7d2c9000    size: 18000
[  221.596661] vm_start: 7fbd7d2c9000 (pa: ffffffffffffffff)    vm_end: 7fbd7d4c8000    size: 1ff000
[  221.596662] vm_start: 7fbd7d4c8000 (pa: 42da000)    vm_end: 7fbd7d4c9000    size: 1000
[  221.596662] vm_start: 7fbd7d4c9000 (pa: d106e000)    vm_end: 7fbd7d4ca000    size: 1000
[  221.596663] vm_start: 7fbd7d4ca000 (pa: ffffffffffffffff)    vm_end: 7fbd7d4ce000    size: 4000
[  221.596664] vm_start: 7fbd7d4ce000 (pa: 11fd3e000)    vm_end: 7fbd7d4f4000    size: 26000
[  221.596665] vm_start: 7fbd7d6d4000 (pa: 1902b000)    vm_end: 7fbd7d6d8000    size: 4000
[  221.596665] vm_start: 7fbd7d6f3000 (pa: d54bb000)    vm_end: 7fbd7d6f4000    size: 1000
[  221.596666] vm_start: 7fbd7d6f4000 (pa: 1eb0000)    vm_end: 7fbd7d6f5000    size: 1000
[  221.596667] vm_start: 7fbd7d6f5000 (pa: 1a0b3000)    vm_end: 7fbd7d6f6000    size: 1000
[  221.596668] vm_start: 7ffeb82fe000 (pa: ffffffffffffffff)    vm_end: 7ffeb8320000    size: 22000
[  221.596668] vm_start: 7ffeb8331000 (pa: ffffffffffffffff)    vm_end: 7ffeb8334000    size: 3000
[  221.596669] vm_start: 7ffeb8334000 (pa: 69396000)    vm_end: 7ffeb8336000    size: 2000
```
pa (physical address) 若為 ffffffffffffffff (-1) 表示 virtual address 對應的某一層 page directory 或 table entry 的值為 0，kernel 並沒有配置 page frame 給該 virtual address

---
### Virtual Memory Layout
<!-- *(此 multi-thead program 的 virtual memory layout 還有待探討)* -->
根據 [Jason/cntofu.com, 深入 Linux 多線程編程](https://cntofu.com/book/46/linux_system/shen_rulinux_duo_xian_cheng_bian_cheng.md)，同一個 process 內的執行緒共享 global variables, heap 但每個執行緒都有自己的 stack，由此推論 `char *str` 應位於執行緒自身的 stack 中 (在函式中宣告，為 local variable)

結合上方程式輸出與下方 gdb `info proc mapping` 的輸出，能夠發現 main() 中的 `char *str` 位於 [stack] 區段，而 `start_routine()` 中的 `char *str` 則位於 `/lib/x86_64-linux-gnu/libgcc_s.so.1` 與 `/lib/x86_64-linux-gnu/libc-2.23.so` 之間的可讀可寫區段，應可判斷此區段即為執行緒自身的 stack

在 [此開發環境下](https://hackmd.io/vi-cx3xQSz-IGW6GtKaqmg?view#Environment) PIE (Position Independent Executables) 預設不會開啟，text segment 的位址不會隨機變動，配置於 data segemnt, BSS segment 的變數位址也不會改變，每一次執行時上述的變數位址輸出均為固定 (`&global_str` 與 `&BSS_str`)

#### Memory Space Mapping Generated by `info proc mapping` in GDB
```
(gdb) info proc mapping
process 3260
Mapped address spaces:

          Start Addr           End Addr       Size     Offset objfile
            0x400000           0x402000     0x2000        0x0 /home/shang/repo/Linux-OS-projects/project1/main
            0x601000           0x602000     0x1000     0x1000 /home/shang/repo/Linux-OS-projects/project1/main
            0x602000           0x603000     0x1000     0x2000 /home/shang/repo/Linux-OS-projects/project1/main
            0x603000           0x624000    0x21000        0x0 [heap]
      0x7fffe8000000     0x7fffe8021000    0x21000        0x0 **[heap for thread2]**
      0x7fffe8021000     0x7fffec000000  0x3fdf000        0x0 
      0x7ffff0000000     0x7ffff0021000    0x21000        0x0 **[heap for thread1]**
      0x7ffff0021000     0x7ffff4000000  0x3fdf000        0x0 
      0x7ffff65d8000     0x7ffff65ee000    0x16000        0x0 /lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff65ee000     0x7ffff67ed000   0x1ff000    0x16000 /lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff67ed000     0x7ffff67ee000     0x1000    0x15000 /lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff67ee000     0x7ffff67ef000     0x1000        0x0 
      0x7ffff67ef000     0x7ffff6fef000   0x800000        0x0 **[stack for thread2]**
      0x7ffff6fef000     0x7ffff6ff0000     0x1000        0x0 
      0x7ffff6ff0000     0x7ffff77f0000   0x800000        0x0 **[stack for thread1]**
      0x7ffff77f0000     0x7ffff79b0000   0x1c0000        0x0 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7ffff79b0000     0x7ffff7bb0000   0x200000   0x1c0000 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7ffff7bb0000     0x7ffff7bb4000     0x4000   0x1c0000 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7ffff7bb4000     0x7ffff7bb6000     0x2000   0x1c4000 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7ffff7bb6000     0x7ffff7bba000     0x4000        0x0 
      0x7ffff7bba000     0x7ffff7bd2000    0x18000        0x0 /lib/x86_64-linux-gnu/libpthread-2.23.so
      0x7ffff7bd2000     0x7ffff7dd1000   0x1ff000    0x18000 /lib/x86_64-linux-gnu/libpthread-2.23.so
      0x7ffff7dd1000     0x7ffff7dd2000     0x1000    0x17000 /lib/x86_64-linux-gnu/libpthread-2.23.so
      0x7ffff7dd2000     0x7ffff7dd3000     0x1000    0x18000 /lib/x86_64-linux-gnu/libpthread-2.23.so
      0x7ffff7dd3000     0x7ffff7dd7000     0x4000        0x0 
      0x7ffff7dd7000     0x7ffff7dfd000    0x26000        0x0 /lib/x86_64-linux-gnu/ld-2.23.so
      0x7ffff7fd8000     0x7ffff7fdc000     0x4000        0x0 
      0x7ffff7ff7000     0x7ffff7ffa000     0x3000        0x0 [vvar]
      0x7ffff7ffa000     0x7ffff7ffc000     0x2000        0x0 [vdso]
      0x7ffff7ffc000     0x7ffff7ffd000     0x1000    0x25000 /lib/x86_64-linux-gnu/ld-2.23.so
      0x7ffff7ffd000     0x7ffff7ffe000     0x1000    0x26000 /lib/x86_64-linux-gnu/ld-2.23.so
      0x7ffff7ffe000     0x7ffff7fff000     0x1000        0x0 
      0x7ffffffdd000     0x7ffffffff000    0x22000        0x0 [stack]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0 [vsyscall]
```
進一步確認可發現
1. **兩個執行緒 t1, t2 的 `char *str` 位於不同的區段**，應為兩執行緒各自的 stack (上方標示之 [stack for thread[1|2]])

2. 在執行緒的 `start_routine()` 中透過 `malloc()` 動態配置的由 `char *heap_str` 所指向的空間位於 [heap] 與 `/lib/x86_64-linux-gnu/libgcc_s.so.1` 之間的**兩個不同的可讀可寫區段** (上方標示之 [heap for thread[1|2]])
根據 [Jason/cntofu.com, 深入 Linux 多線程編程](https://cntofu.com/book/46/linux_system/shen_rulinux_duo_xian_cheng_bian_cheng.md)，該空間應屬於 shared library, shared memory，且執行緒間共享 heap
    - 問題一: 若同一 process 內的執行緒共享 [heap] 的話，則與上述 2. 的狀況不符
    > [Heap Exploitation Part 1: Understanding the Glibc Heap Implementation |  Azeria Labs](https://azeria-labs.com/heap-exploitation-part-1-understanding-the-glibc-heap-implementation/)

    上述連結中的 **arena**, **subheap** 應該與此有關
    > 在 multithreaded application 裡面為避免 race condition 需要用 mutex lock 來確保任何時間點僅有單一的 thread 可以和 heap 互動，但此策略在 multithreaded application 裡因 heap operations 為 performance sensitive 的操作會導致效能低下的狀況，因此有了 "arenas" 的概念

    > 一開始的 (“main”) arena 僅包含單一的 heap，對於 single-threaded application 來說這是 heap manager 唯一用到的 arena。當新的 threads 產生之後 heap manager 會配置次要的 (secondary) arenas 給這些新的 threads，這麼做能夠降低執行緒為了執行像是 malloc, free 這類 heap operations 而等待其他執行緒的機率。
    
### Summary Figure
Check the figure which summarizes the results through the following link

![Link to Image](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/Virtual_Memory_Layout.drawio.png)

### References
> [Thread-Local Storage](https://web.mit.edu/rhel-doc/3/rhel-gcc-en-3/thread-local.html)  
> [G. T. Wang, C 語言 pthread 多執行緒平行化程式設計入門教學與範例](https://blog.gtwang.org/programming/pthread-multithreading-programming-in-c-tutorial/)  
> [Jason/cntofu.com, 深入 Linux 多線程編程](https://cntofu.com/book/46/linux_system/shen_rulinux_duo_xian_cheng_bian_cheng.md)  
> [Project 1 from course website](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2022/linux_project_1.html)  
> [Adding A System Call To The Linux Kernel](https://dev.to/jasper/adding-a-system-call-to-the-linux-kernel-5-8-1-in-ubuntu-20-04-lts-2ga8)  
> [get_pid和get_task_mm](https://zhuanlan.zhihu.com/p/41788388)  
> [__task_pid_nr_ns和find_get_pid](https://zhuanlan.zhihu.com/p/41260133)  
> [Memory mapping - The Linux Kernel documentation](https://linux-kernel-labs.github.io/refs/pull/222/merge/labs/memory_mapping.html)  
> [Linux kernel 内存 - 页表映射（SHIFT，SIZE，MASK）和转换(32位，64位) - 苏小北1024 - 博客园](https://www.cnblogs.com/muahao/p/10297852.html)  
> [Linux Kernel - HackMD](https://hackmd.io/@eugenechou/H1LGA9AiB#Project-1)  
