// Host contract for setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi.
// Not an EE plan input.
#include <cstdio>
#include <cstring>
struct AnimEventRecord {
    unsigned char before_first[20]; float first_float; signed char first_byte; unsigned char pad25[3];
    float second_float; signed char second_byte; unsigned char pad33[3];
    float third_float; signed char third_byte; unsigned char pad41[3];
    float fourth_float; signed char fourth_byte;
};
struct AnimEventEntry { unsigned word0; AnimEventRecord* record; };
extern "C" void setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(void*, AnimEventEntry*, int, int, float, int, float, int, float, int, float);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    AnimEventRecord records[3];
    std::memset(records, 0x7e, sizeof records);
    AnimEventEntry map[3] = {{1, &records[0]}, {2, &records[1]}, {3, &records[2]}};
    setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(nullptr, map, 2, 1, 1.5f, 2, 2.5f, 3, 3.5f, -4, 4.5f);
    const AnimEventRecord& r = records[2];
    expect(r.first_byte == 1 && r.second_byte == 2 && r.third_byte == 3 && r.fourth_byte == -4, "bytes land at +24, +32, +40, +48");
    expect(r.first_float == 1.5f && r.second_float == 2.5f && r.third_float == 3.5f && r.fourth_float == 4.5f, "floats land at +20, +28, +36, +44");
    expect(r.pad25[0] == 0x7e && r.pad33[1] == 0x7e && r.pad41[2] == 0x7e && r.before_first[0] == 0x7e, "gaps untouched");
    unsigned char untouched[sizeof(AnimEventRecord)];
    std::memset(untouched, 0x7e, sizeof untouched);
    expect(std::memcmp(&records[0], untouched, sizeof untouched) == 0 && std::memcmp(&records[1], untouched, sizeof untouched) == 0, "other records untouched");
    setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(nullptr, map, 0, 0x1ff, 0.0f, 0x100, 0.0f, 0, 0.0f, 0x7f, 0.0f);
    expect(records[0].first_byte == -1 && records[0].second_byte == 0 && records[0].fourth_byte == 0x7f, "low eight bits stored");
    if (failures) return 1;
    std::printf("setAnimEventFlagsResMap contract passed\n");
    return 0;
}
