This is a signal library for Lua 5.1. It depends on ANSI C signals and has
some extensions that are available in POSIX, such as kill().

The library should compile cleanly and easily as a shared object:

gcc -shared -o signal.so lsignal.c

This code is distributed under the same license as Lua 5.0. You may view
the license at the top of any of the source files.

===============================================================================
-----------------------------------API-----------------------------------------
===============================================================================

All of these functions are placed inside the signal table.

signal(sig, handler)
  sig = number or string representing the signal for the handler.
  handler = nil or a function, nil to clear a previous handler, and function
            to set the handler.

raise(sig)
  sig = number or string representing the signal for the handler.

========================

For POSIX compliant systems, the following are defined:

kill(pid, sig)
  pid = number representing the process to receive the signal.
  sig = number or string representing the signal to be sent.

========================

SIGNALS: Here are some common signals defined below, but the values can change
         depending on the system the library is compiled on. You can check
         all the available signals to you inside the signal library using
         this script: for k in pairs(signal) do print(k) end
         Making changes to those signals, or removing them, has no effect
         on the operation of the signal library. They are provided as a
         convenience and reference.

SIGHUP      1   /* Hangup (POSIX).  */
SIGINT      2   /* Interrupt (ANSI).  */
SIGQUIT     3   /* Quit (POSIX).  */
SIGILL      4   /* Illegal instruction (ANSI).  */
SIGTRAP     5   /* Trace trap (POSIX).  */
SIGABRT     6   /* Abort (ANSI).  */
SIGIOT      6   /* IOT trap (4.2 BSD).  */
SIGBUS      7   /* BUS error (4.2 BSD).  */
SIGFPE      8   /* Floating-point exception (ANSI).  */
SIGKILL     9   /* Kill, unblockable (POSIX).  */
SIGUSR1     10  /* User-defined signal 1 (POSIX).  */
SIGSEGV     11  /* Segmentation violation (ANSI).  */
SIGUSR2     12  /* User-defined signal 2 (POSIX).  */
SIGPIPE     13  /* Broken pipe (POSIX).  */
SIGALRM     14  /* Alarm clock (POSIX).  */
SIGTERM     15  /* Termination (ANSI).  */
SIGSTKFLT   16  /* Stack fault.  */
SIGCLD      SIGCHLD /* Same as SIGCHLD (System V).  */
SIGCHLD     17  /* Child status has changed (POSIX).  */
SIGCONT     18  /* Continue (POSIX).  */
SIGSTOP     19  /* Stop, unblockable (POSIX).  */
SIGTSTP     20  /* Keyboard stop (POSIX).  */
SIGTTIN     21  /* Background read from tty (POSIX).  */
SIGTTOU     22  /* Background write to tty (POSIX).  */
SIGURG      23  /* Urgent condition on socket (4.2 BSD).  */
SIGXCPU     24  /* CPU limit exceeded (4.2 BSD).  */
SIGXFSZ     25  /* File size limit exceeded (4.2 BSD).  */
SIGVTALRM   26  /* Virtual alarm clock (4.2 BSD).  */
SIGPROF     27  /* Profiling alarm clock (4.2 BSD).  */
SIGWINCH    28  /* Window size change (4.3 BSD, Sun).  */
SIGPOLL     SIGIO   /* Pollable event occurred (System V).  */
SIGIO       29  /* I/O now possible (4.2 BSD).  */
SIGPWR      30  /* Power failure restart (System V).  */
SIGSYS      31  /* Bad system call.  */
