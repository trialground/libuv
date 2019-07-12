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

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define NUM_TICKS 64


int open_cb_count = 0;
#define OUTPUT_SIZE 1024
static char output[OUTPUT_SIZE];

void open_noent_cb(uv_fs_t* req) {
  ASSERT(req->fs_type == UV_FS_OPEN);
  open_cb_count++;
  uv_buf_t buf;
  buf = uv_buf_init(output, sizeof(output));
  uv_fs_read(uv_default_loop(), req, req->result, &buf, 1, 0, NULL);
  ASSERT(strcmp(buf.base, "## hello world") == 0);
}

TEST_IMPL(my_test) {
  uv_fs_t req;
  // uv_fs_t req2;
  // uv_fs_t req3;
  int r;

  uv_loop_t * loop = uv_default_loop();

  r = uv_fs_open(loop, &req, "doc.md", O_RDONLY, 0, open_noent_cb);
  ASSERT(r == 0);


  ASSERT(open_cb_count == 0);
  uv_run(loop, UV_RUN_DEFAULT);
  ASSERT(open_cb_count == 1);
  /* TODO add EACCES test */

  MAKE_VALGRIND_HAPPY();
  return 0;
}
