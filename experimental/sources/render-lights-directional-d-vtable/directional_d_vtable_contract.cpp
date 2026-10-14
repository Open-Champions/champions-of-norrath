// Host check of the EE object for norrath_factory_directional_d_vtable.
// Not an EE plan input. Pass the reviewed-toolchain object as argv[1].
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_u32(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned read_u16(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: directional_d_vtable_contract <ee-object>\n");
        return 2;
    }
    std::ifstream in(argv[1], std::ios::binary);
    std::vector<unsigned char> elf((std::istreambuf_iterator<char>(in)),
                                   std::istreambuf_iterator<char>());
    expect(elf.size() > 52 && elf[0] == 0x7f && elf[1] == 'E', "elf");
    if (failures) return 1;
    const unsigned shoff = read_u32(elf.data() + 32);
    const unsigned shentsize = read_u16(elf.data() + 46);
    const unsigned shnum = read_u16(elf.data() + 48);
    const unsigned shstrndx = read_u16(elf.data() + 50);
    auto section = [&](unsigned index) { return elf.data() + shoff + index * shentsize; };
    const unsigned str_off = read_u32(section(shstrndx) + 16);
    auto name_of = [&](const unsigned char* sh) {
        return reinterpret_cast<const char*>(elf.data() + str_off + read_u32(sh));
    };
    const unsigned char* data_sh = nullptr;
    const unsigned char* rel_sh = nullptr;
    const unsigned char* sym_sh = nullptr;
    for (unsigned i = 0; i < shnum; ++i) {
        const char* name = name_of(section(i));
        if (std::strcmp(name, ".data") == 0) data_sh = section(i);
        if (std::strcmp(name, ".rel.data") == 0) rel_sh = section(i);
        if (std::strcmp(name, ".symtab") == 0) sym_sh = section(i);
    }
    expect(data_sh && rel_sh && sym_sh, "required sections");
    if (failures) return 1;
    const unsigned data_off = read_u32(data_sh + 16);
    const unsigned data_size = read_u32(data_sh + 20);
    expect(data_size == 144u, "vtable is 144 bytes");
    bool zeros = true;
    for (unsigned i = 0; i < data_size; ++i) if (elf[data_off + i] != 0) zeros = false;
    expect(zeros, "slot words are zero before relocation");
    const unsigned sym_off = read_u32(sym_sh + 16);
    const unsigned sym_size = read_u32(sym_sh + 20);
    const unsigned sym_entsize = read_u32(sym_sh + 36);
    const unsigned strtab_off = read_u32(section(read_u32(sym_sh + 24)) + 16);
    bool found = false;
    for (unsigned i = 0; i < sym_size / sym_entsize; ++i) {
        const unsigned char* sym = elf.data() + sym_off + i * sym_entsize;
        const char* name = reinterpret_cast<const char*>(elf.data() + strtab_off + read_u32(sym));
        if (std::strcmp(name, "norrath_factory_directional_d_vtable") == 0) {
            found = true;
            expect(read_u32(sym + 4) == 0u, "vtable address");
            expect(read_u32(sym + 8) == 144u, "vtable symbol size");
            expect((sym[12] & 0xf) == 1, "object symbol");
            expect((sym[12] >> 4) == 1, "global binding");
        }
    }
    expect(found, "vtable symbol");
    const char* expected[] = {
        "norrath_vt_tf_22charDirectional_LightD",
        "norrath_vt_dtor_22charDirectional_LightD",
        "norrath_vt_msg_hurt_10GameObject",
        "norrath_vt_msg_trigger_10GameObject",
        "norrath_vt_msg_run_10GameObject",
        "norrath_vt_msg_draw_10GameObject",
        "norrath_vt_msg_collision_10GameObject",
        "norrath_vt_msg_route_10GameObject",
        "norrath_vt_msg_load_10GameObject",
        "norrath_vt_msg_save_10GameObject",
        "norrath_vt_msg_collision_world_10GameObject",
        "norrath_vt_msg_move_collision_10GameObject",
        "norrath_vt_msg_alert_10GameObject",
        "norrath_vt_msg_get_poke_name_10GameObject",
        "norrath_vt_msg_poke_10GameObject",
        "norrath_vt_clone_22charDirectional_LightD",
        "norrath_vt_get_model_10GameObject"
    };
    const unsigned rel_off = read_u32(rel_sh + 16);
    const unsigned rel_size = read_u32(rel_sh + 20);
    const unsigned rel_entsize = read_u32(rel_sh + 36);
    const unsigned count = rel_size / rel_entsize;
    expect(count == 17u, "seventeen function slots");
    for (unsigned i = 0; i < count && i < 17; ++i) {
        const unsigned char* rel = elf.data() + rel_off + i * rel_entsize;
        const unsigned offset = read_u32(rel);
        const unsigned info = read_u32(rel + 4);
        expect(offset == 4u + 8u * i, expected[i]);
        expect((info & 0xffu) == 2u, "R_MIPS_32");
        const unsigned char* sym = elf.data() + sym_off + (info >> 8) * sym_entsize;
        const char* name = reinterpret_cast<const char*>(elf.data() + strtab_off + read_u32(sym));
        expect(std::strcmp(name, expected[i]) == 0, expected[i]);
    }
    if (failures) return 1;
    std::printf("directional_d_vtable contract passed\n");
    return 0;
}
