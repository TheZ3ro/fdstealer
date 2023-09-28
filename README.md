# fdstealer
dummy PoC to write into fds from other pids

### usage
```
gcc fdstealer.c -o fdstealer
./fdstealer 35452 3 stolen
```

### why?

> As of 2020, you cannot do that (or anything similar) if /proc/\*/fd/\* is a socket.
  
https://unix.stackexchange.com/a/613450


So I wanted to try out some Linux syscalls, namely:

**pidfd_open** - obtain a file descriptor that refers to a process  
**pidfd_getfd** - obtain a duplicate of another process's file descriptor  
**write** - write to a file descriptor


### Yama - Linux Security Module

Modern Linux distributions include or enable the [**Yama** security module](https://www.kernel.org/doc/Documentation/security/Yama.txt).  
This module restict `ptrace` capabilities of processes on the system.  
Specifically the value of the `/proc/sys/kernel/yama` file:

0 - *classic ptrace permissions*: a process can `PTRACE_ATTACH` to any other  
    process running under the same uid, as long as it is dumpable (i.e.  
    did not transition uids, start privileged, or have called  
    `prctl(PR_SET_DUMPABLE...)` already). Similarly, `PTRACE_TRACEME` is  
    unchanged.

1 - *restricted ptrace*: a process must have a predefined relationship  
    with the inferior it wants to call `PTRACE_ATTACH` on. By default,  
    this relationship is that of only its descendants when the above  
    classic criteria is also met. To change the relationship, an  
    inferior can call `prctl(PR_SET_PTRACER, debugger, ...)` to declare  
    an allowed debugger `PID` to call `PTRACE_ATTACH` on the inferior.  
    Using `PTRACE_TRACEME` is unchanged.

2 - *admin-only attach*: only processes with `CAP_SYS_PTRACE` may use `ptrace`  
    with `PTRACE_ATTACH`, or through children calling `PTRACE_TRACEME`.

3 - *no attach*: no processes may use ptrace with `PTRACE_ATTACH` nor via  
    `PTRACE_TRACEME`. Once set, this sysctl value cannot be changed.
