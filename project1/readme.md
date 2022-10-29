## Some Notes
### gcc command option
Compile main.c into binary executable `gcc main.c -lpthread -o main`

Compile main.c into assembly language file `gcc main.c -lpthread -S`
### Thread-Local Storage Variable **__thread int thread_i**
The address of global variable thread_i in main() and start_routine() is different.
```
thread: main
The value of thread_i in main: 0 (address: 0x7f8db4e3c73c)
The address of char *str in main: 0x7fffd037b498

thread: t2
The value of thread_i in t2: 2 (address: 0x7f8db463a6fc)
The address of char *str in t2: 0x7f8db4639ee0
The address of char *heap_str in t2: 0x7f8db4639ed8
The address of global variable char *global_str: 0x563e0e637010

thread: t1
The value of thread_i in t1: 1 (address: 0x7f8db4e3b6fc)
The address of char *str in t1: 0x7f8db4e3aee0
The address of char *heap_str in t1: 0x7f8db4e3aed8
The address of global variable char *global_str: 0x563e0e637010
```
### Memory Space Mapping
```
gef➤  vmmap
[ Legend:  Code | Heap | Stack ]
Start              End                Offset             Perm Path
0x00563e0e633000 0x00563e0e634000 0x00000000000000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x00563e0e634000 0x00563e0e635000 0x00000000001000 r-x /home/shang/repo/Linux-OS-projects/project1/main
0x00563e0e635000 0x00563e0e636000 0x00000000002000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x00563e0e636000 0x00563e0e637000 0x00000000002000 r-- /home/shang/repo/Linux-OS-projects/project1/main
0x00563e0e637000 0x00563e0e638000 0x00000000003000 rw- /home/shang/repo/Linux-OS-projects/project1/main
0x00563e10589000 0x00563e105aa000 0x00000000000000 rw- [heap]
0x007f8da4000000 0x007f8da4021000 0x00000000000000 rw- 
0x007f8da4021000 0x007f8da8000000 0x00000000000000 --- 
0x007f8dac000000 0x007f8dac021000 0x00000000000000 rw- 
0x007f8dac021000 0x007f8db0000000 0x00000000000000 --- 
0x007f8db3e1f000 0x007f8db3e22000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007f8db3e22000 0x007f8db3e34000 0x00000000003000 r-x /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007f8db3e34000 0x007f8db3e38000 0x00000000015000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007f8db3e38000 0x007f8db3e39000 0x00000000018000 r-- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007f8db3e39000 0x007f8db3e3a000 0x00000000019000 rw- /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
0x007f8db3e3a000 0x007f8db3e3b000 0x00000000000000 --- 
0x007f8db3e3b000 0x007f8db463b000 0x00000000000000 rw- 
0x007f8db463b000 0x007f8db463c000 0x00000000000000 --- 
0x007f8db463c000 0x007f8db4e3f000 0x00000000000000 rw- 
0x007f8db4e3f000 0x007f8db4e61000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007f8db4e61000 0x007f8db4fd9000 0x00000000022000 r-x /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007f8db4fd9000 0x007f8db5027000 0x0000000019a000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007f8db5027000 0x007f8db502b000 0x000000001e7000 r-- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007f8db502b000 0x007f8db502d000 0x000000001eb000 rw- /usr/lib/x86_64-linux-gnu/libc-2.31.so
0x007f8db502d000 0x007f8db5031000 0x00000000000000 rw- 
0x007f8db5031000 0x007f8db5037000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007f8db5037000 0x007f8db5048000 0x00000000006000 r-x /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007f8db5048000 0x007f8db504e000 0x00000000017000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007f8db504e000 0x007f8db504f000 0x0000000001c000 r-- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007f8db504f000 0x007f8db5050000 0x0000000001d000 rw- /usr/lib/x86_64-linux-gnu/libpthread-2.31.so
0x007f8db5050000 0x007f8db5056000 0x00000000000000 rw- 
0x007f8db506c000 0x007f8db506d000 0x00000000000000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007f8db506d000 0x007f8db5090000 0x00000000001000 r-x /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007f8db5090000 0x007f8db5098000 0x00000000024000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007f8db5099000 0x007f8db509a000 0x0000000002c000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007f8db509a000 0x007f8db509b000 0x0000000002d000 rw- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x007f8db509b000 0x007f8db509c000 0x00000000000000 rw- 
0x007fffd035d000 0x007fffd037e000 0x00000000000000 rw- [stack]
0x007fffd03e7000 0x007fffd03eb000 0x00000000000000 r-- [vvar]
0x007fffd03eb000 0x007fffd03ec000 0x00000000000000 r-x [vdso]
```

