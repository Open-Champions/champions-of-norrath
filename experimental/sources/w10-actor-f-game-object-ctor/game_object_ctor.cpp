#include "../vslots-b-bytes/bytes.hpp"
// Observed __10GameObjectG6Point3PQ210GameObject11resourceMapPPc at 0x001411f0, 1124 bytes: the GameObject
// constructor, called by the derived constructors as (object, position, resource map, argument strings).
// Order in the image (o is the object, map the third argument, args the fourth):
//  1. The 12-byte position is copied to a local first (it may overlap the object).
//  2. o+0, o+4 and o+8 become 0; the word at +72 gets the _vt$10GameObject table; floats 24.0 at +28 and 72.0 at
//     +32; +12 becomes 0; bytes 25 at +40 and 73 at +41; the position goes to +16..27; +52 becomes 0; +36 becomes map.
//  3. A non-null map whose word at +4 differs from resourceFrame: when its word at +8 is zero the word at +4
//     becomes resourceFrame; otherwise for each 8-byte entry from byte 8 of the map, until an entry whose first word
//     is zero: lumpFindResource(the map's word at +0, the entry's first word) is stored in the entry's second word
//     (the map is re-read from o+36 after each call, before the store), then the word at +4 becomes resourceFrame.
//  4. Cell x = ((int)pos.x + 15360) >> 8 and y = ((int)pos.y + 15360) >> 8 (arithmetic shifts). With the word at
//     +12 (re-read) lacking bit 1 and 0 <= x, y < 120 the list head is cell 120 * y + x of ghostObjectGrid when bit 11
//     (0x800) of that word is set and of objectGrid otherwise; else offTheGrid. listOnewayLink(head, o+8, o).
//  5. findTagInt(o, "hide", 0) non-zero: the list node reached through o+8 is unlinked (words at +0 and +4 are its
//     next and previous links; the node's word at +8 is the record) and pushed on the front of offTheGrid.
//  6. +44 becomes args; syncRandState = syncRandState * 1664525 + 1013904223 and +68 gets that value >> 16.
//  7. For n = 0, 1, ...: a scan of args for the n-th string whose first four characters equal the in-place lower-cased
//     text "name" and whose fifth character is '=' (strncmp on four characters); the scan returns the string plus 5.
//     When it finds nothing the constructor returns. Otherwise the scan runs a second time with the same n, and
//     objectAddString(its result, o) is called (a null result is passed on as is).
// The lower-casing pass runs at the start of every scan whose args pointer is non-null, using the _ctype_ table
// (upper-case bit 1) on the sign-extended character. The constructor returns nothing meaningful.
// Unknown: what the "hide" and "name" tags, the +44 and +68 words and the grids mean beyond the observed use.
extern "C" {
extern const unsigned char norrath_game_object_vtable_view[];
extern unsigned char norrath_constructor_default_head_view[];
extern unsigned char norrath_constructor_grid_a_view[];
extern unsigned char norrath_constructor_grid_b_view[];
extern unsigned char norrath_constructor_random_state_view[];
extern unsigned char norrath_constructor_resource_frame_view[];
extern unsigned char norrath_constructor_tag_a_view[];
extern unsigned char norrath_constructor_tag_b_view[];
extern const unsigned char _ctype_[];
void* lumpFindResource__FPcT0(char* lump, char* name);
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* item);
int findTagInt__10GameObjectPci(void* object, char* tag, int occurrence);
int norrath_strncmp(const char* left, const char* right, unsigned count) asm("strncmp");
void objectAddString__FPcP10GameObject(char* string, void* object);
}

namespace {
using namespace vslots_b;

// The full address behind a stored 32-bit pointer word (the identity on the target).
Address at(unsigned stored) { return address(pointer(stored)); }

void lower_name_tag() {
    for (Address p = address(norrath_constructor_tag_b_view);; ++p) {
        const unsigned raw = read_u8(p);
        if (raw == 0u) break;
        const int character = raw < 0x80u ? static_cast<int>(raw) : static_cast<int>(raw) - 256;
        const unsigned flags = read_u8(address(_ctype_) + 1u + static_cast<unsigned>(character));
        write_u8(p, (flags & 1u) != 0u ? static_cast<unsigned>(character + 32) : static_cast<unsigned>(character));
    }
}

// A grid cell coordinate of a position float: (int)value + 15360, arithmetic shift right by 8.
int grid_cell(Address field) {
    const unsigned biased = static_cast<unsigned>(static_cast<int>(read_single(field))) + 15360u;
    return signed_word((biased >> 8) | ((biased & 0x80000000u) != 0u ? 0xff000000u : 0u));
}

// The n-th "name=" string of the argument list, or null.
Address find_named(Address object, unsigned n) {
    if (read_word(object + 44u) == 0u) return 0u;
    lower_name_tag();
    Address base = at(read_word(object + 44u));
    if (read_word(base) == 0u) return 0u;
    unsigned remaining = n;
    unsigned offset = 0u;
    unsigned cursor = 0u;
    for (;;) {
        const Address candidate = at(read_word(base + cursor));
        cursor += 4u;
        const int compared = norrath_strncmp(reinterpret_cast<const char*>(candidate),
                                     reinterpret_cast<const char*>(norrath_constructor_tag_b_view), 4u);
        const Address arguments = at(read_word(object + 44u));
        if (compared == 0) {
            const Address current = at(read_word(arguments + offset));
            const unsigned fifth = read_u8(current + 4u);
            if ((fifth < 0x80u ? static_cast<int>(fifth) : static_cast<int>(fifth) - 256) == 61) {
                remaining -= 1u;
                if (signed_word(remaining) == -1) return current + 5u;
            }
        }
        base = arguments;
        offset = cursor;
        if (read_word(arguments + cursor) == 0u) return 0u;
    }
}
}  // namespace

extern "C" void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, const void* point, void* resource_map,
                                                                      char** arguments) {
    const Address object = address(self);
    const Address head = object + 8u;
    unsigned char saved[12];
    for (unsigned i = 0; i < 12u; ++i) saved[i] = bytes(address(point))[i];
    write_word(object, 0u);
    write_word(object + 4u, 0u);
    write_word(object + 72u, address_bits(norrath_game_object_vtable_view));
    write_word(head, 0u);
    write_word(object + 28u, 0x41c00000u);
    write_word(object + 32u, 0x42900000u);
    write_word(object + 12u, 0u);
    write_u8(object + 40u, 25u);
    write_u8(object + 41u, 73u);
    for (unsigned i = 0; i < 12u; ++i) bytes(object + 16u)[i] = saved[i];
    write_word(object + 52u, 0u);
    Address map = address(resource_map);
    write_word(object + 36u, map);
    if (map != 0u) {
        const unsigned stamp = read_word(address(norrath_constructor_resource_frame_view));
        if (read_word(map + 4u) != stamp) {
            if (read_word(map + 8u) == 0u) {
                write_word(map + 4u, stamp);
            } else {
                unsigned offset = 8u;
                for (;;) {
                    void* found = lumpFindResource__FPcT0(static_cast<char*>(pointer(read_word(map))),
                                                          static_cast<char*>(pointer(read_word(map + offset))));
                    map = at(read_word(object + 36u));
                    write_word(map + offset + 4u, address_bits(found));
                    offset += 8u;
                    if (read_word(map + offset) == 0u) break;
                }
                write_word(map + 4u, read_word(address(norrath_constructor_resource_frame_view)));
            }
        }
    }
    const int cell_x = grid_cell(object + 16u);
    const int cell_y = grid_cell(object + 20u);
    const unsigned flags = read_word(object + 12u);
    Address list = address(norrath_constructor_default_head_view);
    if ((flags & 2u) == 0u && cell_x >= 0 && cell_x < 120 && cell_y >= 0 && cell_y < 120) {
        const Address grid = (flags & 0x800u) != 0u ? address(norrath_constructor_grid_b_view)
                                                  : address(norrath_constructor_grid_a_view);
        list = grid + 4u * (120u * static_cast<unsigned>(cell_y) + static_cast<unsigned>(cell_x));
    }
    listOnewayLink__FP8ListHeadT0Pv(pointer(list), pointer(head), self);
    if (findTagInt__10GameObjectPci(self, reinterpret_cast<char*>(norrath_constructor_tag_a_view), 0) != 0) {
        const Address record = at(read_word(at(read_word(head)) + 8u));
        write_word(at(read_word(record + 4u)), read_word(record));
        if (read_word(record) != 0u) write_word(at(read_word(record)) + 4u, read_word(record + 4u));
        const Address front = address(norrath_constructor_default_head_view);
        const unsigned old_first = read_word(front);
        write_word(record + 4u, address_bits(norrath_constructor_default_head_view));
        write_word(record, old_first);
        write_word(front, address_bits(bytes(record)));
        if (read_word(record) != 0u) write_word(at(read_word(record)) + 4u, address_bits(bytes(record)));
    }
    write_word(object + 44u, address_bits(arguments));
    const unsigned seed = read_word(address(norrath_constructor_random_state_view)) * 1664525u + 1013904223u;
    write_word(address(norrath_constructor_random_state_view), seed);
    write_word(object + 68u, seed >> 16);
    unsigned occurrence = 0u;
    for (;;) {
        if (find_named(object, occurrence) == 0u) return;
        const Address chosen = find_named(object, occurrence);
        occurrence += 1u;
        objectAddString__FPcP10GameObject(reinterpret_cast<char*>(chosen), self);
    }
}
