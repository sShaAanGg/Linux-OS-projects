## Implementation

### Environment
Linux kernel version: 4.15.1
Distribution: Ubuntu 16.04.7
gcc: 5.4.0 

### References
> [fork(2) — Linux manual page](https://man7.org/linux/man-pages/man2/fork.2.html)  
> [wait(2) — Linux manual page](https://man7.org/linux/man-pages/man2/wait.2.html)  

### Output of `dmesg`
```
[ 9866.295609] start_code of mm_struct is 400000 (pa: 33dc8000)
[ 9866.295610] end_code of mm_struct is 400b34 (pa: 33dc8b34)
[ 9866.295611] start_data of mm_struct is 600e10 (pa: c1460e10)
[ 9866.295611] end_data of mm_struct is 601070 (pa: 8fe49070)
[ 9866.295612] start_brk of mm_struct is 1f23000 (pa: dc84000)
[ 9866.295613] brk of mm_struct is 1f44000 (pa: ffffffffffffffff)

[ 9866.295614] mmap_base of mm_struct is 7f9263cfb000 (pa: ffffffffffffffff)

[ 9866.295614] start_stack of mm_struct is 7ffddeaffc60 (pa: 65475c60)
[ 9866.295615] arg_start of mm_struct is 7ffddeb01e2d
[ 9866.295615] arg_end of mm_struct is 7ffddeb01e34
[ 9866.295615] env_start of mm_struct is 7ffddeb01e34
[ 9866.295616] env_end of mm_struct is 7ffddeb02ff1

<!-- [ 9866.295617] vm_start: 400000 (pa: 33dc8000)    vm_end: 401000    size: 1000
[ 9866.295617] vm_start: 600000 (pa: c1460000)    vm_end: 601000    size: 1000
[ 9866.295618] vm_start: 601000 (pa: 8fe49000)    vm_end: 602000    size: 1000
[ 9866.295619] vm_start: 1f23000 (pa: dc84000)    vm_end: 1f44000    size: 21000
[ 9866.295620] vm_start: 7f9263709000 (pa: 11fe40000)    vm_end: 7f92638c9000    size: 1c0000
[ 9866.295621] vm_start: 7f92638c9000 (pa: ffffffffffffffff)    vm_end: 7f9263ac9000    size: 200000
[ 9866.295621] vm_start: 7f9263ac9000 (pa: 90aa6000)    vm_end: 7f9263acd000    size: 4000
[ 9866.295622] vm_start: 7f9263acd000 (pa: 6c363000)    vm_end: 7f9263acf000    size: 2000
[ 9866.295623] vm_start: 7f9263acf000 (pa: c2c58000)    vm_end: 7f9263ad3000    size: 4000
[ 9866.295623] vm_start: 7f9263ad3000 (pa: 11fd3e000)    vm_end: 7f9263af9000    size: 26000
[ 9866.295624] vm_start: 7f9263cda000 (pa: 6e0d8000)    vm_end: 7f9263cdd000    size: 3000
[ 9866.295625] vm_start: 7f9263cf8000 (pa: 90ab5000)    vm_end: 7f9263cf9000    size: 1000
[ 9866.295625] vm_start: 7f9263cf9000 (pa: 6b740000)    vm_end: 7f9263cfa000    size: 1000
[ 9866.295626] vm_start: 7f9263cfa000 (pa: c9c59000)    vm_end: 7f9263cfb000    size: 1000
[ 9866.295627] vm_start: 7ffddeae1000 (pa: ffffffffffffffff)    vm_end: 7ffddeb03000    size: 22000
[ 9866.295628] vm_start: 7ffddeb07000 (pa: ffffffffffffffff)    vm_end: 7ffddeb0a000    size: 3000
[ 9866.295628] vm_start: 7ffddeb0a000 (pa: 3f196000)    vm_end: 7ffddeb0c000    size: 2000 -->
[ 9867.296473] start_code of mm_struct is 400000 (pa: 33dc8000)
[ 9867.296500] end_code of mm_struct is 400b34 (pa: 33dc8b34)
[ 9867.296501] start_data of mm_struct is 600e10 (pa: c1460e10)
[ 9867.296502] end_data of mm_struct is 601070 (pa: 90ab4070)
[ 9867.296503] start_brk of mm_struct is 1f23000 (pa: 49141000)
[ 9867.296503] brk of mm_struct is 1f44000 (pa: ffffffffffffffff)

[ 9867.296504] mmap_base of mm_struct is 7f9263cfb000 (pa: ffffffffffffffff)

[ 9867.296505] start_stack of mm_struct is 7ffddeaffc60 (pa: dc85c60)
[ 9867.296505] arg_start of mm_struct is 7ffddeb01e2d
[ 9867.296506] arg_end of mm_struct is 7ffddeb01e34
[ 9867.296506] env_start of mm_struct is 7ffddeb01e34
[ 9867.296506] env_end of mm_struct is 7ffddeb02ff1

<!-- [ 9867.296507] vm_start: 400000 (pa: 33dc8000)    vm_end: 401000    size: 1000
[ 9867.296508] vm_start: 600000 (pa: c1460000)    vm_end: 601000    size: 1000
[ 9867.296509] vm_start: 601000 (pa: 90ab4000)    vm_end: 602000    size: 1000
[ 9867.296510] vm_start: 1f23000 (pa: 49141000)    vm_end: 1f44000    size: 21000
[ 9867.296511] vm_start: 7f9263709000 (pa: 11fe40000)    vm_end: 7f92638c9000    size: 1c0000
[ 9867.296511] vm_start: 7f92638c9000 (pa: ffffffffffffffff)    vm_end: 7f9263ac9000    size: 200000
[ 9867.296512] vm_start: 7f9263ac9000 (pa: 90aa6000)    vm_end: 7f9263acd000    size: 4000
[ 9867.296513] vm_start: 7f9263acd000 (pa: 6d87f000)    vm_end: 7f9263acf000    size: 2000
[ 9867.296514] vm_start: 7f9263acf000 (pa: 90aa7000)    vm_end: 7f9263ad3000    size: 4000
[ 9867.296514] vm_start: 7f9263ad3000 (pa: 11fd3e000)    vm_end: 7f9263af9000    size: 26000
[ 9867.296515] vm_start: 7f9263cda000 (pa: 6e0d8000)    vm_end: 7f9263cdd000    size: 3000
[ 9867.296516] vm_start: 7f9263cf8000 (pa: 90ab5000)    vm_end: 7f9263cf9000    size: 1000
[ 9867.296516] vm_start: 7f9263cf9000 (pa: 6d87e000)    vm_end: 7f9263cfa000    size: 1000
[ 9867.296517] vm_start: 7f9263cfa000 (pa: c9c59000)    vm_end: 7f9263cfb000    size: 1000
[ 9867.296518] vm_start: 7ffddeae1000 (pa: ffffffffffffffff)    vm_end: 7ffddeb03000    size: 22000
[ 9867.296519] vm_start: 7ffddeb07000 (pa: ffffffffffffffff)    vm_end: 7ffddeb0a000    size: 3000
[ 9867.296519] vm_start: 7ffddeb0a000 (pa: ffffffffffffffff)    vm_end: 7ffddeb0c000    size: 2000 -->
```