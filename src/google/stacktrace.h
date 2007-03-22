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
// Author: Sanjay Ghemawat
//
// Routine to extract the current stack trace.

#ifndef _GOOGLE_STACKTRACE_H
#define _GOOGLE_STACKTRACE_H

// Skip the most recent "skip_count" stack frames (also skips the
// frame generated for the "GetStackTrace" routine itself), and then
// record the pc values for upto the next "max_depth" frames in
// "result".  Returns the number of values recorded in "result".
//
// Example:
//      main() { foo(); }
//      foo() { bar(); }
//      bar() {
//        void* result[10];
//        int depth = GetStackTrace(result, 10, 1);
//      }
//
// The GetStackTrace call will skip the frame for "bar".  It will
// return 2 and will produce pc values that map to the following
// procedures:
//      result[0]       foo
//      result[1]       main
// (Actually, there may be a few more entries after "main" to account for
// startup procedures.)
//
// This routine currently produces non-empty stack traces only for
// Linux/x86 machines.
extern int GetStackTrace(void** result, int max_depth, int skip_count);

// Compute the extent of the function call stack by traversing it up.
// Input: "sp" is either NULL, or is a stack pointer
// (e.g., a value of the ebp register for x86).
// If "sp == NULL", the stack pointer for the current thread is implied.
//
// Stores the range of addresses covered by the specified stack
// in *stack_top and *stack_bottom.  Returns true if successful,
// false on failure (e.g., an inability to walk the stack).
//
// If it returns true, *stack_top and *stack_bottom respectively correspond
// to the most-recetly pushed frame of the call stack
// and the intial frame that started the call stack.
// Their relative ordering as integers though
// depends on the underlying machine's architecture.
extern bool GetStackExtent(void* sp,
                           void** stack_top,
                           void** stack_bottom);

#endif /* _GOOGLE_STACKTRACE_H */
