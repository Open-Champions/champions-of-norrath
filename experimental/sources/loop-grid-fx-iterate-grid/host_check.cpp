// Host contract for objectIterateGrid__FPFP10GameObject_v. Not an EE plan input.
// The iterator functions are small models of the list routines: construct
// points the iterator at the first node of a head, next follows node+0.
#include <cstdio>
#include <cstring>

extern "C" unsigned char objectGrid[57600] = {0};
extern "C" unsigned char ghostObjectGrid[57600] = {0};
extern "C" unsigned char offTheGrid[8] = {0};
extern "C" unsigned char norrath_grid_ignore_flags_view[4] = {0};
extern "C" void objectIterateGrid__FPFP10GameObject_v(void (*callback)(void*));

static unsigned char nodes[16][32];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(nodes);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(const void* base, unsigned offset) {
    unsigned v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4);
    return v;
}
static void set_word(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4);
}

static int constructs = 0, destroys = 0, live = 0;
static int mask_seen = 0, destroy_flag_bad = 0, head_order_bad = 0;
static unsigned char* last_head = nullptr;
extern "C" void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask) {
    ++constructs;
    ++live;
    if (live != 1) head_order_bad = 1;
    mask_seen = mask;
    last_head = head;
    set_word(iterator, 4, word(head, 0));
}
extern "C" void norrath_list_safe_iterator_next(unsigned char* iterator) {
    const unsigned current = word(iterator, 4);
    set_word(iterator, 4, word(norrath_host_pointer(current), 0));
}
extern "C" void norrath_list_safe_iterator_destroy(unsigned char*, int flags) {
    ++destroys;
    --live;
    if (flags != 2) destroy_flag_bad = 1;
}

static unsigned seen[16];
static unsigned seen_head_index[16];
static int seen_count = 0;
static void visit(void* object) {
    if (seen_count < 16) {
        seen[seen_count] = static_cast<unsigned>(reinterpret_cast<unsigned long>(object));
        seen_head_index[seen_count] = static_cast<unsigned>(last_head - objectGrid);
    }
    ++seen_count;
}
// A node holds next 0 and the object value at 12.
static void link(unsigned char* head, int first, int second) {
    set_word(head, 0, low(nodes[first]));
    set_word(nodes[first], 12, 0x1000u + first);
    if (second >= 0) {
        set_word(nodes[first], 0, low(nodes[second]));
        set_word(nodes[second], 12, 0x1000u + second);
    }
}

int main() {
    set_word(norrath_grid_ignore_flags_view, 0, 0x28u);
    link(objectGrid + (119 * 120 + 119) * 4, 3, -1);   // last normal cell
    link(objectGrid + (0 * 120 + 7) * 4, 2, 5);         // two nodes in one early cell
    link(objectGrid + (1 * 120 + 0) * 4, 1, -1);
    link(ghostObjectGrid + (4 * 120 + 4) * 4, 6, -1);
    link(offTheGrid, 7, -1);
    objectIterateGrid__FPFP10GameObject_v(visit);
    expect(constructs == 120 * 120 * 2 + 1, "one iterator per cell plus the off-grid list");
    expect(destroys == constructs && live == 0, "every iterator is destroyed once, one live at a time");
    expect(destroy_flag_bad == 0, "destroy flag is 2");
    expect(head_order_bad == 0, "iterators do not overlap");
    expect(mask_seen == 0x28, "the ignore mask is passed to every iterator");
    expect(seen_count == 6, "six objects visited");
    const unsigned expected[6] = {0x1002u, 0x1005u, 0x1001u, 0x1003u, 0x1006u, 0x1007u};
    for (int i = 0; i < 6 && i < seen_count; ++i) expect(seen[i] == expected[i], "visit order: row major, then ghost, then off grid");
    // Changing the mask between calls is read fresh.
    set_word(norrath_grid_ignore_flags_view, 0, 0x7fffffffu);
    seen_count = 0;
    constructs = 0;
    objectIterateGrid__FPFP10GameObject_v(visit);
    expect(mask_seen == 0x7fffffff && seen_count == 6, "mask is reread each call");
    if (failures != 0) return 1;
    std::printf("iterate-grid contract passed\n");
    return 0;
}
