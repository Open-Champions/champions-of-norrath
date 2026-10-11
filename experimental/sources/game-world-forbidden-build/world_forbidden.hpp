#pragma once
#include "find_height.hpp"
static_assert(__FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129,
              "established target single-float representation");
extern "C" {
// Separate actual writable four-byte view0x480028. It is not height0x480024
// or list-grid0x48001c. No source owner or copied initializer is supplied.
extern unsigned char norrath_forbidden_orientation_scale_bytes[];
// Observed original worldA0, Point12-storageA1, radiusF12, float-resultF0.
float norrath_world_test_forbidden(unsigned char* world,
                                  const unsigned char* point, float radius);
}
