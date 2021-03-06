// Copyright (c) 2005, Google Inc.
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ---
// Author: Maxim Lifantsev (with design ideas by Sanjay Ghemawat)
//
//
// Module for detecing heap (memory) leaks.
//
// For full(er) information, see doc/heap_checker.html
//
// This module can be linked into programs with
// no slowdown caused by this unless you activate the leak-checker:
//
//    1. Set the environment variable HEAPCHEK to _type_ before
//       running the program.
//
// _type_ is usually "normal" but can also be "minimal", "strict", or
// "draconian".  (See the html file for other options, like 'local'.)
//
// After that, just run your binary.  If the heap-checker detects
// a memory leak at program-exit, it will print instructions on how
// to track down the leak.

#ifndef BASE_HEAP_CHECKER_H_
#define BASE_HEAP_CHECKER_H_

#include <sys/types.h>  // for size_t
#include <stdint.h>     // for uintptr_t
#include <stdarg.h>     // for va_list
#include <vector>

// Annoying stuff for windows -- makes sure clients can import these functions
#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif


// The class is thread-safe with respect to all the provided static methods,
// as well as HeapLeakChecker objects: they can be accessed by multiple threads.
class PERFTOOLS_DLL_DECL HeapLeakChecker {
 public:

  // ----------------------------------------------------------------------- //
  // Static functions for working with (whole-program) leak checking.

  // If heap leak checking is currently active in some mode
  // e.g. if leak checking was started (and is still active now)
  // due to HEAPCHECK=... defined in the environment.
  // The return value reflects iff HeapLeakChecker objects manually
  // constructed right now will be doing leak checking or nothing.
  // Note that we can go from active to inactive state during InitGoogle()
  // if FLAGS_heap_check gets set to "" by some code before/during InitGoogle().
  static bool IsActive();

  // Return pointer to the whole-program checker if it has been created
  // and NULL otherwise.
  // Once GlobalChecker() returns non-NULL that object will not disappear and
  // will be returned by all later GlobalChecker calls.
  // This is mainly to access BytesLeaked() and ObjectsLeaked() (see below)
  // for the whole-program checker after one calls NoGlobalLeaks()
  // or similar and gets false.
  static HeapLeakChecker* GlobalChecker();

  // Do whole-program leak check now (if it was activated for this binary);
  // return false only if it was activated and has failed.
  // The mode of the check is controlled by the command-line flags.
  // This method can be called repeatedly.
  // Things like GlobalChecker()->SameHeap() can also be called explicitly
  // to do the desired flavor of the check.
  static bool NoGlobalLeaks();

  // If whole-program checker if active,
  // cancel its automatic execution after main() exits.
  // This requires that some leak check (e.g. NoGlobalLeaks())
  // has been called at least once on the whole-program checker.
  static void CancelGlobalCheck();

  // ----------------------------------------------------------------------- //
  // Non-static functions for starting and doing leak checking.

  // Start checking and name the leak check performed.
  // The name is used in naming dumped profiles
  // and needs to be unique only within your binary.
  // It must also be a string that can be a part of a file name,
  // in particular not contain path expressions.
  explicit HeapLeakChecker(const char *name);

  // Destructor (verifies that some *NoLeaks or *SameHeap method
  // has been called at least once).
  ~HeapLeakChecker();

  // Return true iff the heap does not have more objects allocated
  // w.r.t. its state at the time of our construction.
  // This does full pprof heap change checking and reporting.
  // To detect tricky leaks it depends on correct working pprof implementation
  // referred by FLAGS_heap_profile_pprof.
  // (By 'tricky leaks' we mean a change of heap state that e.g. for SameHeap
  //  preserves the number of allocated objects and bytes
  //  -- see TestHeapLeakCheckerTrick in heap-checker_unittest.cc --
  //  and thus is not detected by BriefNoLeaks.)
  // CAVEAT: pprof will do no checking over stripped binaries
  // (our automatic test binaries are stripped)
  // NOTE: All *NoLeaks() and *SameHeap() methods can be called many times
  // to check for leaks at different end-points in program's execution.
  bool NoLeaks() { return DoNoLeaks(NO_LEAKS, USE_PPROF, PPROF_REPORT); }

  // Return true iff the heap does not seem to have more objects allocated
  // w.r.t. its state at the time of our construction
  // by looking at the number of objects & bytes allocated.
  // This also tries to do pprof reporting of detected leaks.
  bool QuickNoLeaks() { return DoNoLeaks(NO_LEAKS, USE_COUNTS, PPROF_REPORT); }

  // Return true iff the heap does not seem to have more objects allocated
  // w.r.t. its state at the time of our construction
  // by looking at the number of objects & bytes allocated.
  // This does not try to use pprof at all.
  bool BriefNoLeaks() { return DoNoLeaks(NO_LEAKS, USE_COUNTS, NO_REPORT); }

  // These are similar to their *NoLeaks counterparts,
  // but they in addition require no negative leaks,
  // i.e. the state of the heap must be exactly the same
  // as at the time of our construction.
  bool SameHeap() { return DoNoLeaks(SAME_HEAP, USE_PPROF, PPROF_REPORT); }
  bool QuickSameHeap()
    { return DoNoLeaks(SAME_HEAP, USE_COUNTS, PPROF_REPORT); }
  bool BriefSameHeap() { return DoNoLeaks(SAME_HEAP, USE_COUNTS, NO_REPORT); }

  // Detailed information about the number of leaked bytes and objects
  // (both of these can be negative as well).
  // These are available only after a *SameHeap or *NoLeaks
  // method has been called.
  // Note that it's possible for both of these to be zero
  // while SameHeap() or NoLeaks() returned false in case
  // of a heap state change that is significant
  // but preserves the byte and object counts.
  ssize_t BytesLeaked() const;
  ssize_t ObjectsLeaked() const;

  // ----------------------------------------------------------------------- //
  // Static helpers to make us ignore certain leaks.

  // Scoped helper class.  Should be allocated on the stack inside a
  // block of code.  Any heap allocations done in the code block
  // covered by the scoped object (including in nested function calls
  // done by the code block) will not be reported as leaks.  This is
  // the recommended replacement for the GetDisableChecksStart() and
  // DisableChecksToHereFrom() routines below.
  //
  // Example:
  //   void Foo() {
  //     HeapLeakChecker::Disabler disabler;
  //     ... code that allocates objects whose leaks should be ignored ...
  //   }
  //
  // REQUIRES: Destructor runs in same thread as constructor
  class Disabler {
   public:
    Disabler();
    ~Disabler();
   private:
    Disabler(const Disabler&);        // disallow copy
    void operator=(const Disabler&);  // and assign
  };

  // Ignore an object located at 'ptr' (can go at the start or into the object)
  // as well as all heap objects (transitively) referenced from it
  // for the purposes of heap leak checking.
  // If 'ptr' does not point to an active allocated object
  // at the time of this call, it is ignored;
  // but if it does, the object must not get deleted from the heap later on;
  // it must also be not already ignored at the time of this call.
  static void IgnoreObject(const void* ptr);

  // Undo what an earlier IgnoreObject() call promised and asked to do.
  // At the time of this call 'ptr' must point at or inside of an active
  // allocated object which was previously registered with IgnoreObject().
  static void UnIgnoreObject(const void* ptr);

  // ----------------------------------------------------------------------- //
  // Initialization; to be called from main() only.

  // Full starting of recommended whole-program checking.
  static void InternalInitStart();

  // ----------------------------------------------------------------------- //
  // Internal types defined in .cc

  class Allocator;
  struct RangeValue;

 private:

  // ----------------------------------------------------------------------- //
  // Various helpers

  // Type for DumpProfileLocked
  enum ProfileType { START_PROFILE, END_PROFILE };

  // Create the name of the heap profile file of profile_type;
  // to be deleted via Allocator::Free().
  char* MakeProfileNameLocked(ProfileType profile_type);

  // Helper for dumping start/end heap leak checking profiles
  // and getting the byte/object counts.
  void DumpProfileLocked(ProfileType profile_type, const void* self_stack_top,
                         size_t* alloc_bytes, size_t* alloc_objects);
  // Helper for constructors
  void Create(const char *name);

  // Types for DoNoLeaks and its helpers.
  enum CheckType { SAME_HEAP, NO_LEAKS };
  enum CheckFullness { USE_PPROF, USE_COUNTS };
  enum ReportMode { PPROF_REPORT, NO_REPORT };

  // Helpers for *NoLeaks and *SameHeap
  bool DoNoLeaks(CheckType check_type,
                 CheckFullness fullness,
                 ReportMode report_mode);
  struct LeakCheckResult;  // defined in .cc
  LeakCheckResult DoLeakCheckLocked();
  bool DoNoLeaksOnceLocked(CheckType check_type,
                           CheckFullness fullness,
                           ReportMode report_mode);

  // Handle a leak profile file: delete it if !keep_profiles_.
  void HandleProfile(ProfileType profile_type);
  void HandleProfileLocked(ProfileType profile_type);

  // These used to be public, but they are now deprecated.
  // Will remove entirely when all internal uses are fixed.
  // In the meantime, use friendship so the unittest can still test them.
  static void* GetDisableChecksStart();
  static void DisableChecksToHereFrom(const void* start_address);
  static void DisableChecksIn(const char* pattern);
  friend void RangeDisabledLeaks();
  friend void NamedTwoDisabledLeaks();
  friend void* RunNamedDisabledLeaks(void*);
  friend void TestHeapLeakCheckerNamedDisabling();
  friend int main(int, char**);


  // Helper for DisableChecksIn
  static void DisableChecksInLocked(const char* pattern);

  // Helper for DisableChecksToHereFrom
  static void DisableChecksFromToLocked(const void* start_address,
                                        const void* end_address,
                                        int max_depth);

  // Helper for DoNoLeaks to ignore all objects reachable from all live data
  static void IgnoreAllLiveObjectsLocked(const void* self_stack_top);

  // Callback we pass to ListAllProcessThreads (see thread_lister.h)
  // that is invoked when all threads of our process are found and stopped.
  // The call back does the things needed to ignore live data reachable from
  // thread stacks and registers for all our threads
  // as well as do other global-live-data ignoring
  // (via IgnoreNonThreadLiveObjectsLocked)
  // during the quiet state of all threads being stopped.
  // For the argument meaning see the comment by ListAllProcessThreads.
  // Here we only use num_threads and thread_pids, that ListAllProcessThreads
  // fills for us with the number and pids of all the threads of our process
  // it found and attached to.
  static int IgnoreLiveThreadsLocked(void* parameter,
                                     int num_threads,
                                     pid_t* thread_pids,
                                     va_list ap);

  // Helper for IgnoreAllLiveObjectsLocked and IgnoreLiveThreadsLocked
  // that we prefer to execute from IgnoreLiveThreadsLocked
  // while all threads are stopped.
  // This helper does live object discovery and ignoring
  // for all objects that are reachable from everything
  // not related to thread stacks and registers.
  static void IgnoreNonThreadLiveObjectsLocked();

  // Helper for IgnoreNonThreadLiveObjectsLocked and IgnoreLiveThreadsLocked
  // to discover and ignore all heap objects
  // reachable from currently considered live objects
  // (live_objects static global variable in out .cc file).
  // "name", "name2" are two strings that we print one after another
  // in a debug message to describe what kind of live object sources
  // are being used.
  static void IgnoreLiveObjectsLocked(const char* name, const char* name2);

  // Runs REGISTER_HEAPCHECK_CLEANUP cleanups and potentially
  // calls DoMainHeapCheck
  static void RunHeapCleanups();

  // Do the overall whole-program heap leak check if needed;
  // returns true when did the leak check.
  static bool DoMainHeapCheck();

  // Type of task for UseProcMapsLocked
  enum ProcMapsTask {
    RECORD_GLOBAL_DATA,
    DISABLE_LIBRARY_ALLOCS
  };

  // Success/Error Return codes for UseProcMapsLocked.
  enum ProcMapsResult {
    PROC_MAPS_USED,
    CANT_OPEN_PROC_MAPS,
    NO_SHARED_LIBS_IN_PROC_MAPS
  };

  // Read /proc/self/maps, parse it, and do the 'proc_maps_task' for each line.
  static ProcMapsResult UseProcMapsLocked(ProcMapsTask proc_maps_task);

  // A ProcMapsTask to disable allocations from 'library'
  // that is mapped to [start_address..end_address)
  // (only if library is a certain system library).
  static void DisableLibraryAllocsLocked(const char* library,
                                         uintptr_t start_address,
                                         uintptr_t end_address);

  // Return true iff "*ptr" points to a heap object
  // ("*ptr" can point at the start or inside of a heap object
  //  so that this works e.g. for pointers to C++ arrays, C++ strings,
  //  multiple-inherited objects, or pointers to members).
  // We also fill *object_size for this object then
  // and we move "*ptr" to point to the very start of the heap object.
  static inline bool HaveOnHeapLocked(const void** ptr, size_t* object_size);

  // Helper to shutdown heap leak checker when it's not needed
  // or can't function properly.
  static void TurnItselfOffLocked();

  // Internally-used c-tor to start whole-executable checking.
  HeapLeakChecker();

  // ----------------------------------------------------------------------- //
  // Friends and externally accessed helpers.

  // Helper for VerifyHeapProfileTableStackGet in the unittest
  // to get the recorded allocation caller for ptr,
  // which must be a heap object.
  static const void* GetAllocCaller(void* ptr);
  friend void VerifyHeapProfileTableStackGet();

  // This gets to execute before constructors for all global objects
  static void BeforeConstructorsLocked();
  friend void HeapLeakChecker_BeforeConstructors();

  // This gets to execute after destructors for all global objects
  friend void HeapLeakChecker_AfterDestructors();

  // ----------------------------------------------------------------------- //
  // Member data.

  class SpinLock* lock_;  // to make HeapLeakChecker objects thread-safe
  const char* name_;  // our remembered name (we own it)
                      // NULL means this leak checker is a noop
  size_t start_inuse_bytes_;  // bytes in use at our construction
  size_t start_inuse_allocs_;  // allocations in use at our construction
  bool has_checked_;  // if we have done the leak check, so these are ready:
  ssize_t inuse_bytes_increase_;  // bytes-in-use increase for this checker
  ssize_t inuse_allocs_increase_;  // allocations-in-use increase
                                   // for this checker
  bool keep_profiles_;  // iff we should keep the heap profiles we've made

  // ----------------------------------------------------------------------- //

  // Disallow "evil" constructors.
  HeapLeakChecker(const HeapLeakChecker&);
  void operator=(const HeapLeakChecker&);
};


// A class that exists solely to run its destructor.  This class should not be
// used directly, but instead by the REGISTER_HEAPCHECK_CLEANUP macro below.
class PERFTOOLS_DLL_DECL HeapCleaner {
 public:
  typedef void (*void_function)(void);
  HeapCleaner(void_function f);
  static void RunHeapCleanups();
 private:
  static std::vector<void_function>* heap_cleanups_;
};

// A macro to declare module heap check cleanup tasks
// (they run only if we are doing heap leak checking.)
// 'body' should be the cleanup code to run.  'name' doesn't matter,
// but must be unique amongst all REGISTER_HEAPCHECK_CLEANUP calls.
#define REGISTER_HEAPCHECK_CLEANUP(name, body)  \
  namespace { \
  void heapcheck_cleanup_##name() { body; } \
  static HeapCleaner heapcheck_cleaner_##name(&heapcheck_cleanup_##name); \
  }

#endif  // BASE_HEAP_CHECKER_H_
