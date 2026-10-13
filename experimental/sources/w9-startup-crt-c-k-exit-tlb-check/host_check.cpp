// Check for _kExitTLBHandler (authored in w8-startup-sce-b-k-exit-tlb-handler, which has no
// committed check). Assembles that unit with the reviewed EE compiler and compares the resulting
// instruction sequence with the sequence the unit's own description states: clear Status bits
// 0, 1, 3, 4; load EPC from the frame's saved-PC word; reload HI, HI1, LO, LO1, SA; reload the
// 128-bit registers 1..25 and 28..31 from 16*n; set Status bits 0, 1, 4; ERET. Every address
// word must be a relocation against the one imported frame symbol.
//
// Build and run on the host (needs a shell; the original executable is not read):
//   c++ -std=c++17 -o check host_check.cpp && ./check <ee-toolchain-root> <experimental/sources>
// A private byte comparison of the same assembly against the executable, ignoring the 14
// address-bearing words (seven high/low pairs), matched once.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

struct Expected {
    std::string mnemonic;
    std::string operands;
    std::string relocation;
};

static std::vector<Expected> expected() {
    const std::string frame = "norrath_ee_exception_frame";
    std::vector<Expected> s = {
        {"mfc0", "at,c0_sr", ""},       {"li", "k0,-28", ""},          {"and", "at,at,k0", ""},
        {"mtc0", "at,c0_sr", ""},       {"sync.p", "", ""},            {"lui", "v0,0x0", frame},
        {"lw", "v0,552(v0)", frame},    {"mtc0", "v0,c0_epc", ""},     {"sync.p", "", ""},
    };
    const struct { int off; const char* op; } specials[] = {
        {512, "mthi"}, {520, "mthi1"}, {528, "mtlo"}, {536, "mtlo1"}, {544, "mtsa"}};
    for (auto& p : specials) {
        s.push_back({"lui", "v0,0x0", frame});
        s.push_back({"ld", "v0," + std::to_string(p.off) + "(v0)", frame});
        s.push_back({p.op, "v0", ""});
    }
    s.push_back({"sync.p", "", ""});
    s.push_back({"lui", "k0,0x0", frame});
    s.push_back({"addiu", "k0,k0,0", frame});
    const char* names[32] = {"", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2",
                             "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5",
                             "s6", "s7", "t8", "t9", "", "", "gp", "sp", "s8", "ra"};
    for (int n = 1; n <= 31; ++n) {
        if (n == 26 || n == 27) continue;
        s.push_back({"lq", std::string(names[n]) + "," + std::to_string(16 * n) + "(k0)", ""});
    }
    s.push_back({"mfc0", "k0,c0_sr", ""});
    s.push_back({"ori", "k0,k0,0x13", ""});
    s.push_back({"mtc0", "k0,c0_sr", ""});
    s.push_back({"sync.p", "", ""});
    s.push_back({"eret", "", ""});
    return s;
}

static std::string run(const std::string& command) {
    std::string out;
    FILE* p = popen(command.c_str(), "r");
    if (!p) return out;
    char buf[512];
    while (std::fgets(buf, sizeof buf, p)) out += buf;
    if (pclose(p) != 0) out.clear();
    return out;
}

int main(int argc, char** argv) {
    if (argc != 3) return 1;
    const std::string tc = argv[1], root = argv[2];
    const std::string obj = "/tmp/norrath_ktlb_check.o";
    const std::string compile =
        tc + "/bin/mips64r5900el-ps2-elf-g++ -G0 -O2 -ffreestanding -fno-pic -mabi=eabi "
             "-march=r5900 -mfp32 -mgp64 -mlong32 -mno-abicalls -msingle-float -std=c++17 "
             "-nostdinc -nostdinc++ -x c++ -c " + root +
        "/w8-startup-sce-b-k-exit-tlb-handler/k_exit_tlb_handler.cpp -o " + obj + " && echo built";
    if (run(compile).empty()) return 2;
    const std::string text = run(tc + "/bin/mips64r5900el-ps2-elf-objdump -dr " + obj);
    if (text.empty()) return 3;

    std::vector<Expected> got;
    size_t pos = 0;
    while (pos < text.size()) {
        size_t end = text.find('\n', pos);
        if (end == std::string::npos) end = text.size();
        const std::string line = text.substr(pos, end - pos);
        pos = end + 1;
        unsigned addr, word;
        char mnem[32] = "", ops[128] = "", sym[128] = "", kind[32] = "";
        int n = std::sscanf(line.c_str(), " %x: %x %31s %127s", &addr, &word, mnem, ops);
        if (n >= 3 && line.find("R_MIPS") == std::string::npos && line.find('\t') != std::string::npos) {
            if (n < 4) ops[0] = 0;
            got.push_back({mnem, ops, ""});
        } else if (std::sscanf(line.c_str(), " %x: %31s %127s", &addr, kind, sym) == 3 &&
                   std::string(kind).rfind("R_MIPS", 0) == 0 && !got.empty()) {
            got.back().relocation = sym;
        }
    }
    const auto want = expected();
    if (got.size() != want.size()) {
        std::printf("instruction count %zu != %zu\n", got.size(), want.size());
        return 4;
    }
    for (size_t i = 0; i < want.size(); ++i) {
        if (got[i].mnemonic != want[i].mnemonic || got[i].operands != want[i].operands ||
            got[i].relocation != want[i].relocation) {
            std::printf("instruction %zu differs: %s %s [%s]\n", i, got[i].mnemonic.c_str(),
                        got[i].operands.c_str(), got[i].relocation.c_str());
            return 5;
        }
    }
    std::printf("ok %zu instructions\n", got.size());
    return 0;
}
