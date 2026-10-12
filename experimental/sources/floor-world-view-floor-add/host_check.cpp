#include <cstdio>
#include <cstring>

extern "C" void floorAdd__FG6Point3sfii(const unsigned char* point, int type_word, float extent,
                                        int kind, int flags);

namespace {
int calls = 0;
unsigned char seen_point[12];
int seen_type = 0;
float seen_extent = 0.0f;
int seen_kind = 0;
int seen_flags = 0;
}

extern "C" void norrath_floor_add(const unsigned char* point, int type_word, float extent,
                                  int kind, int flags) {
    ++calls;
    std::memcpy(seen_point, point, 12);
    seen_type = type_word;
    seen_extent = extent;
    seen_kind = kind;
    seen_flags = flags;
}

int main() {
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    floorAdd__FG6Point3sfii(point, 0x10005, 2.5f, 10, 2);
    if (calls != 1) return 1;
    if (std::memcmp(seen_point, point, 12) != 0) return 2;
    if (seen_type != 0x10005 || seen_extent != 2.5f || seen_kind != 10 || seen_flags != 2) return 3;
    return 0;
}
