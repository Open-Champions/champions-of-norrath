// Observed objectUpdateInGrid__FP10GameObject at 0x143630, extent 240.
// Object words: object+8 holds the object's own list node, whose word at +8
// is the node linked into a grid cell list (see listOnewayLink at 0x194390:
// node words are next 0, previous 4, partner 8, value 12). The cell node is
// unlinked first: the word at previous+0 takes next, then next+4 takes
// previous. Object floats x and y are at 16 and 20, truncated on this CPU.
// Cells are 256 units wide with the origin offset by 15360 over a 120 by 120
// grid: gx = (int(x) + 15360) >> 8, likewise gy. Inside the grid (gx below
// 120 unsigned, 0 <= gy < 120) the head is word (gy * 120 + gx) of
// ghostObjectGrid (0x50a1c0) when bit 0x800 of the word at object+12 is set,
// else of objectGrid (0x4fc0c0); outside it is offTheGrid (the head at
// 0x486890, reached through gp). The cell node is pushed in front: previous
// is the head word, next is the old first, the head takes the node, and the
// old first's previous takes the node. Unlike objectAddToGrid, bit 0x2 of
// the object flags is not consulted.
extern "C" {
extern unsigned char objectGrid[];
extern unsigned char ghostObjectGrid[];
extern unsigned char offTheGrid[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
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
void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int read_truncated_float(Address a) {
    float value;
    __builtin_memcpy(&value, bytes(a), sizeof value);
    return static_cast<int>(value);
}
int arithmetic_shift_8(int value) {
    const unsigned word = static_cast<unsigned>(value);
    return static_cast<int>((word >> 8) | ((word & 0x80000000u) != 0u ? 0xff000000u : 0u));
}
}

extern "C" void objectUpdateInGrid__FP10GameObject(void* object) {
    const Address self = address(object);
    const Address node = read_word(read_word(self + 8u) + 8u);
    write_word(read_word(node + 4u), read_word(node));
    if (read_word(node) != 0u) write_word(read_word(node) + 4u, read_word(node + 4u));
    const int x = read_truncated_float(self + 16u);
    const int y = read_truncated_float(self + 20u);
    const int cell_x = arithmetic_shift_8(x + 15360);
    const int cell_y = arithmetic_shift_8(y + 15360);
    Address head;
    if (static_cast<unsigned>(cell_x) < 120u && cell_y >= 0 && cell_y < 120) {
        const Address grid = (read_word(self + 12u) & 0x800u) != 0u ? address(ghostObjectGrid) : address(objectGrid);
        head = grid + static_cast<unsigned>((cell_y * 120 + cell_x) * 4);
    } else {
        head = address(offTheGrid);
    }
    const unsigned first = read_word(head);
    write_word(node + 4u, static_cast<unsigned>(head));
    write_word(node, first);
    write_word(head, static_cast<unsigned>(node));
    if (read_word(node) != 0u) write_word(first + 4u, static_cast<unsigned>(node));
}
