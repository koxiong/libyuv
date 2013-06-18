/*
 *  Copyright 2013 The LibYuv Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS. All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <stdlib.h>
#include <string.h>

#include "libyuv/basic_types.h"
#include "libyuv/cpu_id.h"
#include "libyuv/row.h"
#include "../unit_test/unit_test.h"

namespace libyuv {

TEST_F(libyuvTest, TestFixedDiv) {
  int num[256];
  int div[256];
  int result_opt[256];
  int result_c[256];

  srandom(time(NULL));
  MemRandomize(reinterpret_cast<uint8*>(&num[0]), sizeof(num));
  MemRandomize(reinterpret_cast<uint8*>(&div[0]), sizeof(div));
  for (int j = 0; j < 256; ++j) {
    if (div[j] == 0) {
      div[j] = 1280;
    }
  }
  for (int i = 0; i < benchmark_pixels_div256_; ++i) {
    for (int j = 0; j < 256; ++j) {
      result_opt[j] = libyuv::FixedDiv(num[j], div[j]);
    }
  }
  for (int j = 0; j < 256; ++j) {
    result_c[j] = libyuv::FixedDiv_C(num[j], div[j]);
    EXPECT_NEAR(result_c[j], result_opt[j], 1);
  }
}

TEST_F(libyuvTest, TestFixedDiv_Opt) {
  int num[256];
  int div[256];
  int result_opt[256];
  int result_c[256];

  EXPECT_EQ(0x20000, libyuv::FixedDiv(640 * 2, 640));
  EXPECT_EQ(0x30000, libyuv::FixedDiv(640 * 3, 640));
  EXPECT_EQ(0x40000, libyuv::FixedDiv(640 * 4, 640));
  EXPECT_EQ(0x50000, libyuv::FixedDiv(640 * 5, 640));
  EXPECT_EQ(0x60000, libyuv::FixedDiv(640 * 6, 640));
  EXPECT_EQ(0x70000, libyuv::FixedDiv(640 * 7, 640));
  EXPECT_EQ(0x80000, libyuv::FixedDiv(640 * 8, 640));
  EXPECT_EQ(0xa0000, libyuv::FixedDiv(640 * 10, 640));
  EXPECT_EQ(0x20000, libyuv::FixedDiv(960 * 2, 960));
  EXPECT_EQ(0x08000, libyuv::FixedDiv(640 / 2, 640));
  EXPECT_EQ(0x04000, libyuv::FixedDiv(640 / 4, 640));
  EXPECT_EQ(0x20000, libyuv::FixedDiv(1080 * 2, 1080));

  srandom(time(NULL));
  MemRandomize(reinterpret_cast<uint8*>(&num[0]), sizeof(num));
  MemRandomize(reinterpret_cast<uint8*>(&div[0]), sizeof(div));
  for (int j = 0; j < 256; ++j) {
    num[j] &= 4095;  // Make numerator smaller.
    div[j] &= 4095;  // Make divisor smaller.
    if (div[j] == 0) {
      div[j] = 1280;
    }
  }

  int has_x86 = TestCpuFlag(kCpuHasX86);
  for (int i = 0; i < benchmark_pixels_div256_; ++i) {
    if (has_x86) {
      for (int j = 0; j < 256; ++j) {
        result_opt[j] = libyuv::FixedDiv(num[j], div[j]);
      }
    } else {
      for (int j = 0; j < 256; ++j) {
        result_opt[j] = libyuv::FixedDiv_C(num[j], div[j]);
      }
    }
  }
  for (int j = 0; j < 256; ++j) {
    result_c[j] = libyuv::FixedDiv_C(num[j], div[j]);
    EXPECT_NEAR(result_c[j], result_opt[j], 1);
  }
}

}  // namespace libyuv
