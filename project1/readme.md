## 多執行緒程式實作
### Environment
> Linux kernel version: 4.15.1
Distribution: Ubuntu 16.04.7
gcc: 5.4.0 
gdb: 8.3
### Kernel Space Code (New System Call)
[Link to Source Code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/get_task_mm.c)
#### References:
> [get_pid和get_task_mm](https://zhuanlan.zhihu.com/p/41788388)  
> [__task_pid_nr_ns和find_get_pid](https://zhuanlan.zhihu.com/p/41260133)


引用自 [Process Descriptor - Understanding the Linux Kernel](https://www.oreilly.com/library/view/understanding-the-linux/0596002130/ch03s02.html)
> This is the role of the process descriptor — a **task_struct** type structure whose fields contain all the information related to a single process.

引用自 [Memory mapping - The Linux Kernel documentation](https://linux-kernel-labs.github.io/refs/pull/222/merge/labs/memory_mapping.html)
> struct mm_struct encompasses all memory areas associated with a process.

> struct vm_area_struct holds information about a contiguous virtual memory area. The memory areas of a process can be viewed by inspecting the maps attribute of the process via procfs

> [img src1](http://esos.hanyang.ac.kr/tc/2015gradproject2/i/entry/2)
> [img src2](https://stackoverflow.com/questions/63497757/why-does-the-vm-area-struct-have-start-code-end-code-field)
![img link](https://i.stack.imgur.com/0kIuB.jpg)

此系統呼叫能透過 pid 得到 current process 的 task_struct，再透過 task_struct 得到其成員 mm_struct，藉此存取 mm_struct 的成員 start_code,end_code, ...

#### 新增 system call 與編譯核心過程
> [Adding A System Call To The Linux Kernel](https://dev.to/jasper/adding-a-system-call-to-the-linux-kernel-5-8-1-in-ubuntu-20-04-lts-2ga8)
### User Space Code
[Link to Source Code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/main.c)

編譯時須加上 -lpthread 選項
```gcc main.c -lpthread -o main```
### Ouput
在當前目錄下輸入 `./main`
會得到以下輸出 (執行緒間輸出的順序不固定)
```
thread: t1
The value of thread_i in t1: 1 (address: 0x7ffff77ef6fc)
The address of char *str in t1       [stack]: 0x7ffff77eef38
The address of char *local_str in t1 [stack]: 0x7ffff77eef00
The value of char *heap_str in t1    [heap|shared_memory]: 0x7ffff00008c0
The address of global variable char *global_str: 0x602098

thread: t2
The value of thread_i in t2: 2 (address: 0x7ffff6fee6fc)
The address of char *str in t2       [stack]: 0x7ffff6fedf38
The address of char *local_str in t2 [stack]: 0x7ffff6fedf00
The value of char *heap_str in t2    [heap|shared_memory]: 0x7fffe80008c0
The address of global variable char *global_str: 0x602098

thread: main
The value of thread_i in main: 0 (address: 0x7ffff7fd96fc)
The address of char *str in main    [stack]: 0x7fffffffd8a8
The value of char *heap_str in main [heap]: 0x603270
The address of global variable char *global_str: 0x602098
The address of uninitialized variable char *BSS_str: 0x6020a8

system call return 0
```
* 可以發現全域變數 `__thread int thread_i` 在**不同執行緒裡都有一份 (位址不同且值也不同，為不同的變數)**

符合 [Thread-Local Storage](https://web.mit.edu/rhel-doc/3/rhel-gcc-en-3/thread-local.html) 裡面對 thread-local variable 的說明
此時再輸入 `dmesg` 會得到以下輸出
```
[ 1660.986937] start_code of mm_struct is 400000
[ 1660.986938] end_code of mm_struct is 40119c

[ 1660.986938] start_data of mm_struct is 601e00
[ 1660.986939] end_data of mm_struct is 6020a0

[ 1660.986940] start_brk of mm_struct is 603000
[ 1660.986940] brk of mm_struct is 624000

[ 1660.986941] mmap_base of mm_struct is 7ffff7fff000

[ 1660.986941] start_stack of mm_struct is 7fffffffd9c0
[ 1660.986942] arg_start of mm_struct is 7fffffffde2c
[ 1660.986942] arg_end of mm_struct is 7fffffffde5d

[ 1660.986942] env_start of mm_struct is 7fffffffde5d
[ 1660.986943] env_end of mm_struct is 7fffffffefc7

[ 1660.986943] vm_start of vm_area_struct is 400000
[ 1660.986944] vm_end of vm_area_struct is 402000
[ 1660.986944] the size of the vm_area (vm_end - vm_start) is 2000

[ 1660.986944] vm_start of vm_area_struct is 601000
[ 1660.986945] vm_end of vm_area_struct is 602000
[ 1660.986945] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986946] vm_start of vm_area_struct is 602000
[ 1660.986946] vm_end of vm_area_struct is 603000
[ 1660.986946] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986947] vm_start of vm_area_struct is 603000
[ 1660.986947] vm_end of vm_area_struct is 624000
[ 1660.986948] the size of the vm_area (vm_end - vm_start) is 21000

[ 1660.986948] vm_start of vm_area_struct is 7fffe8000000
[ 1660.986948] vm_end of vm_area_struct is 7fffe8021000
[ 1660.986949] the size of the vm_area (vm_end - vm_start) is 21000

[ 1660.986949] vm_start of vm_area_struct is 7fffe8021000
[ 1660.986950] vm_end of vm_area_struct is 7fffec000000
[ 1660.986950] the size of the vm_area (vm_end - vm_start) is 3fdf000

[ 1660.986951] vm_start of vm_area_struct is 7ffff0000000
[ 1660.986951] vm_end of vm_area_struct is 7ffff0021000
[ 1660.986951] the size of the vm_area (vm_end - vm_start) is 21000

[ 1660.986952] vm_start of vm_area_struct is 7ffff0021000
[ 1660.986952] vm_end of vm_area_struct is 7ffff4000000
[ 1660.986953] the size of the vm_area (vm_end - vm_start) is 3fdf000

[ 1660.986953] vm_start of vm_area_struct is 7ffff65d8000
[ 1660.986953] vm_end of vm_area_struct is 7ffff65ee000
[ 1660.986954] the size of the vm_area (vm_end - vm_start) is 16000

[ 1660.986954] vm_start of vm_area_struct is 7ffff65ee000
[ 1660.986955] vm_end of vm_area_struct is 7ffff67ed000
[ 1660.986955] the size of the vm_area (vm_end - vm_start) is 1ff000

[ 1660.986955] vm_start of vm_area_struct is 7ffff67ed000
[ 1660.986956] vm_end of vm_area_struct is 7ffff67ee000
[ 1660.986956] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986957] vm_start of vm_area_struct is 7ffff67ee000
[ 1660.986957] vm_end of vm_area_struct is 7ffff67ef000
[ 1660.986958] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986958] vm_start of vm_area_struct is 7ffff67ef000
[ 1660.986958] vm_end of vm_area_struct is 7ffff6fef000
[ 1660.986959] the size of the vm_area (vm_end - vm_start) is 800000

[ 1660.986959] vm_start of vm_area_struct is 7ffff6fef000
[ 1660.986959] vm_end of vm_area_struct is 7ffff6ff0000
[ 1660.986960] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986960] vm_start of vm_area_struct is 7ffff6ff0000
[ 1660.986961] vm_end of vm_area_struct is 7ffff77f0000
[ 1660.986961] the size of the vm_area (vm_end - vm_start) is 800000

[ 1660.986962] vm_start of vm_area_struct is 7ffff77f0000
[ 1660.986962] vm_end of vm_area_struct is 7ffff79b0000
[ 1660.986963] the size of the vm_area (vm_end - vm_start) is 1c0000

[ 1660.986963] vm_start of vm_area_struct is 7ffff79b0000
[ 1660.986963] vm_end of vm_area_struct is 7ffff7bb0000
[ 1660.986964] the size of the vm_area (vm_end - vm_start) is 200000

[ 1660.986964] vm_start of vm_area_struct is 7ffff7bb0000
[ 1660.986965] vm_end of vm_area_struct is 7ffff7bb4000
[ 1660.986965] the size of the vm_area (vm_end - vm_start) is 4000

[ 1660.986966] vm_start of vm_area_struct is 7ffff7bb4000
[ 1660.986966] vm_end of vm_area_struct is 7ffff7bb6000
[ 1660.986966] the size of the vm_area (vm_end - vm_start) is 2000

[ 1660.986967] vm_start of vm_area_struct is 7ffff7bb6000
[ 1660.986967] vm_end of vm_area_struct is 7ffff7bba000
[ 1660.986968] the size of the vm_area (vm_end - vm_start) is 4000

[ 1660.986968] vm_start of vm_area_struct is 7ffff7bba000
[ 1660.986968] vm_end of vm_area_struct is 7ffff7bd2000
[ 1660.986969] the size of the vm_area (vm_end - vm_start) is 18000

[ 1660.986969] vm_start of vm_area_struct is 7ffff7bd2000
[ 1660.986970] vm_end of vm_area_struct is 7ffff7dd1000
[ 1660.986970] the size of the vm_area (vm_end - vm_start) is 1ff000

[ 1660.986970] vm_start of vm_area_struct is 7ffff7dd1000
[ 1660.986971] vm_end of vm_area_struct is 7ffff7dd2000
[ 1660.986971] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986972] vm_start of vm_area_struct is 7ffff7dd2000
[ 1660.986972] vm_end of vm_area_struct is 7ffff7dd3000
[ 1660.986972] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986973] vm_start of vm_area_struct is 7ffff7dd3000
[ 1660.986973] vm_end of vm_area_struct is 7ffff7dd7000
[ 1660.986974] the size of the vm_area (vm_end - vm_start) is 4000

[ 1660.986974] vm_start of vm_area_struct is 7ffff7dd7000
[ 1660.986974] vm_end of vm_area_struct is 7ffff7dfd000
[ 1660.986975] the size of the vm_area (vm_end - vm_start) is 26000

[ 1660.986975] vm_start of vm_area_struct is 7ffff7fd8000
[ 1660.986976] vm_end of vm_area_struct is 7ffff7fdc000
[ 1660.986976] the size of the vm_area (vm_end - vm_start) is 4000

[ 1660.986977] vm_start of vm_area_struct is 7ffff7ff7000
[ 1660.986977] vm_end of vm_area_struct is 7ffff7ffa000
[ 1660.986977] the size of the vm_area (vm_end - vm_start) is 3000

[ 1660.986978] vm_start of vm_area_struct is 7ffff7ffa000
[ 1660.986978] vm_end of vm_area_struct is 7ffff7ffc000
[ 1660.986979] the size of the vm_area (vm_end - vm_start) is 2000

[ 1660.986979] vm_start of vm_area_struct is 7ffff7ffc000
[ 1660.986979] vm_end of vm_area_struct is 7ffff7ffd000
[ 1660.986980] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986980] vm_start of vm_area_struct is 7ffff7ffd000
[ 1660.986981] vm_end of vm_area_struct is 7ffff7ffe000
[ 1660.986981] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986981] vm_start of vm_area_struct is 7ffff7ffe000
[ 1660.986982] vm_end of vm_area_struct is 7ffff7fff000
[ 1660.986982] the size of the vm_area (vm_end - vm_start) is 1000

[ 1660.986983] vm_start of vm_area_struct is 7ffffffdd000
[ 1660.986983] vm_end of vm_area_struct is 7ffffffff000

[ 1660.986983] the size of the vm_area (vm_end - vm_start) is 22000
```

---
### Virtual Memory Layout
<!-- *(此 multi-thead program 的 virtual memory layout 還有待探討)* -->
根據 [Jason/cntofu.com, 深入 Linux 多線程編程](https://cntofu.com/book/46/linux_system/shen_rulinux_duo_xian_cheng_bian_cheng.md)，同一個 process 內的執行緒共享 global variables, heap 但每個執行緒都有自己的 stack，由此推論 `char *str` 應位於執行緒自身的 stack 中 (在函式中宣告，為 local variable)

結合上方程式輸出與下方 gdb `info proc mapping` 的輸出，能夠發現 main() 中的 `char *str` 位於 [stack] 區段，而 `start_routine()` 中的 `char *str` 則位於 `/lib/x86_64-linux-gnu/libgcc_s.so.1` 與 `/lib/x86_64-linux-gnu/libc-2.23.so` 之間的可讀可寫區段，應可判斷此區段即為執行緒自身的 stack

在 [此開發環境下](https://hackmd.io/vi-cx3xQSz-IGW6GtKaqmg?view#Environment) PIE (Position Independent Executables) 預設不會開啟，text segment 的位址不會隨機變動，配置於 data segemnt, BSS segment 的變數位址也不會改變，每一次執行時上述的變數位址輸出均為固定 (`&global_str` 與 `&BSS_str`)

#### Memory Space Mapping Generated by `info proc mapping` in GDB
```
(gdb) info proc mapping
process 5436
Mapped address spaces:

          Start Addr           End Addr       Size     Offset objfile
            0x400000           0x402000     0x2000        0x0 /home/shang/repo/Linux-OS-projects/project1/main
            0x601000           0x602000     0x1000     0x1000 /home/shang/repo/Linux-OS-projects/project1/main
            0x602000           0x603000     0x1000     0x2000 /home/shang/repo/Linux-OS-projects/project1/main
            0x603000           0x624000    0x21000        0x0 [heap]
      0x7fffe8000000     0x7fffe8021000    0x21000        0x0 
      0x7fffe8021000     0x7fffec000000  0x3fdf000        0x0 
      0x7ffff0000000     0x7ffff0021000    0x21000        0x0 
      0x7ffff0021000     0x7ffff4000000  0x3fdf000        0x0 
      0x7ffff65d8000     0x7ffff65ee000    0x16000        0x0 /lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff65ee000     0x7ffff67ed000   0x1ff000    0x16000 /lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff67ed000     0x7ffff67ee000     0x1000    0x15000 /lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff67ee000     0x7ffff67ef000     0x1000        0x0 
      0x7ffff67ef000     0x7ffff6fef000   0x800000        0x0 
      0x7ffff6fef000     0x7ffff6ff0000     0x1000        0x0 
      0x7ffff6ff0000     0x7ffff77f0000   0x800000        0x0 
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

### Summary Figure
Check the figure which summarizes the results through the following link

[Link to Image](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/Virtual_Memory_Layout.drawio.png)

### References
> [Thread-Local Storage](https://web.mit.edu/rhel-doc/3/rhel-gcc-en-3/thread-local.html)  
> [G. T. Wang, C 語言 pthread 多執行緒平行化程式設計入門教學與範例](https://blog.gtwang.org/programming/pthread-multithreading-programming-in-c-tutorial/)  
> [Jason/cntofu.com, 深入 Linux 多線程編程](https://cntofu.com/book/46/linux_system/shen_rulinux_duo_xian_cheng_bian_cheng.md)  
> [Project 1 from course website](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2022/linux_project_1.html)  
> [Adding A System Call To The Linux Kernel](https://dev.to/jasper/adding-a-system-call-to-the-linux-kernel-5-8-1-in-ubuntu-20-04-lts-2ga8)  
> [get_pid和get_task_mm](https://zhuanlan.zhihu.com/p/41788388)  
> [__task_pid_nr_ns和find_get_pid](https://zhuanlan.zhihu.com/p/41260133)  
> [Memory mapping - The Linux Kernel documentation](https://linux-kernel-labs.github.io/refs/pull/222/merge/labs/memory_mapping.html)  
> 