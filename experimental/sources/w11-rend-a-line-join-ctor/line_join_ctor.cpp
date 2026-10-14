#include "../w11-rend-a-common/rend.hpp"
// Observed __4LineP4LineT1i at 0x00279300, 364 bytes: the Line constructor taking two existing
// lines and an int (used by gameLineFuse to join two lines). Arguments: the object being
// built (this), lines a and b, and a flag k. Line records: word +0 (name pointer), count +4, word
// +8, word +12, and a pointer at +20 to an array of 64-byte points whose first 12 bytes are the
// position; the word at +60 of each point is its index. Steps:
//   this[0] = a[0]; this[8] = k != 0 ? 1 : a[8]; this[12] = a[12];
//   this[4] = a[4] + b[4] - k - 1; this[20] = memAlloc__FiPc(this[4] << 6, "point list");
//   for i in 0 .. a[4]-2: point i of this gets index i and a copy of the 12 position bytes of
//     point i of a (the last point of a is dropped);
//   for j in 0 .. b[4]-k-1 with e = a[4] - 1 + j: the 12 position bytes of point j of b are copied
//     to point e of this (with k set the last point of b is dropped as well), but the word at +60
//     that receives e is the one of point j of this, not of point e. That mismatch is what the
//     image does; it overwrites the index of the early copied points, and leaves the index word
//     of the points from a[4]-1 on unwritten by this loop.
// The routine then tail-calls initializeMe__4Line(this). Its own return value is not used.
// Inferred: two paths are joined end to end, the shared end being one vertex when k is 1.
// Unknown: other fields of the record and what initializeMe derives. Nothing here was executed
// on the EE.
extern "C" {
void* memAlloc__FiPc(int bytes, char* label);
void initializeMe__4Line(void* line);
void __4LineP4LineT1i(unsigned char* self, unsigned char* a, unsigned char* b, int k) {
    using namespace rend;
    at<u32>(self, 0) = at<u32>(a, 0);
    at<i32>(self, 8) = k != 0 ? 1 : at<i32>(a, 8);
    at<u32>(self, 12) = at<u32>(a, 12);
    const int count = at<i32>(a, 4) + at<i32>(b, 4) - k - 1;
    at<i32>(self, 4) = count;
    set_ptr(self, 20, memAlloc__FiPc(count << 6, const_cast<char*>("point list")));
    for (int i = 0; i < at<i32>(a, 4) - 1; ++i) {
        unsigned char* to = ptr(self, 20) + i * 64;
        const unsigned char* from = ptr(a, 20) + i * 64;
        at<i32>(to, 60) = i;
        at<u32>(to, 0) = at<u32>(from, 0);
        at<u32>(to, 4) = at<u32>(from, 4);
        at<u32>(to, 8) = at<u32>(from, 8);
    }
    for (int j = 0; j < at<i32>(b, 4) - k; ++j) {
        const int index = at<i32>(a, 4) + j - 1;
        unsigned char* indexed = ptr(self, 20) + j * 64;
        unsigned char* to = ptr(self, 20) + index * 64;
        const unsigned char* from = ptr(b, 20) + j * 64;
        at<i32>(indexed, 60) = index;
        at<u32>(to, 0) = at<u32>(from, 0);
        at<u32>(to, 4) = at<u32>(from, 4);
        at<u32>(to, 8) = at<u32>(from, 8);
    }
    initializeMe__4Line(self);
}
}
