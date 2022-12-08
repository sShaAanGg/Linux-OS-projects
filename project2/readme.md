# Project2
There are 2 implementations, one with `fork()` and `waitpid()` another with only `sleep(10000)`.
To check the experiment result for implementaion 2, execute the program (main2) in different shells.  

In summary, implementation 2 demonstrates the result better since it's much simpler.  
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
Parent process PID is 5381
Child process PID is 5382
The address of char *str in main [stack]: 0x7ffe39a8d198, pa: 10b544198
The value of char *heap_str in main [heap]: 0x952010, pa: 115e55010
The address of global variable char *global_str [data]: 0x602090, pa: 1089b0090
The address of uninitialized variable char *BSS_str [BSS]: 0x6020a0, pa: 1089b00a0

The address of function pointer main: 0x4008e4, pa: 2ef68e4
The address of function pointer printf: 0x4006f0, pa: 2ef66f0
The address of function pointer malloc: 0x400720, pa: 2ef6720

System call returned 0
The address of char *str in main [stack]: 0x7ffe39a8d198, pa: 10b623198
The value of char *heap_str in main [heap]: 0x952010, pa: 103a57010
The address of global variable char *global_str [data]: 0x602090, pa: 10688a090
The address of uninitialized variable char *BSS_str [BSS]: 0x6020a0, pa: 10688a0a0

The address of function pointer main: 0x4008e4, pa: 2ef68e4
The address of function pointer printf: 0x4006f0, pa: 2ef66f0
The address of function pointer malloc: 0x400720, pa: 2ef6720

System call returned 0
```
### Output of `dmesg`
```
[ 1586.780997] start_code of mm_struct is 400000 (pa: 2ef6000)
[ 1586.780998] end_code of mm_struct is 40117c (pa: 2ef717c)
[ 1586.780998] start_data of mm_struct is 601e10 (pa: 108f90e10)
[ 1586.780999] end_data of mm_struct is 602098 (pa: 1089b0098)
[ 1586.780999] start_brk of mm_struct is 952000 (pa: 115e55000)
[ 1586.781000] brk of mm_struct is 973000 (pa: ffffffffffffffff)

[ 1586.781001] mmap_base of mm_struct is 7f6da9064000 (pa: ffffffffffffffff)

[ 1586.781002] start_stack of mm_struct is 7ffe39a8d2b0 (pa: 10b5442b0)
[ 1586.781002] arg_start of mm_struct is 7ffe39a8de36
[ 1586.781002] arg_end of mm_struct is 7ffe39a8de3d
[ 1586.781003] env_start of mm_struct is 7ffe39a8de3d
[ 1586.781003] env_end of mm_struct is 7ffe39a8eff1

[ 1586.781004] vm_start: 400000 (pa: 2ef6000)    vm_end: 402000    size: 2000
[ 1586.781005] vm_start: 601000 (pa: 108f90000)    vm_end: 602000    size: 1000
[ 1586.781005] vm_start: 602000 (pa: 1089b0000)    vm_end: 603000    size: 1000
[ 1586.781006] vm_start: 952000 (pa: 115e55000)    vm_end: 973000    size: 21000
[ 1586.781007] vm_start: 7f6da8a72000 (pa: 11fe40000)    vm_end: 7f6da8c32000    size: 1c0000
[ 1586.781008] vm_start: 7f6da8c32000 (pa: ffffffffffffffff)    vm_end: 7f6da8e32000    size: 200000
[ 1586.781009] vm_start: 7f6da8e32000 (pa: 10b04d000)    vm_end: 7f6da8e36000    size: 4000
[ 1586.781010] vm_start: 7f6da8e36000 (pa: 114075000)    vm_end: 7f6da8e38000    size: 2000
[ 1586.781010] vm_start: 7f6da8e38000 (pa: 112116000)    vm_end: 7f6da8e3c000    size: 4000
[ 1586.781011] vm_start: 7f6da8e3c000 (pa: 11fd3e000)    vm_end: 7f6da8e62000    size: 26000
[ 1586.781012] vm_start: 7f6da9043000 (pa: 114074000)    vm_end: 7f6da9046000    size: 3000
[ 1586.781012] vm_start: 7f6da9061000 (pa: 11407a000)    vm_end: 7f6da9062000    size: 1000
[ 1586.781013] vm_start: 7f6da9062000 (pa: 1107da000)    vm_end: 7f6da9063000    size: 1000
[ 1586.781014] vm_start: 7f6da9063000 (pa: 1065a4000)    vm_end: 7f6da9064000    size: 1000
[ 1586.781014] vm_start: 7ffe39a6d000 (pa: ffffffffffffffff)    vm_end: 7ffe39a8f000    size: 22000
[ 1586.781015] vm_start: 7ffe39bdd000 (pa: ffffffffffffffff)    vm_end: 7ffe39be0000    size: 3000
[ 1586.781016] vm_start: 7ffe39be0000 (pa: b8196000)    vm_end: 7ffe39be2000    size: 2000
[ 1587.785976] start_code of mm_struct is 400000 (pa: 2ef6000)
[ 1587.785977] end_code of mm_struct is 40117c (pa: 2ef717c)
[ 1587.785977] start_data of mm_struct is 601e10 (pa: 108f90e10)
[ 1587.785978] end_data of mm_struct is 602098 (pa: 10688a098)
[ 1587.785978] start_brk of mm_struct is 952000 (pa: 103a57000)
[ 1587.785979] brk of mm_struct is 973000 (pa: ffffffffffffffff)

[ 1587.786004] mmap_base of mm_struct is 7f6da9064000 (pa: ffffffffffffffff)

[ 1587.786006] start_stack of mm_struct is 7ffe39a8d2b0 (pa: 10b6232b0)
[ 1587.786006] arg_start of mm_struct is 7ffe39a8de36
[ 1587.786007] arg_end of mm_struct is 7ffe39a8de3d
[ 1587.786007] env_start of mm_struct is 7ffe39a8de3d
[ 1587.786007] env_end of mm_struct is 7ffe39a8eff1

[ 1587.786008] vm_start: 400000 (pa: 2ef6000)    vm_end: 402000    size: 2000
[ 1587.786009] vm_start: 601000 (pa: 108f90000)    vm_end: 602000    size: 1000
[ 1587.786010] vm_start: 602000 (pa: 10688a000)    vm_end: 603000    size: 1000
[ 1587.786011] vm_start: 952000 (pa: 103a57000)    vm_end: 973000    size: 21000
[ 1587.786011] vm_start: 7f6da8a72000 (pa: 11fe40000)    vm_end: 7f6da8c32000    size: 1c0000
[ 1587.786012] vm_start: 7f6da8c32000 (pa: ffffffffffffffff)    vm_end: 7f6da8e32000    size: 200000
[ 1587.786013] vm_start: 7f6da8e32000 (pa: 10b04d000)    vm_end: 7f6da8e36000    size: 4000
[ 1587.786014] vm_start: 7f6da8e36000 (pa: 10b2a2000)    vm_end: 7f6da8e38000    size: 2000
[ 1587.786014] vm_start: 7f6da8e38000 (pa: 10c8f2000)    vm_end: 7f6da8e3c000    size: 4000
[ 1587.786015] vm_start: 7f6da8e3c000 (pa: ffffffffffffffff)    vm_end: 7f6da8e62000    size: 26000
[ 1587.786016] vm_start: 7f6da9043000 (pa: 114074000)    vm_end: 7f6da9046000    size: 3000
[ 1587.786016] vm_start: 7f6da9061000 (pa: 11407a000)    vm_end: 7f6da9062000    size: 1000
[ 1587.786017] vm_start: 7f6da9062000 (pa: 11006b000)    vm_end: 7f6da9063000    size: 1000
[ 1587.786018] vm_start: 7f6da9063000 (pa: 1065a4000)    vm_end: 7f6da9064000    size: 1000
[ 1587.786018] vm_start: 7ffe39a6d000 (pa: ffffffffffffffff)    vm_end: 7ffe39a8f000    size: 22000
[ 1587.786019] vm_start: 7ffe39bdd000 (pa: ffffffffffffffff)    vm_end: 7ffe39be0000    size: 3000
[ 1587.786020] vm_start: 7ffe39be0000 (pa: ffffffffffffffff)    vm_end: 7ffe39be2000    size: 2000
```
由以上輸出加上下方 `gdb` 的輔助可確認: 
- code segment is shared
- data segment, BSS segment, heap segment, stack segment are not shared (on separate pages)  
    The start address of data segment is the same but the end address of it isn't; there are 2 global variables `char *global_str` with the same initialized value with different physical addresses.  
    **By implementation 2 (main2) we know that data segment is not shared.** Above result may come from copy-on-write mechanism of `fork()` in linux.
    > [fork(2) — Linux manual page](https://man7.org/linux/man-pages/man2/fork.2.html)
- libc `/lib/x86_64-linux-gnu/libc-2.23.so` is shared, dynamic linker `/lib/x86_64-linux-gnu/ld-2.23.so` is uncertain (seems to be shared?)  
  **By implementation 2 (main2) we know that at least the first virtual memory area starts at the same page frame** and the code of libc like `printf()` and `malloc()` is shared.

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
The address of char *str in main [stack]: 0x7ffe0d284dc0, pa: 4afaadc0
The value of char *heap_str in main [heap]: 0x2520010, pa: 4db26010
The address of global variable char *global_str [data]: 0x601078, pa: 1c1d2078
The address of uninitialized variable char *BSS_str [BSS]: 0x601088, pa: 1c1d2088

The address of function pointer main: 0x400804, pa: 1ad8f804
The address of function pointer printf: 0x400630, pa: 1ad8f630
The address of function pointer malloc: 0x400660, pa: 1ad8f660

System call returned 0
```

### Output 2 of `./main2` (from `gcc -g main2.c -o main`)
```
The address of char *str in main [stack]: 0x7ffe3a986c20, pa: c0e70c20
The value of char *heap_str in main [heap]: 0x20fb010, pa: 63879010
The address of global variable char *global_str [data]: 0x601078, pa: ab5a0078
The address of uninitialized variable char *BSS_str [BSS]: 0x601088, pa: ab5a0088

The address of function pointer main: 0x400804, pa: 1ad8f804
The address of function pointer printf: 0x400630, pa: 1ad8f630
The address of function pointer malloc: 0x400660, pa: 1ad8f660

System call returned 0
```

### Output of `dmesg`
```
[  910.877455] start_code of mm_struct is 400000 (pa: 1ad8f000)
[  910.877457] end_code of mm_struct is 400e0c (pa: 1ad8fe0c)
[  910.877457] start_data of mm_struct is 600e10 (pa: 5b8d8e10)
[  910.877458] end_data of mm_struct is 601080 (pa: 1c1d2080)
[  910.877458] start_brk of mm_struct is 2520000 (pa: 4db26000)
[  910.877459] brk of mm_struct is 2541000 (pa: ffffffffffffffff)

[  910.877459] mmap_base of mm_struct is 7f7d9e394000 (pa: ffffffffffffffff)

[  910.877460] start_stack of mm_struct is 7ffe0d284ed0 (pa: 4afaaed0)
[  910.877460] arg_start of mm_struct is 7ffe0d285e33
[  910.877461] arg_end of mm_struct is 7ffe0d285e3b
[  910.877461] env_start of mm_struct is 7ffe0d285e3b
[  910.877462] env_end of mm_struct is 7ffe0d286ff0

[  910.877462] vm_start: 400000 (pa: 1ad8f000)    vm_end: 401000    size: 1000
[  910.877463] vm_start: 600000 (pa: 5b8d8000)    vm_end: 601000    size: 1000
[  910.877464] vm_start: 601000 (pa: 1c1d2000)    vm_end: 602000    size: 1000
[  910.877464] vm_start: 2520000 (pa: 4db26000)    vm_end: 2541000    size: 21000
[  910.877465] vm_start: 7f7d9dda2000 (pa: 11fe40000)    vm_end: 7f7d9df62000    size: 1c0000
[  910.877466] vm_start: 7f7d9df62000 (pa: ffffffffffffffff)    vm_end: 7f7d9e162000    size: 200000
[  910.877467] vm_start: 7f7d9e162000 (pa: 1bcc5000)    vm_end: 7f7d9e166000    size: 4000
[  910.877467] vm_start: 7f7d9e166000 (pa: cae1d000)    vm_end: 7f7d9e168000    size: 2000
[  910.877468] vm_start: 7f7d9e168000 (pa: 4565a000)    vm_end: 7f7d9e16c000    size: 4000
[  910.877469] vm_start: 7f7d9e16c000 (pa: 11fd3e000)    vm_end: 7f7d9e192000    size: 26000
[  910.877469] vm_start: 7f7d9e373000 (pa: 6a76c000)    vm_end: 7f7d9e376000    size: 3000
[  910.877470] vm_start: 7f7d9e391000 (pa: a6623000)    vm_end: 7f7d9e392000    size: 1000
[  910.877471] vm_start: 7f7d9e392000 (pa: 63e1c000)    vm_end: 7f7d9e393000    size: 1000
[  910.877471] vm_start: 7f7d9e393000 (pa: 752b6000)    vm_end: 7f7d9e394000    size: 1000
[  910.877472] vm_start: 7ffe0d265000 (pa: ffffffffffffffff)    vm_end: 7ffe0d287000    size: 22000
[  910.877473] vm_start: 7ffe0d3c5000 (pa: ffffffffffffffff)    vm_end: 7ffe0d3c8000    size: 3000
[  910.877474] vm_start: 7ffe0d3c8000 (pa: b8196000)    vm_end: 7ffe0d3ca000    size: 2000
[  917.948771] start_code of mm_struct is 400000 (pa: 1ad8f000)
[  917.948772] end_code of mm_struct is 400e0c (pa: 1ad8fe0c)
[  917.948802] start_data of mm_struct is 600e10 (pa: faa2e10)
[  917.948804] end_data of mm_struct is 601080 (pa: ab5a0080)
[  917.948804] start_brk of mm_struct is 20fb000 (pa: 63879000)
[  917.948805] brk of mm_struct is 211c000 (pa: ffffffffffffffff)

[  917.948805] mmap_base of mm_struct is 7fc50a914000 (pa: ffffffffffffffff)

[  917.948806] start_stack of mm_struct is 7ffe3a986d30 (pa: c0e70d30)
[  917.948806] arg_start of mm_struct is 7ffe3a988e33
[  917.948807] arg_end of mm_struct is 7ffe3a988e3b
[  917.948807] env_start of mm_struct is 7ffe3a988e3b
[  917.948807] env_end of mm_struct is 7ffe3a989ff0

[  917.948808] vm_start: 400000 (pa: 1ad8f000)    vm_end: 401000    size: 1000
[  917.948809] vm_start: 600000 (pa: faa2000)    vm_end: 601000    size: 1000
[  917.948810] vm_start: 601000 (pa: ab5a0000)    vm_end: 602000    size: 1000
[  917.948810] vm_start: 20fb000 (pa: 63879000)    vm_end: 211c000    size: 21000
[  917.948811] vm_start: 7fc50a322000 (pa: 11fe40000)    vm_end: 7fc50a4e2000    size: 1c0000
[  917.948812] vm_start: 7fc50a4e2000 (pa: ffffffffffffffff)    vm_end: 7fc50a6e2000    size: 200000
[  917.948812] vm_start: 7fc50a6e2000 (pa: 14332000)    vm_end: 7fc50a6e6000    size: 4000
[  917.948813] vm_start: 7fc50a6e6000 (pa: cfe5d000)    vm_end: 7fc50a6e8000    size: 2000
[  917.948814] vm_start: 7fc50a6e8000 (pa: 15f94000)    vm_end: 7fc50a6ec000    size: 4000
[  917.948814] vm_start: 7fc50a6ec000 (pa: 11fd3e000)    vm_end: 7fc50a712000    size: 26000
[  917.948815] vm_start: 7fc50a8f3000 (pa: cf885000)    vm_end: 7fc50a8f6000    size: 3000
[  917.948816] vm_start: 7fc50a911000 (pa: 16b89000)    vm_end: 7fc50a912000    size: 1000
[  917.948816] vm_start: 7fc50a912000 (pa: ce419000)    vm_end: 7fc50a913000    size: 1000
[  917.948817] vm_start: 7fc50a913000 (pa: b1f33000)    vm_end: 7fc50a914000    size: 1000
[  917.948818] vm_start: 7ffe3a968000 (pa: ffffffffffffffff)    vm_end: 7ffe3a98a000    size: 22000
[  917.948818] vm_start: 7ffe3a9b6000 (pa: ffffffffffffffff)    vm_end: 7ffe3a9b9000    size: 3000
[  917.948819] vm_start: 7ffe3a9b9000 (pa: b8196000)    vm_end: 7ffe3a9bb000    size: 2000
```

- The result is almost the same as `main`. The different part is the physical address of **start_data** is different. Therefore, data segment is not shared.
- libc `/lib/x86_64-linux-gnu/libc-2.23.so` and dynamic linker `/lib/x86_64-linux-gnu/ld-2.23.so` are shared (at least the first virtual memory area starts at the same page frame and the code of libc like `printf()` and `malloc()` is shared).

To sum up, only code segment and libraies are shared; others are not shared.
