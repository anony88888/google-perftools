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
// Author: Paul Menage
//
// TODO(menage) Turn this into a real unittest ...

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "google/malloc_extension.h"

#define BUFSIZE (100 << 10)

int main(int argc, char **argv) {
  char *buf1 = (char *)malloc(BUFSIZE);
  memset(buf1, 0, BUFSIZE);
  printf("Allocated buf1 via malloc() at %p\n", buf1);

  char *buf2 = new char[BUFSIZE];
  memset(buf2, 0, BUFSIZE);
  printf("Allocated buf2 via new at %p\n", buf2);
  
  free(buf1);
  delete[] buf2;

  char buffer[10 << 10];
  MallocExtension::instance()->GetStats(buffer, sizeof(buffer));
  printf("Malloc stats:\n%s\n", buffer);

  return 0;
}
