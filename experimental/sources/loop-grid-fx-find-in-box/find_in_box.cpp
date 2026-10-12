// Observed objectFindInBox__FG6Point3T0PP10GameObjecti at 0x13ef30, extent 548.
// The two Point3 arguments arrive by address and are copied before use. Cells
// are 256 wide over a 120 by 120 grid. Lower corner cells are
// (int(min) + 15264) >> 8 and upper corner cells (int(max) + 15456) >> 8, for x
// and y; the lower cells are raised to zero and the upper cells capped at
// 119. If the lower y cell is above the upper y cell nothing is searched.
// Rows run low to high, and cells low to high inside a row, over objectGrid
// (0x4fc0c0; row stride 480 bytes). Each cell is a list: head word, then node
// word 0 for next and node word 12 for the object. A null object is skipped.
// An object whose flag word at +12 shares a bit with the ignore mask (the word
// at 0x483ac0, reread for every object) is skipped. Boxes are tested in this
// order and an object is skipped when any is true: x + r < min.x,
// max.x < x - r, y + r < min.y, max.y < y - r, max.z < z, z + height < min.z,
// with x +16, y +20, z +24, r +28 and height +32 as floats. An unordered
// comparison therefore does not skip. A kept object is stored while the count
// is below capacity - 1; at that point the search stops. A zero word is stored
// after the last object, and its address is returned.
extern "C" {
extern unsigned char objectGrid[];
extern unsigned char norrath_grid_ignore_flags_view[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
struct Point3 { float x, y, z; };
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(static_cast<unsigned>(a)));
#else
    return reinterpret_cast<unsigned char*>(a);
#endif
}
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
float read_float(Address a) {
    float value;
    __builtin_memcpy(&value, bytes(a), sizeof value);
    return value;
}
int shift_8(int value) {
    const unsigned word = static_cast<unsigned>(value);
    return static_cast<int>((word >> 8) | ((word & 0x80000000u) != 0u ? 0xff000000u : 0u));
}
void write_direct(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void** objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 minimum, Point3 maximum, void** output, int capacity) {
    const Point3 low = minimum;
    const Point3 high = maximum;
    int first_y = shift_8(static_cast<int>(low.y) + 15264);
    int last_y = shift_8(static_cast<int>(high.y) + 15456);
    int first_x = shift_8(static_cast<int>(low.x) + 15264);
    int last_x = shift_8(static_cast<int>(high.x) + 15456);
    if (first_y < 0) first_y = 0;
    if (last_y >= 120) last_y = 119;
    if (first_x < 0) first_x = 0;
    if (last_x >= 120) last_x = 119;
    unsigned char* destination = reinterpret_cast<unsigned char*>(output);
    int count = 0;
    for (int row = first_y; row <= last_y; ++row) {
        for (int column = first_x; column <= last_x; ++column) {
            Address node = read_word(address(objectGrid) + static_cast<unsigned>((row * 120 + column) * 4));
            for (; node != 0u; node = read_word(node)) {
                const Address object = read_word(node + 12u);
                if (object == 0u) continue;
                if ((read_word(object + 12u) & read_word(address(norrath_grid_ignore_flags_view))) != 0u) continue;
                const float x = read_float(object + 16u);
                const float radius = read_float(object + 28u);
                if (x + radius < low.x) continue;
                if (high.x < x - radius) continue;
                const float y = read_float(object + 20u);
                if (y + radius < low.y) continue;
                if (high.y < y - radius) continue;
                const float z = read_float(object + 24u);
                if (high.z < z) continue;
                if (z + read_float(object + 32u) < low.z) continue;
                if (!(count < capacity - 1)) goto done;
                write_direct(destination + count * 4, static_cast<unsigned>(object));
                ++count;
            }
        }
    }
done:
    write_direct(destination + count * 4, 0u);
    return reinterpret_cast<void**>(destination + count * 4);
}
