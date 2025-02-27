/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "uv.h"
#include "task.h"

#define NUM_TICKS 64

static uv_idle_t idle_handle;
static int idle_counter;


static void idle_cb(uv_idle_t* handle) {
  ASSERT(handle == &idle_handle);

  if (++idle_counter == NUM_TICKS)
    uv_idle_stop(handle);
}


TEST_IMPL(run_once) {
  uv_loop_t * loop = uv_default_loop();
  uv_idle_init(loop, &idle_handle);
  uv_idle_start(&idle_handle, idle_cb);

  int isok;
  while ((isok = uv_run(loop, UV_RUN_ONCE)));

  printf("isok %d", isok);

  ASSERT(idle_counter == NUM_TICKS);

  MAKE_VALGRIND_HAPPY();
  return 0;
}
