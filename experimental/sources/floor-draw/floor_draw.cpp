/* CPU side of floorDraw at 0x001922b8, 2908 bytes. This builds the
 * submission chain and calls the real queue, clip, and resource providers.
 * It does not upload microcode or talk to the GS. The VU span between
 * vu1floorstart (0x00333df0) and vu1floorend (0x00334120) stays an import.
 *
 * Kind 2 advances by the .lit4 word 0x3d4ccccc at 0x004801dc. Every other
 * kind uses 0x3e4ccccc at 0x004801e0. Those words are one ulp below the
 * decimal literals 0.05f and 0.2f. They are compiler constants, not variables.
 */
extern "C" {
int norrath_game_is_paused();
float norrath_integer_cosine(int angle);
float norrath_integer_sine(int angle);
unsigned ee_cvt_w_s_word(float value);
int worldClipCubeToFrustum__FPA1_fi(float* six_bounds, int plane_mask);
void __11ResourceMapPcT1(void* map, char* group, char* name);
void* norrath_player_resource_map_get(void* map);
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list,
                                                   void* texture, void* record,
                                                   int flags);

extern unsigned char floorDrawEnable;
extern unsigned glistp;
extern unsigned long long zwritesOff;
extern unsigned long long zwritesOn;
extern unsigned char worldWorldViewMatrix[];
extern unsigned char worldCameraPos[];
extern unsigned char norrath_floor_sprite_count_word_view[];
extern unsigned char norrath_floor_expand_list_view[];
extern unsigned char norrath_floor_sprite_grid_view[];
extern unsigned char norrath_floor_fade_packet_word_view[];
extern unsigned char norrath_floor_blood_level_word_view[];
extern unsigned char norrath_floor_program_start_view[];
extern unsigned char norrath_floor_program_end_view[];
extern unsigned char norrath_floor_sprite_texture_map[];
extern unsigned char norrath_floor_sprite_texture_ready_word[];
extern char norrath_floor_sprite_texture_group[];
extern char norrath_floor_sprite_texture_name[];
}

#if defined(NORRATH_HOST_FLOOR_DRAW)
extern unsigned long long norrath_floor_host_address_base;
#endif

namespace {
using Wide = unsigned long long;
using Address = unsigned;

inline Address address(const void* pointer) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline volatile unsigned char* bytes(Address a) {
#if defined(NORRATH_HOST_FLOOR_DRAW)
    return reinterpret_cast<volatile unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(norrath_floor_host_address_base + a));
#else
    return reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
#endif
}
inline unsigned read_word(Address a) {
    const unsigned b0 = bytes(a)[0], b1 = bytes(a)[1], b2 = bytes(a)[2], b3 = bytes(a)[3];
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}
inline unsigned read_half(Address a) {
    return bytes(a)[0] | (unsigned(bytes(a)[1]) << 8);
}
inline void write_word(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v);
    bytes(a)[1] = static_cast<unsigned char>(v >> 8);
    bytes(a)[2] = static_cast<unsigned char>(v >> 16);
    bytes(a)[3] = static_cast<unsigned char>(v >> 24);
}
inline void write_half(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v);
    bytes(a)[1] = static_cast<unsigned char>(v >> 8);
}
inline int signed_half(unsigned v) {
    v &= 0xffffu;
    return v < 0x8000u ? static_cast<int>(v) : static_cast<int>(v) - 65536;
}
inline long long signed_word(unsigned v) {
    return v < 0x80000000u ? static_cast<long long>(v)
                           : static_cast<long long>(v) - 0x100000000LL;
}
inline unsigned byte(Address a) { return bytes(a)[0]; }
inline void store_byte(Address a, unsigned v) { bytes(a)[0] = static_cast<unsigned char>(v); }
inline int signed_byte(unsigned v) { return v < 128 ? int(v) : int(v) - 256; }
inline void* pointer(Address a) {
#if defined(NORRATH_HOST_FLOOR_DRAW)
    return const_cast<unsigned char*>(bytes(a));
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(a));
#endif
}
inline unsigned asr(unsigned v, unsigned n) {
    return (v >> n) | ((0u - (v >> 31)) << (32u - n));
}
inline Wide native_word(unsigned v) { return static_cast<Wide>(signed_word(v)); }
inline Wide read_wide(Address a) {
    return Wide(read_word(a)) | (Wide(read_word(a + 4u)) << 32);
}
inline void write_wide(Address a, Wide v) {
    write_word(a, unsigned(v));
    write_word(a + 4u, unsigned(v >> 32));
}
inline float float_word(unsigned word) {
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(word);
    p[1] = static_cast<unsigned char>(word >> 8);
    p[2] = static_cast<unsigned char>(word >> 16);
    p[3] = static_cast<unsigned char>(word >> 24);
    return value;
}
inline float read_float(Address a) { return float_word(read_word(a)); }
inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    bytes(a)[0] = p[0];
    bytes(a)[1] = p[1];
    bytes(a)[2] = p[2];
    bytes(a)[3] = p[3];
}
inline float ordered_add(float left, float right) {
    float result;
#if defined(NORRATH_HOST_FLOOR_DRAW)
    result = left + right;
#else
    __asm__ volatile("add.s %0, %1, %2" : "=f"(result) : "f"(left), "f"(right) : "memory");
#endif
    return result;
}
inline float ordered_multiply(float left, float right) {
    float result;
#if defined(NORRATH_HOST_FLOOR_DRAW)
    result = left * right;
#else
    __asm__ volatile("mul.s %0, %1, %2" : "=f"(result) : "f"(left), "f"(right) : "memory");
#endif
    return result;
}
inline unsigned ee_c_le_s_word(float left, float right) {
#if defined(NORRATH_HOST_FLOOR_DRAW)
    return left <= right ? 1u : 0u;
#else
    unsigned result;
    __asm__ volatile(
        ".set push\n\t"
        ".set noreorder\n\t"
        "c.le.s %1, %2\n\t"
        "bc1f 1f\n\t"
        "addu %0, $zero, $zero\n\t"
        "addiu %0, $zero, 1\n\t"
        "1:\n\t"
        ".set pop"
        : "=r"(result)
        : "f"(left), "f"(right)
        : "cc");
    return result;
#endif
}
inline float ee_cvt_s_w_float(int value) {
#if defined(NORRATH_HOST_FLOOR_DRAW)
    return static_cast<float>(value);
#else
    float result;
    __asm__ volatile("mtc1 %1, %0\n\tcvt.s.w %0, %0" : "=f"(result) : "r"(value));
    return result;
#endif
}
inline float ee_sub_s_float(float left, float right) {
#if defined(NORRATH_HOST_FLOOR_DRAW)
    return left - right;
#else
    float result;
    __asm__ volatile("sub.s %0, %1, %2" : "=f"(result) : "f"(left), "f"(right) : "memory");
    return result;
#endif
}
#if defined(NORRATH_HOST_FLOOR_DRAW)
inline unsigned host_cvt_w_s(float value) {
    return static_cast<unsigned>(static_cast<int>(value));
}
#endif
inline unsigned convert_w_s(float value) {
#if defined(NORRATH_HOST_FLOOR_DRAW)
    return host_cvt_w_s(value);
#else
    return ee_cvt_w_s_word(value);
#endif
}

inline Address pad_to(Address cursor, unsigned residue) {
    while ((cursor & 15u) != residue) {
        store_byte(cursor, 0);
        ++cursor;
    }
    return cursor;
}
inline unsigned tag_count(Address end, Address start) {
    return asr(asr(end - start, 3u) - 2u, 1u);
}
inline Wide gif_ad_tag(unsigned loops) {
    return (Wide(1) << 60) | (1u << 15) | loops;
}
inline unsigned vif(unsigned command, unsigned count, unsigned immediate) {
    return (command << 24) | (count << 16) | immediate;
}
inline void gs_register(Address out, Wide data, unsigned reg) {
    write_wide(out + 8u, reg);
    write_wide(out, data);
}
inline unsigned sprite_offset(unsigned index) {
    return (index * 3u) << 3;
}

void advance_expansion() {
    const float type_two_step = float_word(0x3d4cccccu);
    const float other_step = float_word(0x3e4cccccu);
    Address record = address(norrath_floor_expand_list_view);
    for (int remaining = 15; remaining >= 0; --remaining, record += 20u) {
        if (read_word(record + 4u) == 0) continue;
        const int kind = signed_byte(byte(record));
        const float previous = read_float(record + 12u);
        const float radius = ordered_add(previous, kind == 2 ? type_two_step : other_step);
        const int angle_x = signed_half(read_half(record + 8u));
        const Address target_x = read_word(record + 4u);
        const float cosine = norrath_integer_cosine(angle_x);
        const Address target_y = read_word(record + 4u);
        const unsigned x = convert_w_s(ordered_multiply(ordered_multiply(cosine, radius), 4.0f));
        write_half(target_x, x);
        const int angle_y = signed_half(read_half(record + 8u));
        const float sine = norrath_integer_sine(angle_y);
        write_float(record + 12u, radius);
        const float partial_y = ordered_multiply(sine, radius);
        const float limit = read_float(record + 16u);
        const unsigned expired = ee_c_le_s_word(limit, radius);
        const unsigned y = convert_w_s(ordered_multiply(partial_y, 4.0f));
        write_half(target_y + 2u, y);
        if (expired) write_word(record + 4u, 0);
    }
}

Address begin_register_packet(Address start, Address& direct, Address& gif) {
    write_word(start + 8u, vif(0x11, 0, 0));
    direct = pad_to(start + 12u, 12u);
    gif = direct + 4u;
    write_wide(gif, gif_ad_tag(0));
    write_wide(gif + 8u, 0x0eu);
    gs_register(direct + 20u, 0, 0x3fu);
    gs_register(direct + 36u, 0, 0x06u);
    gs_register(direct + 52u, 0x61u, 0x14u);
    write_word(direct, vif(0x50, 0, 0));
    const Wide z_off = read_wide(address(&zwritesOff));
    gs_register(direct + 68u, z_off, 0x4eu);
    gs_register(direct + 84u, (Wide(5) << 16) | 0x1du, 0x47u);
    gs_register(direct + 100u, 0x44u, 0x42u);
    write_word(direct + 116u, 0x80808080u);
    write_wide(direct + 124u, 1u);
    write_word(direct + 120u, 0);
    const Address end = direct + 132u;
    const Wide tag = read_wide(gif);
    const unsigned command = read_word(direct);
    write_wide(gif, tag | native_word(tag_count(end, gif)));
    write_word(direct, command | asr(end - direct - 4u, 4u));
    return pad_to(end, 0);
}

void setup_pair(Address out, unsigned offset, unsigned x, unsigned y) {
    write_half(out + offset, x);
    write_half(out + offset + 2u, y);
}
Address emit_program_and_setup(Address cursor, Address first_tag) {
    const unsigned program_size = address(norrath_floor_program_end_view) -
                                  address(norrath_floor_program_start_view);
    const unsigned rounded = asr(program_size + 15u, 4u) << 4;
    const unsigned start_unit = asr(16384u - rounded, 3u);
    const Wide program_ref = (Wide(address(norrath_floor_program_start_view)) << 32) |
                             native_word(asr(rounded, 4u)) | 0x30000000u;
    const Address m = address(worldWorldViewMatrix);
    const float first_matrix_word = read_float(m);
    write_wide(first_tag, native_word(tag_count(cursor, first_tag)) | 0x10000000u);
    write_word(cursor + 12u, start_unit | (rounded << 13) | vif(0x4a, 0, 0));
    write_wide(cursor, program_ref);
    write_word(cursor + 8u, 0);
    write_float(cursor + 28u, first_matrix_word);
    const Address setup = cursor + 16u;
    const float matrix_1 = read_float(m + 4u);
    write_word(cursor + 24u, vif(0x6c, 5, 0x3ceu));
    write_float(cursor + 32u, matrix_1);
    for (unsigned i = 2; i != 16; ++i) {
        const float word = read_float(m + 4u * i);
        write_float(cursor + 28u + 4u * i, word);
    }
    write_word(cursor + 92u, 0x8000u);
    write_word(cursor + 96u, (0x90aau << 16) | 0x4000u);
    write_word(cursor + 100u, (0x3434u << 16) | 0x3431u);
    write_word(cursor + 104u, 4u);
    write_word(cursor + 108u, vif(0x65, 22, 0x3d3u));
    setup_pair(cursor, 112u, 8u, 8u);
    setup_pair(cursor, 116u, 1024u, 1024u);
    setup_pair(cursor, 120u, 1032u, 1032u);
    setup_pair(cursor, 124u, 1536u, 1536u);
    setup_pair(cursor, 128u, 1032u, 1544u);
    setup_pair(cursor, 132u, 1536u, 2048u);
    setup_pair(cursor, 136u, 1032u, 8u);
    setup_pair(cursor, 140u, 2048u, 1024u);
    setup_pair(cursor, 144u, 1544u, 1032u);
    setup_pair(cursor, 148u, 2048u, 1536u);
    setup_pair(cursor, 152u, 1544u, 1544u);
    setup_pair(cursor, 156u, 2048u, 2048u);
    setup_pair(cursor, 160u, 2056u, 8u);
    setup_pair(cursor, 164u, 3072u, 1024u);
    setup_pair(cursor, 168u, 2056u, 1032u);
    setup_pair(cursor, 172u, 2560u, 1536u);
    setup_pair(cursor, 176u, 2056u, 1544u);
    setup_pair(cursor, 180u, 2560u, 2048u);
    setup_pair(cursor, 184u, 8u, 1032u);
    setup_pair(cursor, 188u, 1024u, 2048u);
    setup_pair(cursor, 192u, 3080u, 8u);
    setup_pair(cursor, 196u, 4096u, 1024u);
    write_word(cursor + 200u, start_unit | vif(0x14, 0, 0));
    const Address end = pad_to(cursor + 204u, 0);
    write_wide(setup, native_word(tag_count(end, setup)) | 0x10000000u);
    return end;
}

struct Region {
    unsigned mask;
    signed char x0, y0, x1, y1;
};
static_assert(sizeof(Region) == 8, "original subdivision record size");
struct Submission {
    unsigned total, maximum;
    Address largest;
};
int clip_region(const Region& r) {
    const float top = read_float(address(worldCameraPos) + 8u);
    const float x1 = ee_cvt_s_w_float(int((unsigned(int(r.x1)) << 8) - 3548u));
    const float y0 = ee_cvt_s_w_float(int((unsigned(int(r.y0)) << 8) - 3876u));
    const float y1 = ee_cvt_s_w_float(int((unsigned(int(r.y1)) << 8) - 3548u));
    const float x0 = ee_cvt_s_w_float(int((unsigned(int(r.x0)) << 8) - 3876u));
    const float bottom = ee_sub_s_float(top, 480.0f);
    float bounds[6] = {x0, x1, y0, y1, bottom, top};
    return worldClipCubeToFrustum__FPA1_fi(bounds, int(r.mask));
}
int clip_leaf(const Region& r, Address packet) {
    const float y1 = ee_cvt_s_w_float(int((unsigned(int(r.y1)) << 8) - 3548u));
    const float x0 = ee_cvt_s_w_float(int((unsigned(int(r.x0)) << 8) - 3876u));
    const float x1 = ee_cvt_s_w_float(int((unsigned(int(r.x1)) << 8) - 3548u));
    const float y0 = ee_cvt_s_w_float(int((unsigned(int(r.y0)) << 8) - 3876u));
    const float bottom = ee_cvt_s_w_float(signed_half(read_half(packet + 756u)));
    const float top = ee_cvt_s_w_float(signed_half(read_half(packet + 758u)));
    float bounds[6] = {x0, x1, y0, y1, bottom, top};
    return worldClipCubeToFrustum__FPA1_fi(bounds, int(r.mask));
}
Address emit_visible_cells(Address cursor, Submission& result) {
    Region pending[32];
    pending[0] = Region{0x8000u, 0, 0, 29, 29};
    unsigned used = 1, buffer_bank = 0;
    do {
        Region r = pending[--used];
        if (r.x0 < r.x1 || r.y0 < r.y1) {
            if (r.mask != 0x803fu) {
                const int mask = clip_region(r);
                if (mask == 0) continue;
                r.mask = unsigned(mask);
            }
            Region low = r, high = r;
            if (int(r.y1) - int(r.y0) < int(r.x1) - int(r.x0)) {
                const int midpoint = (int(r.x1) + int(r.x0)) >> 1;
                low.x1 = static_cast<signed char>(midpoint);
                high.x0 = static_cast<signed char>(midpoint + 1);
            } else {
                const int midpoint = (int(r.y1) + int(r.y0)) >> 1;
                low.y1 = static_cast<signed char>(midpoint);
                high.y0 = static_cast<signed char>(midpoint + 1);
            }
            pending[used++] = low;
            pending[used++] = high;
            continue;
        }
        const unsigned row = unsigned(int(r.y0));
        const unsigned thirty_row = (row * 30u);
        const unsigned index = thirty_row + unsigned(int(r.x0));
        const Address cell = address(norrath_floor_sprite_grid_view) + (index << 2);
        Address packet = read_word(cell);
        if (packet == 0) continue;
        if (r.mask != 0x803fu) {
            if (clip_leaf(r, packet) == 0) continue;
            packet = read_word(cell);
        }
        const unsigned sprite_count = read_word(packet + 760u);
        write_word(cursor + 8u, sprite_count | (1u << 26));
        const Wide ref = (Wide(packet) << 32) | 0x30000000u | 47u;
        const unsigned count = read_word(packet + 764u);
        write_wide(cursor, ref);
        result.total += count;
        if (signed_word(result.maximum) < signed_word(count)) {
            result.maximum = count;
            result.largest = packet;
        }
        const unsigned command = buffer_bank | vif(0x60, 0, 0) | (0x015du << 16) | (0x0cu << 24);
        buffer_bank ^= 0x200u;
        write_word(cursor + 12u, command);
        cursor += 16u;
    } while (used != 0);
    return cursor;
}

Address end_register_packet(Address start) {
    write_word(start + 8u, vif(0x11, 0, 0));
    const Address direct = pad_to(start + 12u, 12u), gif = direct + 4u;
    write_wide(gif, gif_ad_tag(0));
    write_wide(gif + 8u, 0x0eu);
    write_word(direct, vif(0x50, 0, 2));
    const Wide z_on = read_wide(address(&zwritesOn));
    gs_register(gif + 16u, z_on, 0x4eu);
    Address end = gif + 32u;
    const Wide tag = read_wide(gif);
    const Wide chain = native_word(tag_count(end, start)) | 0x10000000u;
    write_wide(gif, tag | native_word(tag_count(end, gif)));
    write_wide(start, chain);
    if (((chain >> 28) & 7u) == 1) {
        write_wide(start, (chain & 0xffffffff8fffffffull) | 0x60000000u);
    } else {
        write_wide(end + 8u, 0);
        write_wide(end, 0x60000000u);
        end += 16u;
    }
    return end;
}

void fade_one_sprite(unsigned total, Address largest) {
    const Address fade_word = address(norrath_floor_fade_packet_word_view);
    Address packet = read_word(fade_word);
    if (signed_word(total) >= 46) {
        if (packet == 0) packet = largest;
        write_word(fade_word, packet);
    }
    if (packet == 0) return;
    if (signed_word(read_word(packet + 764u)) <= 0) packet = 0;
    write_word(fade_word, packet);
    if (packet == 0) return;
    if (read_word(address(norrath_floor_blood_level_word_view)) != 0) return;
    if (signed_word(read_word(packet + 760u)) <= 0) return;
    unsigned index = 0;
    while (signed_half(read_half(packet + sprite_offset(index) + 14u)) == 0) {
        ++index;
        if (signed_word(index) >= signed_word(read_word(packet + 760u))) return;
    }
    const unsigned count = read_word(packet + 760u);
    if (signed_word(index) >= signed_word(count)) return;
    const unsigned offset = sprite_offset(index);
    const unsigned alpha = read_half(packet + offset + 6u) - 4u;
    write_half(packet + offset + 6u, alpha);
    if (signed_half(alpha) >= 0) return;
    unsigned moving = index, move_offset = offset;
    packet = read_word(fade_word);
    if (signed_word(moving) < signed_word(count - 1u)) {
        do {
            const Address destination = packet + move_offset;
            const Wide a = read_wide(destination + 24u);
            const Wide b = read_wide(destination + 32u);
            const Wide c = read_wide(destination + 40u);
            write_wide(destination, a);
            write_wide(destination + 8u, b);
            write_wide(destination + 16u, c);
            ++moving;
            packet = read_word(fade_word);
            const unsigned current_count = read_word(packet + 760u);
            move_offset += 24u;
            if (signed_word(moving) >= signed_word(current_count - 1u)) break;
        } while (true);
    }
    const unsigned old_count = read_word(packet + 760u);
    const unsigned old_total = read_word(packet + 764u);
    write_word(packet + 760u, old_count - 1u);
    write_word(packet + 764u, old_total - 1u);
    write_word(fade_word, 0);
}
}

extern "C" void floorDraw__Fv() {
    if (signed_byte(byte(address(&floorDrawEnable))) == 0) return;
    if (read_word(address(norrath_floor_sprite_count_word_view)) == 0) return;
    Submission result{0, 0, 0};
    const int paused = norrath_game_is_paused();
    Address cursor = read_word(address(&glistp));
    if (paused == 0) {
        advance_expansion();
        cursor = read_word(address(&glistp));
    }
    const Address first = cursor;
    Address direct, gif;
    cursor = begin_register_packet(cursor, direct, gif);
    cursor = emit_program_and_setup(cursor, first);
    cursor = emit_visible_cells(cursor, result);
    cursor = end_register_packet(cursor);
    const Address initialized = address(norrath_floor_sprite_texture_ready_word);
    if (read_word(initialized) == 0) {
        __11ResourceMapPcT1(norrath_floor_sprite_texture_map,
                            norrath_floor_sprite_texture_group,
                            norrath_floor_sprite_texture_name);
        write_word(initialized, 1u);
    }
    const Address submission_start = read_word(address(&glistp));
    write_word(address(&glistp), cursor);
    void* texture = norrath_player_resource_map_get(norrath_floor_sprite_texture_map);
    (void)drawAddToList__FPviP8_textureP11_drawRecordi(pointer(submission_start), 0,
                                                       texture, nullptr, 0);
    fade_one_sprite(result.total, result.largest);
}
