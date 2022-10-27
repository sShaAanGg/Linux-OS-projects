## Some Notes
### gcc command option
`gcc main.c -lpthread -o main`

### Thread-Local Storage Variable **__thread int thread_i**
The address of global variable thread_i in main() and start_routine() is different.
```
thread: t1
thread_i in t1: 1 (address: 0x7fa4b11516fc)
The address of char *str in t1: 0x7fa4b1150ee0

thread: t2
thread_i in t2: 2 (address: 0x7fa4b09506fc)
The address of char *str in t2: 0x7fa4b094fee0

thread: main
thread_i in main: 0 (address: 0x7fa4b115273c)
The address of char *str in main: 0x7ffd91da1958
```
### Memory Space Mapping
```
gef➤  vmmap
[ Legend:  Code | Heap | Stack ]
Start              End                Offset             Perm Path
0x0055ab1e8db000 0x0055ab1e8dc000 0x00000000000000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x0055ab1e8dc000 0x0055ab1e8dd000 0x00000000001000 r-x /home/shang/repo/Linux-OS-projects/project1/main
0x0055ab1e8dd000 0x0055ab1e8de000 0x00000000002000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x0055ab1e8de000 0x0055ab1e8df000 0x00000000002000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x0055ab1e8df000 0x0055ab1e8e0000 0x00000000003000 rw- /home/shang/repo/Linux-OS-projects/project1/main
0x0055ab2004e000 0x0055ab2006f000 0x00000000000000 rw- [heap]
0x007fa4ac000000 0x007fa4ac021000 0x00000000000000 rw- 
0x007fa4ac021000 0x007fa4b0000000 0x00000000000000 --- 
0x007fa4b0135000 0x007fa4b0138000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa4b0138000 0x007fa4b014a000 0x00000000003000 r-x /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa4b014a000 0x007fa4b014e000 0x00000000015000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa4b014e000 0x007fa4b014f000 0x00000000018000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa4b014f000 0x007fa4b0150000 0x00000000019000 rw- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007fa4b0150000 0x007fa4b0151000 0x00000000000000 --- 
0x007fa4b0151000 0x007fa4b0951000 0x00000000000000 rw- 
0x007fa4b0951000 0x007fa4b0952000 0x00000000000000 --- 
0x007fa4b0952000 0x007fa4b1155000 0x00000000000000 rw- 
0x007fa4b1155000 0x007fa4b1177000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa4b1177000 0x007fa4b12ef000 0x00000000022000 r-x /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa4b12ef000 0x007fa4b133d000 0x0000000019a000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa4b133d000 0x007fa4b1341000 0x000000001e7000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa4b1341000 0x007fa4b1343000 0x000000001eb000 rw- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007fa4b1343000 0x007fa4b1347000 0x00000000000000 rw- 
0x007fa4b1347000 0x007fa4b134d000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa4b134d000 0x007fa4b135e000 0x00000000006000 r-x /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa4b135e000 0x007fa4b1364000 0x00000000017000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa4b1364000 0x007fa4b1365000 0x0000000001c000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa4b1365000 0x007fa4b1366000 0x0000000001d000 rw- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007fa4b1366000 0x007fa4b136c000 0x00000000000000 rw- 
0x007fa4b1382000 0x007fa4b1383000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa4b1383000 0x007fa4b13a6000 0x00000000001000 r-x /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa4b13a6000 0x007fa4b13ae000 0x00000000024000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa4b13af000 0x007fa4b13b0000 0x0000000002c000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa4b13b0000 0x007fa4b13b1000 0x0000000002d000 rw- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007fa4b13b1000 0x007fa4b13b2000 0x00000000000000 rw- 
0x007ffd91d82000 0x007ffd91da3000 0x00000000000000 rw- [stack]
0x007ffd91dc7000 0x007ffd91dcb000 0x00000000000000 r-- [vvar]
0x007ffd91dcb000 0x007ffd91dcc000 0x00000000000000 r-x [vdso]
```

