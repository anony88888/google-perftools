/* A manual version of config.h fit for windows machines. */
#ifndef GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H__
#define GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H__

/* the location of <hash_map> */
#define HASH_MAP_H  <hash_map>

/* the namespace of hash_map/hash_set */
#define HASH_NAMESPACE  stdext

/* the location of <hash_set> */
#define HASH_SET_H  <hash_set>

/* Define to 1 if compiler supports __builtin_stack_pointer */
#undef HAVE_BUILTIN_STACK_POINTER

/* Define to 1 if you have the <conflict-signal.h> header file. */
#undef HAVE_CONFLICT_SIGNAL_H

/* Define to 1 if you have the declaration of `cfree', and to 0 if you don't.
   */
#undef HAVE_DECL_CFREE

/* Define to 1 if you have the declaration of `memalign', and to 0 if you
   don't. */
#undef HAVE_DECL_MEMALIGN

/* Define to 1 if you have the declaration of `posix_memalign', and to 0 if
   you don't. */
#undef HAVE_DECL_POSIX_MEMALIGN

/* Define to 1 if you have the declaration of `pvalloc', and to 0 if you
   don't. */
#undef HAVE_DECL_PVALLOC

/* Define to 1 if you have the declaration of `uname', and to 0 if you don't.
   */
#undef HAVE_DECL_UNAME

/* Define to 1 if you have the declaration of `valloc', and to 0 if you don't.
   */
#undef HAVE_DECL_VALLOC

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you have the <execinfo.h> header file. */
#undef HAVE_EXECINFO_H

/* Define to 1 if you have the `geteuid' function. */
#undef HAVE_GETEUID

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1   /* we define it in windows/port.cc */

/* Define to 1 if you have the <glob.h> header file. */
#undef HAVE_GLOB_H

/* Define to 1 if you have the <grp.h> header file. */
#undef HAVE_GRP_H

/* define if the compiler has hash_map */
#define HAVE_HASH_MAP 1

/* define if the compiler has hash_set */
#define HAVE_HASH_SET 1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the <libunwind.h> header file. */
#undef HAVE_LIBUNWIND_H

/* Define to 1 if you have the <linux/ptrace.h> header file. */
#undef HAVE_LINUX_PTRACE_H

/* Define to 1 if you have the <malloc.h> header file. */
#undef HAVE_MALLOC_H

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have a working `mmap' system call. */
#undef HAVE_MMAP

/* define if the compiler implements namespaces */
#define HAVE_NAMESPACES 1

/* define if libc has program_invocation_name */
#undef HAVE_PROGRAM_INVOCATION_NAME

/* Define if you have POSIX threads libraries and header files. */
#undef HAVE_PTHREAD

/* Define to 1 if you have the <pwd.h> header file. */
#undef HAVE_PWD_H

/* Define to 1 if you have the `sbrk' function. */
#undef HAVE_SBRK

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if the system has the type `struct mallinfo'. */
#undef HAVE_STRUCT_MALLINFO

/* Define to 1 if `eip' is member of `struct sigcontext'. */
#undef HAVE_STRUCT_SIGCONTEXT_EIP

/* Define to 1 if `regs->nip' is member of `struct sigcontext'. */
#undef HAVE_STRUCT_SIGCONTEXT_REGS__NIP

/* Define to 1 if `rip' is member of `struct sigcontext'. */
#undef HAVE_STRUCT_SIGCONTEXT_RIP

/* Define to 1 if `sc_eip' is member of `struct sigcontext'. */
#undef HAVE_STRUCT_SIGCONTEXT_SC_EIP

/* Define to 1 if `sc_ip' is member of `struct sigcontext'. */
#undef HAVE_STRUCT_SIGCONTEXT_SC_IP

/* Define to 1 if `sc_ir' is member of `struct sigcontext'. */
#undef HAVE_STRUCT_SIGCONTEXT_SC_IR

/* Define to 1 if `si_faddr' is member of `struct siginfo'. */
#undef HAVE_STRUCT_SIGINFO_SI_FADDR

/* Define to 1 if `uc_mcontext' is member of `struct ucontext'. */
#undef HAVE_STRUCT_UCONTEXT_UC_MCONTEXT

/* Define to 1 if you have the <syscall.h> header file. */
#undef HAVE_SYSCALL_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if compiler supports __thread */
#define HAVE_TLS 1

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the <unwind.h> header file. */
#undef HAVE_UNWIND_H

/* define if your compiler has __attribute__ */
#undef HAVE___ATTRIBUTE__

/* Define to 1 if the system has the type `__int64'. */
#define HAVE___INT64 1

/* prefix where we look for installed files */
#undef INSTALL_PREFIX

/* Define to 1 if int32_t is equivalent to intptr_t */
#undef INT32_EQUALS_INTPTR

/* Name of package */
#undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* Always the empty-string on non-windows systems. On windows, should be
   "__declspec(dllexport)". This way, when we compile the dll, we export our
   functions/classes. It's safe to define this here because config.h is only
   used internally, to compile the DLL, and every DLL source file #includes
   "config.h" before anything else. */
#define PERFTOOLS_DLL_DECL  __declspec(dllexport)

/* printf format code for printing a size_t and ssize_t */
#define PRIdS  "Id"

/* printf format code for printing a size_t and ssize_t */
#define PRIuS  "Iu"

/* printf format code for printing a size_t and ssize_t */
#define PRIxS  "Ix"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
#undef PTHREAD_CREATE_JOINABLE

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* the namespace where STL code like vector<> is defined */
#define STL_NAMESPACE  std

/* Version number of package */
#undef VERSION

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

// ---------------------------------------------------------------------
// Extra stuff not found in config.h.in

// This must be defined before the windows.h is included.  It's needed
// for mutex.h, to give access to the TryLock method.
#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0400
#endif

// TODO(csilvers): include windows/port.h in every relevant source file instead?
#include "windows/port.h"

// windows/port.h defines compatibility APIs for several .h files, which
// we therefore shouldn't be #including directly.  This hack keeps us from
// doing so.  TODO(csilvers): do something more principled.
#define BASE_SPINLOCK_H__ 1
#define GOOGLE_MAYBE_THREADS_H__ 1

#endif  /* GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H__ */
