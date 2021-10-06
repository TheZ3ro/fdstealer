# fdstealer
dummy PoC to write into fds from other pids

### usage
```
gcc fdstealer.c -o fdstealer
./fdstealer 35452 3 stolen
```

### why?

> As of 2020, you cannot do that (or anything similar) if /proc/<pid>/fd/<fd> is a socket.
  
https://unix.stackexchange.com/a/613450


So I wanted to try out some Linux syscalls, namely:

**pidfd_open** - obtain a file descriptor that refers to a process  
**pidfd_getfd** - obtain a duplicate of another process's file descriptor  
**write** - write to a file descriptor

