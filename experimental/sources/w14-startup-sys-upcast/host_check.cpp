// Host check for upcast__C16__user_type_infoRC9type_infoPvPPv
// (source: ../w13-startup-fn-a-upcast/upcast.cpp). Not a build-plan input.
//   c++ -std=c++17 -fno-strict-aliasing -o check host_check.cpp && ./check
// The routine reads the vtable pointer at this+4 and, at vtable+16, a halfword adjustment followed (at +4) by a
// function pointer. On the 64-bit host the pointer-sized words are 8 bytes, but the offsets are the same, so the
// objects are built byte-wise with memcpy. The scripted slot records its arguments and fills the record. The
// check requires: this+adjustment is passed (positive, zero and negative adjustments), the constant 6, the
// target and object pointers unchanged, a record zeroed before the call, and the address of that record; a
// nonzero slot result returns 0 and stores nothing; otherwise record word 0 is stored through the last
// argument and the result is 1 exactly when record word 1 is at least 6.
#include <cstdio>
#include <cstring>
#include "../w13-startup-fn-a-upcast/upcast.cpp"

static const void* seen_self; static int seen_path; static const void* seen_target; static void* seen_object;
static NorrathUpcastRecord* seen_record; static bool record_was_zero;
static int slot_result; static void* set_object; static int set_path, set_marker;
static int slot(const void* self, int path, const void* target, void* object, NorrathUpcastRecord* rec) {
    seen_self = self; seen_path = path; seen_target = target; seen_object = object; seen_record = rec;
    record_was_zero = rec->object == nullptr && rec->access_path == 0 && rec->marker == 0;
    rec->object = set_object; rec->access_path = set_path; rec->marker = set_marker;
    return slot_result;
}

int main() {
    int bad = 0;
    alignas(16) static unsigned char block[256];
    alignas(16) static unsigned char vtable[64];
    const int adjusts[] = {0, 8, -4};
    for (int adj : adjusts) {
        for (int result = 0; result < 2; ++result) {
            for (int path = -1; path <= 8; ++path) {
                std::memset(block, 0xab, sizeof block); std::memset(vtable, 0xcd, sizeof vtable);
                unsigned char* self = block + 64;
                const void* vt = vtable; std::memcpy(self + 4, &vt, sizeof vt);
                short a = (short)adj; std::memcpy(vtable + 16, &a, 2);
                int (*fn)(const void*, int, const void*, void*, NorrathUpcastRecord*) = slot;
                std::memcpy(vtable + 20, &fn, sizeof fn);
                int target_tag, object_tag; void* adjusted = (void*)0x1234;
                slot_result = result; set_object = &object_tag; set_path = path; set_marker = 77;
                record_was_zero = false;
                int r = upcast__C16__user_type_infoRC9type_infoPvPPv(self, &target_tag, &object_tag + 0, &adjusted);
                if (seen_self != self + adj || seen_path != 6 || seen_target != &target_tag || seen_object != &object_tag || !record_was_zero) {
                    std::printf("args adj=%d res=%d path=%d\n", adj, result, path); ++bad;
                }
                if (result) {
                    if (r != 0 || adjusted != (void*)0x1234) { std::printf("nonzero slot adj=%d path=%d\n", adj, path); ++bad; }
                } else {
                    if (adjusted != &object_tag) { std::printf("store adj=%d path=%d\n", adj, path); ++bad; }
                    if (r != (path >= 6 ? 1 : 0)) { std::printf("result adj=%d path=%d got %d\n", adj, path, r); ++bad; }
                }
            }
        }
    }
    return bad;
}
