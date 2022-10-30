## Some Notes
### gcc command option
Compile main.c into binary executable `gcc main.c -lpthread -o main`

Compile main.c into assembly language file `gcc main.c -lpthread -S`
### Thread-Local Storage Variable **__thread int thread_i**
The address of global variable thread_i in main() and start_routine() is different.
```
thread: t1
The value of thread_i in t1: 1 (address: 0x7fa05a8756fc)
The address of char *str in t1 [stack]: 0x7fa05a874ed8
The value of char *heap_str in t1 [heap]: 0x7fa054000b60

thread: main
The value of thread_i in main: 0 (address: 0x7fa05a87673c)
The address of char *str in main [stack]: 0x7fff95f1f670
The value of char *heap_str in main [heap]: 0x55c2d6858500

thread: t2
The value of thread_i in t2: 2 (address: 0x7fa05a0746fc)
The address of char *str in t2 [stack]: 0x7fa05a073ed8
The value of char *heap_str in t2 [heap]: 0x7fa04c000b60
```
### Memory Space Mapping
```
gef➤  vmmap
[ Legend:  Code | Heap | Stack ]
Start              End                Offset             Perm Path
0x0055c2d556b000 0x0055c2d556c000 0x00000000000000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x0055c2d556c000 0x0055c2d556d000 0x00000000001000 r-x /home/shang/repo/Linux-OS-projects/project1/main
0x0055c2d556d000 0x0055c2d556e000 0x00000000002000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x0055c2d556e000 0x0055c2d556f000 0x00000000002000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x0055c2d556f000 0x0055c2d5570000 0x00000000003000 rw- /home/shang/repo/Linux-OS-projects/project1/main
0x0055c2d6858000 0x0055c2d6879000 0x00000000000000 rw- [heap]

0x007fa04c000000 0x007fa04c021000 0x00000000000000 rw- **[heap for thread 2]**
0x007fa04c021000 0x007fa050000000 0x00000000000000 --- 
0x007fa054000000 0x007fa054021000 0x00000000000000 rw- **[heap for thread 1]**
0x007fa054021000 0x007fa058000000 0x00000000000000 --- 
0x007fa059859000 0x007fa05985c000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa05985c000 0x007fa05986e000 0x00000000003000 r-x /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa05986e000 0x007fa059872000 0x00000000015000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa059872000 0x007fa059873000 0x00000000018000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa059873000 0x007fa059874000 0x00000000019000 rw- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa059874000 0x007fa059875000 0x00000000000000 --- 
0x007fa059875000 0x007fa05a075000 0x00000000000000 rw- **[stack for thread 2]**
0x007fa05a075000 0x007fa05a076000 0x00000000000000 --- 
0x007fa05a076000 0x007fa05a879000 0x00000000000000 rw- **[stack for thread 1]**
0x007fa05a879000 0x007fa05a89b000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa05a89b000 0x007fa05aa13000 0x00000000022000 r-x /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa05aa13000 0x007fa05aa61000 0x0000000019a000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa05aa61000 0x007fa05aa65000 0x000000001e7000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa05aa65000 0x007fa05aa67000 0x000000001eb000 rw- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa05aa67000 0x007fa05aa6b000 0x00000000000000 rw- 
0x007fa05aa6b000 0x007fa05aa71000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa05aa71000 0x007fa05aa82000 0x00000000006000 r-x /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa05aa82000 0x007fa05aa88000 0x00000000017000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa05aa88000 0x007fa05aa89000 0x0000000001c000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa05aa89000 0x007fa05aa8a000 0x0000000001d000 rw- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa05aa8a000 0x007fa05aa90000 0x00000000000000 rw- 
0x007fa05aaa6000 0x007fa05aaa7000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa05aaa7000 0x007fa05aaca000 0x00000000001000 r-x /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa05aaca000 0x007fa05aad2000 0x00000000024000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa05aad3000 0x007fa05aad4000 0x0000000002c000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa05aad4000 0x007fa05aad5000 0x0000000002d000 rw- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa05aad5000 0x007fa05aad6000 0x00000000000000 rw- 
0x007fff95f00000 0x007fff95f21000 0x00000000000000 rw- [stack]
0x007fff95f46000 0x007fff95f4a000 0x00000000000000 r-- [vvar]
0x007fff95f4a000 0x007fff95f4b000 0x00000000000000 r-x [vdso]
```

