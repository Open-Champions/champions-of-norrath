static unsigned char arena[1 << 16];
#define NORRATH_WORD_ADDRESS(word) (arena + static_cast<unsigned>(word))
#include "parse_zone_room_actors.cpp"

#include <cstdio>
#include <string>

static std::string trace;
static unsigned short tags[8];
static unsigned tag_index;
static int begin_result, end_result, sub_objects, index_value, init_result, item_result;
static int items_seen;

extern "C" int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second) {
    if (file != arena + 0x55) trace += "BADFILE";
    *tag = tag_index < 8 ? tags[tag_index++] : 0xffff;
    *second = 0;
    trace += "B;";
    return begin_result;
}
extern "C" int Read__9VIObjFileRi(void*, int* value) { *value = index_value; trace += "R;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { trace += "E;"; return end_result; }
extern "C" int NumSubObjects__9VIObjFile(void*) { trace += "N;"; return sub_objects; }
extern "C" int Init__t7VIArray1Zii(void* array, int count) {
    trace += "I" + std::to_string(count) + ";";
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(array)) = count;
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(array) + 4) = 0x8000;
    return init_result;
}
extern "C" int ParseZoneActor__10VIESFParse(void*) {
    trace += "A;";
    *reinterpret_cast<int*>(arena + 0x8000 + 4 * items_seen) = 0;
    ++items_seen;
    return item_result;
}

static int run(int begin, int end, int n, int index, int init, int item, unsigned short t1, unsigned short t2) {
    for (unsigned i = 0; i < sizeof arena; ++i) arena[i] = 0;
    trace.clear();
    tag_index = 0; items_seen = 0;
    tags[0] = t1; tags[1] = t2;
    begin_result = begin; end_result = end; sub_objects = n; index_value = index; init_result = init; item_result = item;
    *reinterpret_cast<unsigned*>(arena + 0x100 + 36) = 0x55;
    *reinterpret_cast<unsigned*>(arena + 0x100 + 28) = 0x2000;
    *reinterpret_cast<unsigned*>(arena + 0x2000 + 96) = 0x3000;
    return ParseZoneRoomActorsObj__10VIESFParse(arena + 0x100);
}

int main() {
    if (run(0, 0, 2, 3, 0, 5, 0x3280, 0x6040) != 0) return 1;
    if (trace != "B;R;E;B;N;I2;N;A;N;A;N;E;E;") return 2;
    if (*reinterpret_cast<int*>(arena + 0x8000) != 5 || *reinterpret_cast<int*>(arena + 0x8004) != 5) return 3;
    // The array of the room with index 3 sits 3 * 120 bytes into the room table.
    if (*reinterpret_cast<unsigned*>(arena + 0x3000 + 360 + 60) != 2u || items_seen != 2) return 4;
    if (run(0, 0, 0, 1, 0, 5, 0x3280, 0x6040) != 0 || trace != "B;R;E;B;N;I0;N;E;E;") return 5;
    if (run(0, 0, -4, 1, 0, 5, 0x3280, 0x6040) != 0) return 6;
    if (run(-1, 0, 2, 3, 0, 5, 0x3280, 0x6040) != -1 || trace != "B;") return 7;
    if (run(0, 0, 2, 3, 0, 5, 0x3280 + 1, 0x6040) != -1 || trace != "B;") return 8;
    if (run(0, 0, 2, 3, 0, 5, 0x3280, 0x6040 + 1) != -1 || trace != "B;R;E;B;") return 9;
    if (run(0, -1, 2, 3, 0, 5, 0x3280, 0x6040) != -1 || trace != "B;R;E;") return 10;
    if (run(0, 0, 2, 3, -1, 5, 0x3280, 0x6040) != -1 || trace != "B;R;E;B;N;I2;") return 11;
    if (run(0, 0, 3, 3, 0, -2, 0x3280, 0x6040) != -1 || items_seen != 1) return 12;
    // The negative value is stored before the early return.
    if (*reinterpret_cast<int*>(arena + 0x8000) != -2) return 13;
    std::puts("esfparse room-actors passed");
    return 0;
}
