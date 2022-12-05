# Project2
There are 2 implementations, one with `fork()` and `waitpid()` another with only `sleep(10000)`.
To check the experiment result for implementaion 2, execute the program (main2) in different shells.

## Environment
Linux kernel version: 4.15.1
Distribution: Ubuntu 16.04.7
gcc: 5.4.0 

## References
> [fork(2) — Linux manual page](https://man7.org/linux/man-pages/man2/fork.2.html)  
> [wait(2) — Linux manual page](https://man7.org/linux/man-pages/man2/wait.2.html)  
## Implementation 1
[Link to source code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project2/main.c)
### Output of `./main` (from `gcc -g main.c -o main`)
```
Parent process PID is 4381
Child process PID is 4382
The address of char *str in main [stack]: 0x7ffe4968bab8, pa: 843e9ab8
The value of char *heap_str in main [heap]: 0x2141010, pa: f996010
The address of global variable char *global_str [data]: 0x602088, pa: 843e8088
The address of uninitialized variable char *BSS_str [BSS]: 0x602098, pa: 843e8098

System call returned 0
The address of char *str in main [stack]: 0x7ffe4968bab8, pa: 87093ab8
The value of char *heap_str in main [heap]: 0x2141010, pa: ab378010
The address of global variable char *global_str [data]: 0x602088, pa: 68b2f088
The address of uninitialized variable char *BSS_str [BSS]: 0x602098, pa: 68b2f098

System call returned 0
```
### Output of `dmesg`
```
[ 2620.605542] start_code of mm_struct is 400000 (pa: 7fc47000)
[ 2620.605543] end_code of mm_struct is 400f04 (pa: 7fc47f04)
[ 2620.605543] start_data of mm_struct is 601e10 (pa: 44c7e10)
[ 2620.605544] end_data of mm_struct is 602090 (pa: 843e8090)
[ 2620.605544] start_brk of mm_struct is 2141000 (pa: f996000)
[ 2620.605545] brk of mm_struct is 2162000 (pa: ffffffffffffffff)

[ 2620.605546] mmap_base of mm_struct is 7f932e4a5000 (pa: ffffffffffffffff)

[ 2620.605546] start_stack of mm_struct is 7ffe4968bbd0 (pa: 843e9bd0)
[ 2620.605547] arg_start of mm_struct is 7ffe4968de36
[ 2620.605547] arg_end of mm_struct is 7ffe4968de3d
[ 2620.605547] env_start of mm_struct is 7ffe4968de3d
[ 2620.605548] env_end of mm_struct is 7ffe4968eff1

[ 2620.605548] vm_start: 400000 (pa: 7fc47000)    vm_end: 401000    size: 1000
[ 2620.605549] vm_start: 601000 (pa: 44c7000)    vm_end: 602000    size: 1000
[ 2620.605550] vm_start: 602000 (pa: 843e8000)    vm_end: 603000    size: 1000
[ 2620.605551] vm_start: 2141000 (pa: f996000)    vm_end: 2162000    size: 21000
[ 2620.605552] vm_start: 7f932deb3000 (pa: 11fe5e000)    vm_end: 7f932e073000    size: 1c0000
[ 2620.605552] vm_start: 7f932e073000 (pa: ffffffffffffffff)    vm_end: 7f932e273000    size: 200000
[ 2620.605553] vm_start: 7f932e273000 (pa: d1a24000)    vm_end: 7f932e277000    size: 4000
[ 2620.605554] vm_start: 7f932e277000 (pa: 843e5000)    vm_end: 7f932e279000    size: 2000
[ 2620.605555] vm_start: 7f932e279000 (pa: 13e1a000)    vm_end: 7f932e27d000    size: 4000
[ 2620.605555] vm_start: 7f932e27d000 (pa: 11fcbd000)    vm_end: 7f932e2a3000    size: 26000
[ 2620.605556] vm_start: 7f932e484000 (pa: 80131000)    vm_end: 7f932e487000    size: 3000
[ 2620.605557] vm_start: 7f932e4a2000 (pa: 845b000)    vm_end: 7f932e4a3000    size: 1000
[ 2620.605557] vm_start: 7f932e4a3000 (pa: 843e7000)    vm_end: 7f932e4a4000    size: 1000
[ 2620.605558] vm_start: 7f932e4a4000 (pa: acfb0000)    vm_end: 7f932e4a5000    size: 1000
[ 2620.605559] vm_start: 7ffe4966d000 (pa: ffffffffffffffff)    vm_end: 7ffe4968f000    size: 22000
[ 2620.605559] vm_start: 7ffe4979a000 (pa: ffffffffffffffff)    vm_end: 7ffe4979d000    size: 3000
[ 2620.605560] vm_start: 7ffe4979d000 (pa: 82996000)    vm_end: 7ffe4979f000    size: 2000
[ 2621.614646] start_code of mm_struct is 400000 (pa: 7fc47000)
[ 2621.614648] end_code of mm_struct is 400f04 (pa: 7fc47f04)
[ 2621.614649] start_data of mm_struct is 601e10 (pa: 44c7e10)
[ 2621.614649] end_data of mm_struct is 602090 (pa: 68b2f090)
[ 2621.614650] start_brk of mm_struct is 2141000 (pa: ab378000)
[ 2621.614650] brk of mm_struct is 2162000 (pa: ffffffffffffffff)

[ 2621.614651] mmap_base of mm_struct is 7f932e4a5000 (pa: ffffffffffffffff)

[ 2621.614651] start_stack of mm_struct is 7ffe4968bbd0 (pa: 87093bd0)
[ 2621.614652] arg_start of mm_struct is 7ffe4968de36
[ 2621.614652] arg_end of mm_struct is 7ffe4968de3d
[ 2621.614652] env_start of mm_struct is 7ffe4968de3d
[ 2621.614653] env_end of mm_struct is 7ffe4968eff1

[ 2621.614654] vm_start: 400000 (pa: 7fc47000)    vm_end: 401000    size: 1000
[ 2621.614654] vm_start: 601000 (pa: 44c7000)    vm_end: 602000    size: 1000
[ 2621.614655] vm_start: 602000 (pa: 68b2f000)    vm_end: 603000    size: 1000
[ 2621.614656] vm_start: 2141000 (pa: ab378000)    vm_end: 2162000    size: 21000
[ 2621.614657] vm_start: 7f932deb3000 (pa: 11fe5e000)    vm_end: 7f932e073000    size: 1c0000
[ 2621.614657] vm_start: 7f932e073000 (pa: ffffffffffffffff)    vm_end: 7f932e273000    size: 200000
[ 2621.614658] vm_start: 7f932e273000 (pa: d1a24000)    vm_end: 7f932e277000    size: 4000
[ 2621.614659] vm_start: 7f932e277000 (pa: 80130000)    vm_end: 7f932e279000    size: 2000
[ 2621.614660] vm_start: 7f932e279000 (pa: afba2000)    vm_end: 7f932e27d000    size: 4000
[ 2621.614660] vm_start: 7f932e27d000 (pa: ffffffffffffffff)    vm_end: 7f932e2a3000    size: 26000
[ 2621.614661] vm_start: 7f932e484000 (pa: 80131000)    vm_end: 7f932e487000    size: 3000
[ 2621.614662] vm_start: 7f932e4a2000 (pa: 845b000)    vm_end: 7f932e4a3000    size: 1000
[ 2621.614662] vm_start: 7f932e4a3000 (pa: c8997000)    vm_end: 7f932e4a4000    size: 1000
[ 2621.614663] vm_start: 7f932e4a4000 (pa: acfb0000)    vm_end: 7f932e4a5000    size: 1000
[ 2621.614664] vm_start: 7ffe4966d000 (pa: ffffffffffffffff)    vm_end: 7ffe4968f000    size: 22000
[ 2621.614665] vm_start: 7ffe4979a000 (pa: ffffffffffffffff)    vm_end: 7ffe4979d000    size: 3000
[ 2621.614665] vm_start: 7ffe4979d000 (pa: ffffffffffffffff)    vm_end: 7ffe4979f000    size: 2000
```
由以上輸出加上下方 `gdb` 的輔助可確認: 
- code segment is shared
- data segment, BSS segment, heap segment, stack segment are not shared (on separate pages)  
    The start address of data segment is the same but the end address of it isn't; there are 2 global variables `char *global_str` with the same initialized value with different physical addresses.  
    **By implementation 2 (main2) we know that data segment is not shared.** Above result may comes from copy-on-write mechanism of `fork()` in linux.
    > [fork(2) — Linux manual page](https://man7.org/linux/man-pages/man2/fork.2.html)
- libc `/lib/x86_64-linux-gnu/libc-2.23.so` is shared, dynamic linker `/lib/x86_64-linux-gnu/ld-2.23.so` is uncertain (seems to be shared?)  
  **By implementation 2 (main2) we know that at least the first virtual memory area starts at the same page frame**

To sum up, only code segment and libraies are shared; others are not shared.

### Output of `info proc` in gdb
下方為 parent process virtual memory layout, 透過此輸出可知各 libraries 的位置 (順序與 virtual memory area size 皆為固定)

```
(gdb) info proc mapping
process 3995
Mapped address spaces:

          Start Addr           End Addr       Size     Offset objfile
            0x400000           0x401000     0x1000        0x0 /home/shang/repo/Linux-OS-projects/project2/main
            0x601000           0x602000     0x1000     0x1000 /home/shang/repo/Linux-OS-projects/project2/main
            0x602000           0x603000     0x1000     0x2000 /home/shang/repo/Linux-OS-projects/project2/main
           0x1bdf000          0x1c00000    0x21000        0x0 [heap]
      0x7f17788f3000     0x7f1778ab3000   0x1c0000        0x0 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7f1778ab3000     0x7f1778cb3000   0x200000   0x1c0000 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7f1778cb3000     0x7f1778cb7000     0x4000   0x1c0000 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7f1778cb7000     0x7f1778cb9000     0x2000   0x1c4000 /lib/x86_64-linux-gnu/libc-2.23.so
      0x7f1778cb9000     0x7f1778cbd000     0x4000        0x0 
      0x7f1778cbd000     0x7f1778ce3000    0x26000        0x0 /lib/x86_64-linux-gnu/ld-2.23.so
      0x7f1778ec4000     0x7f1778ec7000     0x3000        0x0 
      0x7f1778ee2000     0x7f1778ee3000     0x1000    0x25000 /lib/x86_64-linux-gnu/ld-2.23.so
      0x7f1778ee3000     0x7f1778ee4000     0x1000    0x26000 /lib/x86_64-linux-gnu/ld-2.23.so
      0x7f1778ee4000     0x7f1778ee5000     0x1000        0x0 
      0x7ffe967fb000     0x7ffe9681d000    0x22000        0x0 [stack]
      0x7ffe968ab000     0x7ffe968ae000     0x3000        0x0 [vvar]
      0x7ffe968ae000     0x7ffe968b0000     0x2000        0x0 [vdso]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0 [vsyscall]
```

## Implementation 2
main2.c is another simple implementation without `fork()`. The experiment is conducted by executed the program `main2` simultaneously in shells (calling `sleep(10000);`).  
[Link to source code](https://github.com/sShaAanGg/Linux-OS-projects/blob/main/project2/main2.c)
### Output 1 of `./main2` (from `gcc -g main2.c -o main`)
```
The address of char *str in main [stack]: 0x7ffc63cea8e0, pa: cbfdc8e0
The value of char *heap_str in main [heap]: 0xa04010, pa: cbfd4010
The address of global variable char *global_str [data]: 0x601078, pa: cbfc4078
The address of uninitialized variable char *BSS_str [BSS]: 0x601088, pa: cbfc4088

System call returned 0
```

### Output 2 of `./main2` (from `gcc -g main2.c -o main`)
```
The address of char *str in main [stack]: 0x7fffe2de1f20, pa: 6122f20
The value of char *heap_str in main [heap]: 0x169f010, pa: b047010
The address of global variable char *global_str [data]: 0x601078, pa: 3bb1b078
The address of uninitialized variable char *BSS_str [BSS]: 0x601088, pa: 3bb1b088

System call returned 0
```

### Output of `dmesg`
```
[ 2008.555969] start_code of mm_struct is 400000 (pa: 3c5f000)
[ 2008.555970] end_code of mm_struct is 400cd4 (pa: 3c5fcd4)
[ 2008.555971] start_data of mm_struct is 600e10 (pa: 126c8e10)
[ 2008.555971] end_data of mm_struct is 601080 (pa: 3bb1b080)
[ 2008.555972] start_brk of mm_struct is 169f000 (pa: b047000)
[ 2008.555972] brk of mm_struct is 16c0000 (pa: ffffffffffffffff)

[ 2008.555973] mmap_base of mm_struct is 7f90adcd9000 (pa: ffffffffffffffff)

[ 2008.555974] start_stack of mm_struct is 7fffe2de2030 (pa: 2fd0030)
[ 2008.555974] arg_start of mm_struct is 7fffe2de3e2a
[ 2008.555974] arg_end of mm_struct is 7fffe2de3e32
[ 2008.555975] env_start of mm_struct is 7fffe2de3e32
[ 2008.555975] env_end of mm_struct is 7fffe2de4ff0

[ 2008.555976] vm_start: 400000 (pa: 3c5f000)    vm_end: 401000    size: 1000
[ 2008.555977] vm_start: 600000 (pa: 126c8000)    vm_end: 601000    size: 1000
[ 2008.555977] vm_start: 601000 (pa: 3bb1b000)    vm_end: 602000    size: 1000
[ 2008.555978] vm_start: 169f000 (pa: b047000)    vm_end: 16c0000    size: 21000
[ 2008.555979] vm_start: 7f90ad6e7000 (pa: 11fe40000)    vm_end: 7f90ad8a7000    size: 1c0000
[ 2008.555979] vm_start: 7f90ad8a7000 (pa: ffffffffffffffff)    vm_end: 7f90adaa7000    size: 200000
[ 2008.555980] vm_start: 7f90adaa7000 (pa: b057000)    vm_end: 7f90adaab000    size: 4000
[ 2008.556006] vm_start: 7f90adaab000 (pa: 6120000)    vm_end: 7f90adaad000    size: 2000
[ 2008.556007] vm_start: 7f90adaad000 (pa: 4634000)    vm_end: 7f90adab1000    size: 4000
[ 2008.556008] vm_start: 7f90adab1000 (pa: 11fd3e000)    vm_end: 7f90adad7000    size: 26000
[ 2008.556009] vm_start: 7f90adcb8000 (pa: d2668000)    vm_end: 7f90adcbb000    size: 3000
[ 2008.556009] vm_start: 7f90adcd6000 (pa: 6121000)    vm_end: 7f90adcd7000    size: 1000
[ 2008.556010] vm_start: 7f90adcd7000 (pa: 17594000)    vm_end: 7f90adcd8000    size: 1000
[ 2008.556010] vm_start: 7f90adcd8000 (pa: 5d07000)    vm_end: 7f90adcd9000    size: 1000
[ 2008.556011] vm_start: 7fffe2dc3000 (pa: ffffffffffffffff)    vm_end: 7fffe2de5000    size: 22000
[ 2008.556012] vm_start: 7fffe2de7000 (pa: ffffffffffffffff)    vm_end: 7fffe2dea000    size: 3000
[ 2008.556013] vm_start: 7fffe2dea000 (pa: ceb96000)    vm_end: 7fffe2dec000    size: 2000
[ 2022.157672] start_code of mm_struct is 400000 (pa: 3c5f000)
[ 2022.157673] end_code of mm_struct is 400cd4 (pa: 3c5fcd4)
[ 2022.157673] start_data of mm_struct is 600e10 (pa: cbfece10)
[ 2022.157674] end_data of mm_struct is 601080 (pa: cbfc4080)
[ 2022.157674] start_brk of mm_struct is a04000 (pa: cbfd4000)
[ 2022.157675] brk of mm_struct is a25000 (pa: ffffffffffffffff)

[ 2022.157676] mmap_base of mm_struct is 7f8b7b5e7000 (pa: ffffffffffffffff)

[ 2022.157676] start_stack of mm_struct is 7ffc63cea9f0 (pa: cbfdc9f0)
[ 2022.157676] arg_start of mm_struct is 7ffc63cece2a
[ 2022.157677] arg_end of mm_struct is 7ffc63cece32
[ 2022.157677] env_start of mm_struct is 7ffc63cece32
[ 2022.157678] env_end of mm_struct is 7ffc63cedff0

[ 2022.157678] vm_start: 400000 (pa: 3c5f000)    vm_end: 401000    size: 1000
[ 2022.157679] vm_start: 600000 (pa: cbfec000)    vm_end: 601000    size: 1000
[ 2022.157680] vm_start: 601000 (pa: cbfc4000)    vm_end: 602000    size: 1000
[ 2022.157680] vm_start: a04000 (pa: cbfd4000)    vm_end: a25000    size: 21000
[ 2022.157681] vm_start: 7f8b7aff5000 (pa: 11fe40000)    vm_end: 7f8b7b1b5000    size: 1c0000
[ 2022.157682] vm_start: 7f8b7b1b5000 (pa: ffffffffffffffff)    vm_end: 7f8b7b3b5000    size: 200000
[ 2022.157683] vm_start: 7f8b7b3b5000 (pa: cbfdb000)    vm_end: 7f8b7b3b9000    size: 4000
[ 2022.157683] vm_start: 7f8b7b3b9000 (pa: d152a000)    vm_end: 7f8b7b3bb000    size: 2000
[ 2022.157684] vm_start: 7f8b7b3bb000 (pa: d152d000)    vm_end: 7f8b7b3bf000    size: 4000
[ 2022.157685] vm_start: 7f8b7b3bf000 (pa: 11fd3e000)    vm_end: 7f8b7b3e5000    size: 26000
[ 2022.157685] vm_start: 7f8b7b5c6000 (pa: cbfd9000)    vm_end: 7f8b7b5c9000    size: 3000
[ 2022.157686] vm_start: 7f8b7b5e4000 (pa: d152b000)    vm_end: 7f8b7b5e5000    size: 1000
[ 2022.157686] vm_start: 7f8b7b5e5000 (pa: cbfd2000)    vm_end: 7f8b7b5e6000    size: 1000
[ 2022.157687] vm_start: 7f8b7b5e6000 (pa: d151f000)    vm_end: 7f8b7b5e7000    size: 1000
[ 2022.157688] vm_start: 7ffc63ccc000 (pa: ffffffffffffffff)    vm_end: 7ffc63cee000    size: 22000
[ 2022.157688] vm_start: 7ffc63d68000 (pa: ffffffffffffffff)    vm_end: 7ffc63d6b000    size: 3000
[ 2022.157689] vm_start: 7ffc63d6b000 (pa: ceb96000)    vm_end: 7ffc63d6d000    size: 2000
```

- The result is almost the same as `main`. The different part is the physical address of **start_data** is different. Therefore, data segment is not shared.
- libc `/lib/x86_64-linux-gnu/libc-2.23.so` and dynamic linker `/lib/x86_64-linux-gnu/ld-2.23.so` are shared (at least the first virtual memory area starts at the same page frame)

To sum up, only code segment and libraies are shared; others are not shared.
