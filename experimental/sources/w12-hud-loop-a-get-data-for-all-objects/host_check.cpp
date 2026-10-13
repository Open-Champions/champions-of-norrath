// Host check for getDataForAllObjects__FPciPiiT2T2. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin host_check.cpp
// Target words (node and object links, the filter function) are mapped through a byte arena: a word is an
// offset into it. The iterator, the per-object serializer and the filter are stand-ins that log.
#define NORRATH_HOST_CONTRACT
#include "get_data_for_all_objects.cpp"

#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

extern "C" {
alignas(8) unsigned char objectGrid[120 * 480];
alignas(8) unsigned char ghostObjectGrid[120 * 480];
alignas(8) unsigned char offTheGrid[8];
unsigned userFilterFunction;
}

static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_gdao_pointer(unsigned word) { return arena + word; }

static std::string trace;
static std::map<const unsigned char*, std::vector<unsigned>> chains;
static std::map<unsigned, int> filterResult;
static std::map<unsigned, int> lengths;
static char output[1 << 16];
static int constructs;
static unsigned mutateObject;

static void put_u32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }

static std::string head_name(const unsigned char* head) {
    char text[40];
    if (head >= objectGrid && head < objectGrid + sizeof objectGrid) {
        std::snprintf(text, sizeof text, "o%d.%d", static_cast<int>((head - objectGrid) / 480), static_cast<int>((head - objectGrid) % 480 / 4));
    } else if (head >= ghostObjectGrid && head < ghostObjectGrid + sizeof ghostObjectGrid) {
        std::snprintf(text, sizeof text, "g%d.%d", static_cast<int>((head - ghostObjectGrid) / 480), static_cast<int>((head - ghostObjectGrid) % 480 / 4));
    } else {
        return "t";
    }
    return text;
}

extern "C" {
void __16ListSafeIteratorP8ListHeadi(unsigned* iterator, unsigned char* head, int mask) {
    ++constructs;
    if (mask != 0) trace += "MASK?";
    auto it = chains.find(head);
    if (it != chains.end()) trace += "[" + head_name(head);
    iterator[1] = it != chains.end() && !it->second.empty() ? it->second[0] : 0;
    iterator[2] = 0;
    iterator[3] = reinterpret_cast<__UINTPTR_TYPE__>(head) & 0xffffffffu;
    iterator[4] = 0;
    // keep the head for next(): the host pointer is held in two words
    std::memcpy(&iterator[4], &head, sizeof head);
}
void next__16ListSafeIterator(unsigned* iterator) {
    unsigned char* head;
    std::memcpy(&head, &iterator[4], sizeof head);
    ++iterator[2];
    auto& chain = chains[head];
    iterator[1] = iterator[2] < chain.size() ? chain[iterator[2]] : 0;
}
void _$_16ListSafeIterator(unsigned* iterator, int flags) {
    unsigned char* head;
    std::memcpy(&head, &iterator[4], sizeof head);
    if (chains.count(head)) trace += flags == 2 ? "]" : "]?";
}
int objGet__FP10GameObjectPc(unsigned char* object, char* destination) {
    unsigned id = static_cast<unsigned>(object - arena);
    char text[40];
    std::snprintf(text, sizeof text, "O%u@%d,", id, static_cast<int>(destination - output));
    trace += text;
    return lengths[id];
}
int norrath_gdao_call_filter(unsigned function, unsigned argument) {
    if (function != 0x100) trace += "F?";
    char text[24];
    std::snprintf(text, sizeof text, "f%u,", argument);
    trace += text;
    if (mutateObject != 0) put_u32(arena + mutateObject + 12, 0);
    return filterResult[argument];
}
}

// Builds one object in the arena: object at 'id' with flags, filter argument and serializer length.
static void make_object(unsigned node, unsigned id, unsigned flags, unsigned argument, int length) {
    put_u32(arena + node + 12, id);
    put_u32(arena + id + 12, flags);
    put_u32(arena + id + 52, argument);
    lengths[id] = length;
}

static void reset() {
    trace.clear();
    chains.clear();
    filterResult.clear();
    lengths.clear();
    std::memset(arena, 0, sizeof arena);
    userFilterFunction = 0;
    constructs = 0;
    mutateObject = 0;
}

static int run(int size, int pass, int* count, int* eligible, int* omitted) {
    *count = *eligible = *omitted = -77;
    return getDataForAllObjects__FPciPiiT2T2(output, size, count, pass, eligible, omitted);
}

static int expect(int code, const char* want) {
    if (trace == want) return 0;
    std::printf("case %d\n got  %s\n want %s\n", code, trace.c_str(), want);
    return code;
}

int main() {
    int rc, count, eligible, omitted, result;
    // empty world: 28801 iterators, nothing written but the totals
    reset();
    result = run(52000, 0, &count, &eligible, &omitted);
    if (result != 0 || count != 0 || eligible != 0 || omitted != 0 || constructs != 28801) return 1;

    // pass 0 takes everything not skipped, in grid order (objectGrid rows and columns, ghost grid, then the extra list)
    reset();
    chains[objectGrid + 1 * 480 + 0 * 4] = {0x1000};
    chains[objectGrid + 0 * 480 + 119 * 4] = {0x1040, 0x1080};
    chains[ghostObjectGrid + 5 * 480 + 7 * 4] = {0x10c0};
    chains[offTheGrid] = {0x1100};
    make_object(0x1000, 0x2000, 0x1000, 11, 10);
    make_object(0x1040, 0x2100, 0x0000, 12, 20);
    make_object(0x1080, 0x2200, 0x4000, 13, 30);     // flag 0x4000: skipped, not counted anywhere
    make_object(0x10c0, 0x2300, 0x2000, 14, 40);
    make_object(0x1100, 0x2400, 0x3000, 15, 50);
    result = run(52000, 0, &count, &eligible, &omitted);
    if ((rc = expect(2, "[o0.119O8448@0,][o1.0O8192@20,][g5.7O8960@30,]"
                        "[tO9216@70,]"))) return rc;
    if (result != 120 || count != 5 - 1 || eligible != 3 || omitted != 0) return 3;

    // a user filter that rejects one object, called with the word at object+52; the flags are re-read after it
    reset();
    userFilterFunction = 0x100;
    chains[objectGrid] = {0x1000, 0x1040, 0x1080};
    make_object(0x1000, 0x2000, 0x1000, 21, 5);
    make_object(0x1040, 0x2100, 0x1000, 22, 6);
    make_object(0x1080, 0x2200, 0x4000, 23, 7);
    filterResult[21] = 1;
    result = run(52000, 0, &count, &eligible, &omitted);
    if ((rc = expect(4, "[o0.0f21,f22,O8448@0,]"))) return rc;
    if (result != 6 || count != 1 || eligible != 1 || omitted != 0) return 5;

    // pass 2 keeps every third eligible object (the running total after the increase), others count as omitted;
    // an object with no 0x3000 bit is always kept
    reset();
    chains[objectGrid] = {0x1000, 0x1040, 0x1080, 0x10c0, 0x1100};
    make_object(0x1000, 0x2000, 0x1000, 1, 1);
    make_object(0x1040, 0x2100, 0x1000, 1, 2);
    make_object(0x1080, 0x2200, 0x1000, 1, 3);
    make_object(0x10c0, 0x2300, 0x0000, 1, 4);
    make_object(0x1100, 0x2400, 0x1000, 1, 5);
    result = run(52000, 2, &count, &eligible, &omitted);
    if ((rc = expect(6, "[o0.0O8704@0,O8960@3,]"))) return rc;
    if (result != 7 || count != 2 || eligible != 4 || omitted != 3) return 7;

    // flag 0x2000 forces objects in only for pass 1
    reset();
    chains[objectGrid] = {0x1000, 0x1040, 0x1080};
    make_object(0x1000, 0x2000, 0x2000, 1, 1);
    make_object(0x1040, 0x2100, 0x2000, 1, 2);
    make_object(0x1080, 0x2200, 0x2000, 1, 3);
    result = run(52000, 1, &count, &eligible, &omitted);
    if (count != 3 || omitted != 0 || result != 6) return 8;
    result = run(52000, 2, &count, &eligible, &omitted);
    if (count != 1 || omitted != 2 || result != 3) return 9;

    // output full: the walk stops at once with -1 and leaves the outputs untouched
    reset();
    chains[objectGrid] = {0x1000, 0x1040};
    make_object(0x1000, 0x2000, 0x0000, 1, 600);
    make_object(0x1040, 0x2100, 0x0000, 1, 600);
    result = run(1000, 0, &count, &eligible, &omitted);
    if ((rc = expect(10, "[o0.0O8192@0,]"))) return rc;
    if (result != -1 || count != 1 || eligible != -77 || omitted != -77) return 11;
    // exactly size - 512 used up still stops, one byte less does not
    reset();
    chains[objectGrid] = {0x1000};
    make_object(0x1000, 0x2000, 0x0000, 1, 488);
    if (run(1000, 0, &count, &eligible, &omitted) != -1) return 12;
    lengths[0x2000] = 487;
    if (run(1000, 0, &count, &eligible, &omitted) != 487) return 13;
    // the filter may change the flags, which are read again afterwards: here it clears 0x1000
    reset();
    userFilterFunction = 0x100;
    chains[objectGrid] = {0x1000};
    make_object(0x1000, 0x2000, 0x1000, 31, 5);
    mutateObject = 0x2000;
    result = run(52000, 3, &count, &eligible, &omitted);
    if (count != 1 || eligible != 0 || omitted != 0 || result != 5) return 14;
    std::puts("get data for all objects passed");
    return 0;
}
