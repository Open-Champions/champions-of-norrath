#include <cstring>

extern "C" int msg_load__8ItemPropPc(void* object, const unsigned char* buffer);

static int order[4], steps;
static void* unpacked;
static unsigned char unpack_saw[72];
static unsigned char* prop_object;
static const unsigned char* prop_buffer;
static unsigned flags_at_prop;
static unsigned flags_at_matrix;
static int prop_result = 9;
static unsigned char* matrix;
static unsigned char point_seen[12];
static int yaw_seen;

static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

extern "C" void unpack__4itemPQ24item11description(void* description) {
    order[steps++] = 1;
    unpacked = description;
    std::memcpy(unpack_saw, description, 72);
}
extern "C" int msg_load__4PropPc(void* object, const unsigned char* buffer) {
    order[steps++] = 2;
    prop_object = static_cast<unsigned char*>(object);
    prop_buffer = buffer;
    flags_at_prop = word(prop_object + 12);
    // The Prop loader may rewrite the object; make it write the flag and bytes the caller reads.
    return prop_result;
}
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* m, const void* point, short yaw) {
    order[steps++] = 3;
    matrix = static_cast<unsigned char*>(m);
    std::memcpy(point_seen, point, 12);
    yaw_seen = yaw;
    flags_at_matrix = word(prop_object + 12);
}

int main() {
    unsigned char object[400];
    unsigned char buffer[200];
    for (int i = 0; i < 400; ++i) object[i] = static_cast<unsigned char>(i * 3);
    for (int i = 0; i < 200; ++i) buffer[i] = static_cast<unsigned char>(0x80 + i);
    unsigned w = 0x00ff0800u | 0x3001u;
    std::memcpy(object + 12, &w, 4);
    buffer[12] = 0x00;   // object+340 comes from the loaded buffer
    unsigned short yaw = 0x7f00;
    std::memcpy(object + 56, &yaw, 2);
    int n = msg_load__8ItemPropPc(object, buffer);
    if (n != 81) return 1;
    if (steps != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 2;
    if (unpacked != object + 328 || std::memcmp(unpack_saw, buffer, 72) != 0) return 3;
    if (prop_object != object || prop_buffer != buffer + 72) return 4;
    if (flags_at_prop != ((w | 0x100000u) & ~0x800u)) return 5;
    if (flags_at_matrix != flags_at_prop) return 6;           // bit 39 clear: bits 12 and 13 stay
    if (matrix != object + 272 || std::memcmp(point_seen, object + 16, 12) != 0) return 7;
    if (yaw_seen != static_cast<short>(0x7f00 + 16384)) return 8;   // wraps to a negative short
    // Bit 39 set (byte 340, 0x80): bits 12 and 13 are cleared after Prop::msg_load.
    for (int i = 0; i < 200; ++i) buffer[i] = static_cast<unsigned char>(i);
    w = 0xffffffffu;
    std::memcpy(object + 12, &w, 4);
    buffer[12] = 0x80;
    yaw = 0x8001;
    std::memcpy(object + 56, &yaw, 2);
    steps = 0;
    n = msg_load__8ItemPropPc(object, buffer);
    if (flags_at_prop != (0xffffffffu & ~0x800u)) return 9;
    if (flags_at_matrix != (0xffffffffu & ~0x800u & ~0x3000u)) return 10;
    if (yaw_seen != static_cast<short>(0x8001 + 16384)) return 11;
    // Other bits in byte 340 do not count.
    buffer[12] = 0x7f;
    w = 0;
    std::memcpy(object + 12, &w, 4);
    msg_load__8ItemPropPc(object, buffer);
    if (flags_at_matrix != 0x100000u) return 12;
    if (object[340] != 0x7f) return 13;
    return 0;
}
