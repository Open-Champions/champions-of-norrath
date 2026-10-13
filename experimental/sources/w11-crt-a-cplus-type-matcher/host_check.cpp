// Host contract for __cplus_type_matcher. Not an EE plan input. The record is a byte array so
// the 32-bit word offsets hold on a 64-bit host; the callee and the match function are stubs.
#include <cstring>
#include "cplus_type_matcher.cpp"
static int matcher_calls, rtti_calls, rtti_result = 1;
static void* seen_match; static unsigned seen_type, seen_original; static void* seen_value;
static int marker;
static void* match_function() { ++matcher_calls; return &marker; }
extern "C" int __throw_type_match_rtti_2(void* m, unsigned t, unsigned o, void* v) {
    ++rtti_calls; seen_match = m; seen_type = t; seen_original = o; seen_value = v; return rtti_result;
}
int main() {
    alignas(8) unsigned char record[64] = {0};
    const unsigned type = 0x11223344u, original = 0x55667788u;
    std::memcpy(record + 12, &type, 4); std::memcpy(record + 40, &original, 4);
    unsigned short cxx[4] = {0, 0, 4, 0}, other[4] = {0, 0, 2, 0};
    // wrong language: null, and nothing else is looked at
    if (__cplus_type_matcher(record, reinterpret_cast<void*>(match_function), other) != 0 || matcher_calls || rtti_calls) return 1;
    // catch-all
    if (__cplus_type_matcher(record, reinterpret_cast<void*>(-1), cxx) != reinterpret_cast<void*>(1) || matcher_calls || rtti_calls) return 2;
    if (__cplus_type_matcher(record, reinterpret_cast<void*>(-1), 0) != reinterpret_cast<void*>(1)) return 3;
    // typed clause, table present
    void* r = __cplus_type_matcher(record, reinterpret_cast<void*>(match_function), cxx);
    if (r != reinterpret_cast<void*>(1) || matcher_calls != 1 || rtti_calls != 1) return 4;
    if (seen_match != &marker || seen_type != type || seen_original != original || seen_value != record + 8) return 5;
    // typed clause, no table, callee says no
    rtti_result = 0;
    r = __cplus_type_matcher(record, reinterpret_cast<void*>(match_function), 0);
    if (r != 0 || matcher_calls != 2 || rtti_calls != 2) return 6;
    // any nonzero answer becomes exactly 1
    rtti_result = 77;
    if (__cplus_type_matcher(record, reinterpret_cast<void*>(match_function), cxx) != reinterpret_cast<void*>(1)) return 7;
    return 0;
}
