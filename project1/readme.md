## 多執行緒程式實作
### Environment
> Linux kernel version: 4.15.1
Distribution: Ubuntu 16.04.7
gcc: 5.4.0 
gdb: 8.3
### Kernel Space Code (New System Call)
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

此系統呼叫能透過 pid 得到 current process 的 task_struct，再透過 task_struct 得到其成員 mm_struct，藉此存取 mm_struct 的成員 **mmap (a list of vm_area_struct)**, start_code, end_code, start_data, end_data ...
[Link to Source Code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project1/get_task_mm.c)

#### 新增 system call 與編譯核心過程
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
The value of thread_i in t1: 1 (address: 0x7f9cecc516fc, pa: cac056fc)
The address of char *str in t1       [stack]: 0x7f9cecc50f30, pa: c43d7f30
The value of char *heap_str in t1    [heap|shared_memory]: 0x7f9ce80008c0, pa: cb79b8c0
The address of global variable char *global_str: 0x602098, pa: 4528a098
The address of uninitialized variable char *BSS_str: 0x6020a8, pa: 4528a0a8

thread: t2
The value of thread_i in t2: 2 (address: 0x7f9ce7fff6fc, pa: c4ef76fc)
The address of char *str in t2       [stack]: 0x7f9ce7ffef30, pa: cac00f30
The value of char *heap_str in t2    [heap|shared_memory]: 0x7f9ce00008c0, pa: 1be168c0
The address of global variable char *global_str: 0x602098, pa: 4528a098
The address of uninitialized variable char *BSS_str: 0x6020a8, pa: 4528a0a8

thread: main
The value of thread_i in main: 0 (va: 0x7f9ced4406fc, pa: c7446fc)
The address of char *str in main    [stack]: 0x7ffcdf723520, pa: c7cfc520
The value of char *heap_str in main [heap]: 0xe35270, pa: caa36270
The address of global variable char *global_str: 0x602098, pa: 4528a098
The address of uninitialized variable char *BSS_str: 0x6020a8, pa: 4528a0a8

system call return 0
```
* 可以發現全域變數 `__thread int thread_i` 在**不同執行緒裡都有一份 (位址不同且值也不同，為不同的變數)**

符合 [Thread-Local Storage](https://web.mit.edu/rhel-doc/3/rhel-gcc-en-3/thread-local.html) 裡面對 thread-local variable 的說明
### Output of `dmesg`
此時再輸入 `dmesg` 會得到以下輸出
```
[  155.241730] virtual adddress: 7f9cecc516fc    physical address: cac056fc
[  155.241736] virtual adddress: 7f9cecc50f30    physical address: c43d7f30
[  155.241738] virtual adddress: 7f9ce80008c0    physical address: cb79b8c0
[  155.241740] virtual adddress: 602098    physical address: 4528a098
[  155.241742] virtual adddress: 6020a8    physical address: 4528a0a8
[  156.242080] virtual adddress: 7f9ce7fff6fc    physical address: c4ef76fc
[  156.242083] virtual adddress: 7f9ce7ffef30    physical address: cac00f30
[  156.242086] virtual adddress: 7f9ce00008c0    physical address: 1be168c0
[  156.242088] virtual adddress: 602098    physical address: 4528a098
[  156.242090] virtual adddress: 6020a8    physical address: 4528a0a8
[  157.242760] virtual adddress: 7f9ced4406fc    physical address: c7446fc
[  157.242767] virtual adddress: 7ffcdf723520    physical address: c7cfc520
[  157.242769] virtual adddress: e35270    physical address: caa36270
[  157.242772] virtual adddress: 602098    physical address: 4528a098
[  157.242774] virtual adddress: 6020a8    physical address: 4528a0a8
[  157.242777] start_code of mm_struct is 400000
[  157.242778] virtual adddress: 400000    physical address: ca85f000
[  157.242778] end_code of mm_struct is 4012b4
[  157.242779] start_data of mm_struct is 601e00
[  157.242779] virtual adddress: 601e00    physical address: 50005e00
[  157.242780] end_data of mm_struct is 6020a0
[  157.242780] start_brk of mm_struct is e35000
[  157.242780] virtual adddress: e35000    physical address: caa36000
[  157.242781] brk of mm_struct is e56000

[  157.242781] mmap_base of mm_struct is 7f9ced461000

[  157.242782] start_stack of mm_struct is 7ffcdf723640
[  157.242782] virtual adddress: 7ffcdf723640    physical address: c7cfc640
[  157.242783] arg_start of mm_struct is 7ffcdf723e9c
[  157.242783] arg_end of mm_struct is 7ffcdf723ea3

[  157.242783] env_start of mm_struct is 7ffcdf723ea3
[  157.242784] env_end of mm_struct is 7ffcdf724ff1

[  157.242784] vm_start: 400000    vm_end: 402000    size: 2000
[  157.242785] virtual adddress: 400000    physical address: ca85f000
[  157.242786] vm_start: 601000    vm_end: 602000    size: 1000
[  157.242786] virtual adddress: 601000    physical address: 50005000
[  157.242787] vm_start: 602000    vm_end: 603000    size: 1000
[  157.242787] virtual adddress: 602000    physical address: 4528a000
[  157.242788] vm_start: e35000    vm_end: e56000    size: 21000
[  157.242788] virtual adddress: e35000    physical address: caa36000
[  157.242789] vm_start: 7f9ce0000000    vm_end: 7f9ce0021000    size: 21000
[  157.242790] virtual adddress: 7f9ce0000000    physical address: 1be16000
[  157.242790] vm_start: 7f9ce0021000    vm_end: 7f9ce4000000    size: 3fdf000
[  157.242791] vm_start: 7f9ce77ff000    vm_end: 7f9ce7800000    size: 1000
[  157.242792] vm_start: 7f9ce7800000    vm_end: 7f9ce8000000    size: 800000
[  157.242793] vm_start: 7f9ce8000000    vm_end: 7f9ce8021000    size: 21000
[  157.242793] virtual adddress: 7f9ce8000000    physical address: cb79b000
[  157.242794] vm_start: 7f9ce8021000    vm_end: 7f9cec000000    size: 3fdf000
[  157.242794] vm_start: 7f9cec23b000    vm_end: 7f9cec251000    size: 16000
[  157.242795] virtual adddress: 7f9cec23b000    physical address: 10a940000
[  157.242796] vm_start: 7f9cec251000    vm_end: 7f9cec450000    size: 1ff000
[  157.242796] vm_start: 7f9cec450000    vm_end: 7f9cec451000    size: 1000
[  157.242797] virtual adddress: 7f9cec450000    physical address: ca155000
[  157.242797] vm_start: 7f9cec451000    vm_end: 7f9cec452000    size: 1000
[  157.242798] vm_start: 7f9cec452000    vm_end: 7f9cecc52000    size: 800000
[  157.242798] vm_start: 7f9cecc52000    vm_end: 7f9cece12000    size: 1c0000
[  157.242799] virtual adddress: 7f9cecc52000    physical address: 11fe3f000
[  157.242800] vm_start: 7f9cece12000    vm_end: 7f9ced012000    size: 200000
[  157.242800] vm_start: 7f9ced012000    vm_end: 7f9ced016000    size: 4000
[  157.242801] virtual adddress: 7f9ced012000    physical address: cf3c6000
[  157.242801] vm_start: 7f9ced016000    vm_end: 7f9ced018000    size: 2000
[  157.242802] virtual adddress: 7f9ced016000    physical address: 40519000
[  157.242802] vm_start: 7f9ced018000    vm_end: 7f9ced01c000    size: 4000
[  157.242803] virtual adddress: 7f9ced018000    physical address: caa37000
[  157.242803] vm_start: 7f9ced01c000    vm_end: 7f9ced034000    size: 18000
[  157.242804] virtual adddress: 7f9ced01c000    physical address: 11fe1a000
[  157.242804] vm_start: 7f9ced034000    vm_end: 7f9ced233000    size: 1ff000
[  157.242805] vm_start: 7f9ced233000    vm_end: 7f9ced234000    size: 1000
[  157.242806] virtual adddress: 7f9ced233000    physical address: 43dfa000
[  157.242806] vm_start: 7f9ced234000    vm_end: 7f9ced235000    size: 1000
[  157.242807] virtual adddress: 7f9ced234000    physical address: 134b3000
[  157.242807] vm_start: 7f9ced235000    vm_end: 7f9ced239000    size: 4000
[  157.242808] vm_start: 7f9ced239000    vm_end: 7f9ced25f000    size: 26000
[  157.242808] virtual adddress: 7f9ced239000    physical address: 11fd3e000
[  157.242809] vm_start: 7f9ced43f000    vm_end: 7f9ced443000    size: 4000
[  157.242810] virtual adddress: 7f9ced43f000    physical address: 40518000
[  157.242810] vm_start: 7f9ced45e000    vm_end: 7f9ced45f000    size: 1000
[  157.242811] virtual adddress: 7f9ced45e000    physical address: 3fc28000
[  157.242811] vm_start: 7f9ced45f000    vm_end: 7f9ced460000    size: 1000
[  157.242812] virtual adddress: 7f9ced45f000    physical address: 5f2cb000
[  157.242812] vm_start: 7f9ced460000    vm_end: 7f9ced461000    size: 1000
[  157.242813] virtual adddress: 7f9ced460000    physical address: 43b35000
[  157.242813] vm_start: 7ffcdf703000    vm_end: 7ffcdf725000    size: 22000
[  157.242814] vm_start: 7ffcdf74e000    vm_end: 7ffcdf751000    size: 3000
[  157.242815] vm_start: 7ffcdf751000    vm_end: 7ffcdf753000    size: 2000
[  157.242815] virtual adddress: 7ffcdf751000    physical address: d6396000
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