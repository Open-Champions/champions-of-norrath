// Host contract for setAnimEventFlags__8Creatureififififi. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct AnimEventRecord {
    unsigned char before_first[20]; float first_float; signed char first_byte; unsigned char pad25[3];
    float second_float; signed char second_byte; unsigned char pad33[3];
    float third_float; signed char third_byte; unsigned char pad41[3];
    float fourth_float; signed char fourth_byte;
};
struct AnimEventEntry { unsigned word0; AnimEventRecord* record; };
struct EventCreature { unsigned char before_table[36]; AnimEventEntry* table; };
extern "C" void setAnimEventFlags__8Creatureififififi(EventCreature*, int, float, int, float, int, float, int, float, int);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    AnimEventRecord records[3];
    std::memset(records, 0x7e, sizeof records);
    AnimEventEntry table[3] = {{1, &records[0]}, {2, &records[1]}, {3, &records[2]}};
    EventCreature creature;
    std::memset(&creature, 0, sizeof creature);
    creature.table = table;
    setAnimEventFlags__8Creatureififififi(&creature, 1, 1.5f, 10, 2.5f, 20, 3.5f, 30, 4.5f, -40);
    const AnimEventRecord& r = records[1];
    expect(r.first_float == 1.5f && r.second_float == 2.5f && r.third_float == 3.5f && r.fourth_float == 4.5f, "floats go to +20, +28, +36, +44 in argument order");
    expect(r.first_byte == 10 && r.second_byte == 20 && r.third_byte == 30 && r.fourth_byte == -40, "bytes go to +24, +32, +40, +48 in argument order");
    expect(r.pad25[0] == 0x7e && r.pad33[2] == 0x7e && r.pad41[1] == 0x7e && r.before_first[19] == 0x7e, "bytes between the fields are untouched");
    unsigned char untouched[sizeof(AnimEventRecord)];
    std::memset(untouched, 0x7e, sizeof untouched);
    expect(std::memcmp(&records[0], untouched, sizeof untouched) == 0 && std::memcmp(&records[2], untouched, sizeof untouched) == 0, "other records untouched");
    setAnimEventFlags__8Creatureififififi(&creature, 0, 0.0f, 0x1ff, 0.0f, 0x100, 0.0f, 0, 0.0f, 0x7f);
    expect(records[0].first_byte == -1 && records[0].second_byte == 0 && records[0].fourth_byte == 0x7f, "byte stores keep the low eight bits");
    if (failures) return 1;
    std::printf("setAnimEventFlags contract passed\n");
    return 0;
}
