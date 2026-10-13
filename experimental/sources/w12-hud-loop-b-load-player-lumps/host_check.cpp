// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// Every callee is a stub that appends to a trace; the libc string calls are the host's own.
#include "load_player_lumps.cpp"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>

extern "C" {
alignas(8) unsigned char player[4 * 23984];
int nmPlayers;
signed char coreForceSmallCharacters;
char* racePrefix[10] = {const_cast<char*>("bm"), const_cast<char*>("bf"), const_cast<char*>("wem"), const_cast<char*>("wef"),
                        const_cast<char*>("hem"), const_cast<char*>("hef"), const_cast<char*>("em"), const_cast<char*>("ef"),
                        const_cast<char*>("dem"), const_cast<char*>("def")};
}
static std::string trace;
static std::string present;      // names for which lumpQuery answers nonzero, comma separated with a trailing comma
static std::string missing;      // partial-load texts that return zero, comma separated with a trailing comma
static int checkAnswer, modeAnswer;
extern "C" {
void* lumpQuery__FPc(const char* n) { trace += std::string("Q:") + n + ";"; return present.find(std::string(n) + ",") != std::string::npos ? &trace : nullptr; }
void lumpClear__Fi(int id) { trace += "clear" + std::to_string(id) + ";"; }
void lumpLock__Fi(int id) { trace += "lock" + std::to_string(id) + ";"; }
void* lumpLoad__FPc(const char* n) { trace += std::string("load:") + n + ";"; return nullptr; }
void lumpRename__FPcT0(const char* a, const char* b) { trace += std::string("ren:") + a + ">" + b + ";"; }
int lumpPartialLoad__FPcT0(const char* l, const char* n) {
    trace += std::string("P:") + l + ":" + n + ";";
    return missing.find(std::string(n) + ",") == std::string::npos;
}
int playerCheckArmorCacheRaces__Fv() { trace += "check;"; return checkAnswer; }
void playerDeallocateArmorCaches__Fv() { trace += "dealloc;"; }
void playerInitArmorCache__Fv() { trace += "init;"; }
void playerAllocateArmorCache__Fii(int i, int r) { trace += "alloc" + std::to_string(i) + "," + std::to_string(r) + ";"; }
int MC_GetAsyncMode__Fv() { trace += "mode;"; return modeAnswer; }
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
static void setPlayer(int k, int cls, int hair, int colour, int beard, int body) {
    std::memset(player + k * 23984, 0, 23984);
    *reinterpret_cast<int*>(player + k * 23984 + 84) = cls;
    signed char* r = reinterpret_cast<signed char*>(player + k * 23984);
    r[32] = static_cast<signed char>(hair); r[33] = static_cast<signed char>(colour);
    r[34] = static_cast<signed char>(beard); r[35] = static_cast<signed char>(body);
}
static const char* const kQueries = "Q:ragnar2.lmp;Q:sonya2.lmp;Q:elf_mwo2.lmp;Q:elf_fwo2.lmp;Q:hem_high.lmp;Q:elf_fhi2.lmp;Q:eru_male.lmp;Q:eru_fem.lmp;Q:elf_mdk.lmp;Q:elf_fdk2.lmp;";
static int run() { trace.clear(); return loadPlayerLumps__Fv(); }

int main() {
    // One player of class 0 whose lump is the only one present, races check passes: nothing to do.
    setPlayer(0, 0, -1, 0, -1, 0); nmPlayers = 1; coreForceSmallCharacters = 0;
    present = "ragnar2.lmp,"; missing = ""; checkAnswer = 1; modeAnswer = 5;
    expect("unchanged result", std::to_string(run()), "0");
    expect("unchanged trace", trace, std::string(kQueries) + "check;mode;");
    // Same, but async mode 7 forces the reload: select.lmp, no hair/beard (-1), body texture 1 + 1.
    modeAnswer = 7;
    expect("forced result", std::to_string(run()), "1");
    expect("forced trace", trace,
           std::string(kQueries) + "check;mode;clear5;dealloc;init;load:ragnar2.lmp;ren:ragnar2.lmp>ragnar2.lmp;"
           "P:select.lmp:bm_bodyv1.vif;P:select.lmp:bm_bodyv1.tex;alloc0,0;lock5;");
    // A different lump set (class 0 missing) reloads without asking for the mode.
    present = ""; modeAnswer = 5;
    expect("changed result", std::to_string(run()), "1");
    // A failing races check reloads too (and still skips nothing before it).
    present = "ragnar2.lmp,"; checkAnswer = 0;
    expect("check zero result", std::to_string(run()), "1");
    expect("check zero trace has no mode", trace.find("mode;") == std::string::npos ? "ok" : "mode", "ok");
    // Two players of different classes: lumps for both present, select2.lmp, hair and beard attempted.
    checkAnswer = 1; present = "sonya2.lmp,elf_fwo2.lmp,";
    setPlayer(0, 1, 2, 3, 0, 4); setPlayer(1, 3, -1, 1, 1, 0); nmPlayers = 2;
    expect("two result", std::to_string(run()), "0");
    modeAnswer = 7; run();
    expect("two trace", trace,
           std::string(kQueries) + "check;mode;clear5;dealloc;init;load:sonya2.lmp;ren:sonya2.lmp>sonya2.lmp;"
           "P:select2.lmp:bf_hair3a.vif;P:select2.lmp:bf_hair3d.tex;P:select2.lmp:bf_beard1a.vif;P:select2.lmp:bf_beard1d.tex;"
           "P:select2.lmp:bf_bodyv1.vif;P:select2.lmp:bf_bodyv5.tex;alloc0,1;"
           "load:elf_fwo2.lmp;ren:elf_fwo2.lmp>elf_fwo2.lmp;"
           "P:select2.lmp:wef_beard2a.vif;P:select2.lmp:wef_beard2b.tex;P:select2.lmp:wef_bodyv1.vif;P:select2.lmp:wef_bodyv1.tex;alloc1,3;lock5;");
    // Hair vif missing: both loads are still issued, then the choice is cleared. Beard texture missing: cleared.
    missing = "bf_hair3a.vif,bf_beard1d.tex,"; setPlayer(0, 1, 2, 3, 0, 4); nmPlayers = 1;
    run();
    expect("hair cleared", std::to_string(reinterpret_cast<signed char*>(player)[32]), "-1");
    expect("beard cleared", std::to_string(reinterpret_cast<signed char*>(player)[34]), "-1");
    expect("colour kept", std::to_string(reinterpret_cast<signed char*>(player)[33]), "3");
    expect("both loads issued", trace.find("bf_hair3d.tex") != std::string::npos ? "ok" : "missing", "ok");
    // Only the texture missing for hair: also cleared; both present: kept.
    missing = "bf_hair3d.tex,"; setPlayer(0, 1, 2, 3, 0, 4); run();
    expect("hair tex cleared", std::to_string(reinterpret_cast<signed char*>(player)[32]), "-1");
    expect("beard kept", std::to_string(reinterpret_cast<signed char*>(player)[34]), "0");
    // Three players use the low names; one player with the small-characters flag does too and uses select2.
    missing = ""; present = ""; coreForceSmallCharacters = 1;
    setPlayer(0, 8, -1, 0, -1, 0); nmPlayers = 1; run();
    expect("small flag", trace.substr(trace.find("load:")),
           "load:elf_mdkl.lmp;ren:elf_mdkl.lmp>elf_mdk.lmp;P:select2.lmp:dem_bodyv1.vif;P:select2.lmp:dem_bodyv1.tex;alloc0,8;lock5;");
    coreForceSmallCharacters = 0;
    setPlayer(0, 9, -1, 0, -1, 0); setPlayer(1, 0, -1, 0, -1, 0); setPlayer(2, 1, -1, 0, -1, 0); nmPlayers = 3; run();
    expect("three players", trace.substr(trace.find("load:")),
           "load:elf_fdkl.lmp;ren:elf_fdkl.lmp>elf_fdk2.lmp;P:select2.lmp:def_bodyv1.vif;P:select2.lmp:def_bodyv1.tex;alloc0,9;"
           "load:ragnar2l.lmp;ren:ragnar2l.lmp>ragnar2.lmp;P:select2.lmp:bm_bodyv1.vif;P:select2.lmp:bm_bodyv1.tex;alloc1,0;"
           "load:sonya2l.lmp;ren:sonya2l.lmp>sonya2.lmp;P:select2.lmp:bf_bodyv1.vif;P:select2.lmp:bf_bodyv1.tex;alloc2,1;lock5;");
    std::printf(bad ? "FAIL\n" : "ok: load player lumps\n");
    return bad;
}
