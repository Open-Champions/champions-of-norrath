// Observed lightUpdatePosition__FP5Light at 0x0013eb60, extent 192. One argument: a
// Light. Returns nothing. Light words as in lightAdd: next at 0, previous-link at 4,
// x and y floats at 48 and 52.
//
// Unlike lightAdd (0x0013d230) this does not clear the light's own links after unlinking,
// and it does nothing at all unless the previous-link is non-zero (a light that is not in
// a list is left alone). Otherwise the light is unlinked: the word at previous+0 takes
// next, and if next is non-zero the word at next+4 takes the previous-link. Then the light
// is put at the front of the list for its cell exactly as lightAdd does: gx = (int(x) +
// 11520) >> 8 and gy likewise from y; with gx below 90 (unsigned) and 0 <= gy < 90 the head
// word is lightGrid[gy * 90 + gx] (0x4f40f0), otherwise the off-grid head word at
// 0x486848. Its next takes the old first, its previous-link is the head word, the head
// takes the light, and the old first's previous-link takes the light. The float to
// integer conversion truncates on this CPU.
// Inferred: the name says this is the move after a light's position changed.
// Unknown: why lightAdd clears the links and this does not (callers not examined); whether
// the early return on a zero previous-link is relied on.
extern "C" {
extern unsigned char norrath_floor_light_grid_view[];
extern unsigned char norrath_light_off_grid_head_view[8];
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

extern "C" void lightUpdatePosition__FP5Light(void* light) {
    const Address self = address(light);
    if (read_word(self + 4u) == 0u) return;
    write_word(read_word(self + 4u), read_word(self));
    const unsigned following = read_word(self);
    if (following != 0u) write_word(following + 4u, read_word(self + 4u));
    const int x = read_truncated_float(self + 48u);
    const int y = read_truncated_float(self + 52u);
    const int cell_x = arithmetic_shift_8(x + 11520);
    const int cell_y = arithmetic_shift_8(y + 11520);
    Address head;
    if (static_cast<unsigned>(cell_x) < 90u && cell_y >= 0 && cell_y < 90) {
        head = address(norrath_floor_light_grid_view) + static_cast<unsigned>((cell_y * 90 + cell_x) * 4);
    } else {
        head = address(norrath_light_off_grid_head_view);
    }
    const unsigned first = read_word(head);
    write_word(self + 4u, static_cast<unsigned>(head));
    write_word(self, first);
    write_word(head, static_cast<unsigned>(self));
    if (read_word(self) != 0u) write_word(read_word(self) + 4u, static_cast<unsigned>(self));
}
