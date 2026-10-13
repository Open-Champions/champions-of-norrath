// Host check for _kExitTLBHandler. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <assembled-unit.o>
// Builds the expected instruction words from their MIPS field layout (not from a listing), compares
// them with the assembled object word for word, and requires the 14 address-bearing words to be
// relocations (HI16 on each lui, LO16 on each lw/ld/addiu) against the one frame symbol. Any changed
// register, offset, mask, status-bit constant, relocation kind or length fails.
// A private link at 0x002e0100 with the frame symbol at 0x008e5780 matched the pinned image's 244 bytes.
#include "../w12-startup-rt-a-check/elf_object.hpp"
#include <map>

static std::uint32_t i_type(unsigned op, unsigned rs, unsigned rt, unsigned imm) { return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xffff); }
static std::uint32_t cop0(unsigned sub, unsigned rt, unsigned rd) { return 0x40000000 | (sub << 21) | (rt << 16) | (rd << 11); }

int main(int argc, char** argv) {
    if (argc < 2) return 9;
    ElfObject o(argv[1]);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol("_kExitTLBHandler");
    std::vector<std::uint32_t> w;
    std::map<unsigned, unsigned> rel;   // word index -> expected relocation type (5 HI16, 6 LO16)
    const unsigned at = 1, v0 = 2, k0 = 26, SYNC = 0x0000040f;
    auto lui_v0 = [&] { rel[w.size()] = 5; w.push_back(i_type(15, 0, v0, 0)); };
    auto lo = [&](unsigned op, unsigned off) { rel[w.size()] = 6; w.push_back(i_type(op, v0, v0, off)); };
    w.push_back(cop0(0, at, 12));                  // mfc0 at,Status
    w.push_back(i_type(9, 0, k0, 0xffe4));         // li k0,-28
    w.push_back((at << 21) | (k0 << 16) | (at << 11) | 0x24);   // and at,at,k0
    w.push_back(cop0(4, at, 12));                  // mtc0 at,Status
    w.push_back(SYNC);
    lui_v0(); lo(35, 0x228);                       // lw v0,saved-PC word
    w.push_back(cop0(4, v0, 14)); w.push_back(SYNC);   // mtc0 v0,EPC
    const struct { unsigned off, op; } sp[5] = {{0x200, 0x00400011}, {0x208, 0x70400011}, {0x210, 0x00400013}, {0x218, 0x70400013}, {0x220, 0x00400029}};
    for (auto& e : sp) { lui_v0(); lo(55, e.off); w.push_back(e.op); }   // ld, then mthi/mthi1/mtlo/mtlo1/mtsa v0
    w.push_back(SYNC);
    rel[w.size()] = 5; w.push_back(i_type(15, 0, k0, 0));        // lui k0,frame
    rel[w.size()] = 6; w.push_back(i_type(9, k0, k0, 0));        // addiu k0,k0,frame
    for (unsigned n = 1; n <= 31; ++n) {
        if (n == 26 || n == 27) continue;
        w.push_back(i_type(0x1e, k0, n, 16 * n));  // lq n,16*n(k0)
    }
    w.push_back(cop0(0, k0, 12));                  // mfc0 k0,Status
    w.push_back(i_type(13, k0, k0, 0x13));         // ori k0,k0,0x13
    w.push_back(cop0(4, k0, 12));                  // mtc0 k0,Status
    w.push_back(SYNC);
    w.push_back(0x42000018);                       // eret
    if (!s.found || s.section != ".text" || s.size != w.size() * 4 || s.size != 244 || s.bytes.size() != s.size) { std::puts("shape"); return 1; }
    for (std::size_t i = 0; i < w.size(); ++i)
        if (word_at(s.bytes, 4 * i) != w[i]) { std::printf("word %zu\n", i); return 2; }
    if (s.relocs.size() != rel.size() || rel.size() != 14) { std::puts("reloc count"); return 3; }
    for (auto& r : s.relocs) {
        auto it = rel.find(r.offset / 4);
        if (r.offset % 4 || it == rel.end() || it->second != r.type || r.symbol != "norrath_ee_exception_frame") { std::puts("reloc"); return 4; }
    }
    std::puts("_kExitTLBHandler ok");
    return 0;
}
