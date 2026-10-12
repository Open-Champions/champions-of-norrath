#define sprintf norrath_host_sprintf
#define strcpy norrath_host_strcpy
#define strcat norrath_host_strcat
#define strlen norrath_host_strlen
#include "set_srs_info.cpp"
#undef sprintf
#undef strcpy
#undef strcat
#undef strlen

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

unsigned char netGameLaunchInfo[328];
char playerSelectInfo[164];
const char* norrath_sce_d_race_strings_view_4293e8[10] = {"Elf", "Dwarf", "Human", "Gnome", "Troll", "Orc", "Ogre", "Halfling", "Gargoyle", "Imp"};
int norrath_sce_d_unknown_sdata_word_view_4852d8;

static unsigned char temp_player[23984], ram_player[23984];
static int info_first = 1, info_second = 1;
static const char* info_text = "";
static int order_log[8], order_n;
static int copy_count;
static void* last_copy_source;

extern "C" {
void* MC_GetTempPlayerStorage__Fi(int slot) { order_log[order_n++] = 1; if (slot != 0) order_log[order_n++] = 99; return temp_player; }
void* MC_GetRamPlayerPtr__Fi(int slot) { order_log[order_n++] = 2; if (slot != 0) order_log[order_n++] = 99; return ram_player; }
void MC_ByteCopy__FPvT0i(void* d, void* s, int n) { order_log[order_n++] = 3; copy_count = n; last_copy_source = s; std::memcpy(d, s, static_cast<size_t>(n)); }
void MC_GetNetGameInfo__FPiT0Pc(int* a, int* b, char* t) { order_log[order_n++] = 4; *a = info_first; *b = info_second; std::strcpy(t, info_text); }
int norrath_host_sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    int r = std::vsnprintf(d, 128, f, ap);
    va_end(ap);
    return r;
}
char* norrath_host_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
char* norrath_host_strcat(char* d, const char* s) { return std::strcat(d, s); }
unsigned norrath_host_strlen(const char* s) { return static_cast<unsigned>(std::strlen(s)); }
}

static std::string at(int offset) { return std::string(reinterpret_cast<char*>(netGameLaunchInfo) + offset); }
static void fill_player(unsigned char* p, const char* name, int race, unsigned char flag) {
    std::memset(p, 0, 23984);
    std::strcpy(reinterpret_cast<char*>(p), name);
    std::memcpy(p + 84, &race, 4);
    p[89] = flag;
}
static void reset(unsigned char flags) {
    std::memset(netGameLaunchInfo, 0x7e, sizeof netGameLaunchInfo);
    netGameLaunchInfo[0] = flags;
    order_n = 0; copy_count = 0; last_copy_source = nullptr;
    info_first = 1; info_second = 1; info_text = "Hall of Mirrors";
    fill_player(temp_player, "Tempname", 3, 0x81);
    fill_player(ram_player, "Ramname", 4, 0xc3);
    norrath_sce_d_unknown_sdata_word_view_4852d8 = 1;
}

int main() {
    /* neither flag: ram player, net game info text, level 1 (no prefix) */
    reset(0);
    setSRSInfo__Fv();
    if (at(7) != "Hall of Mirrors" || at(135) != "Ramname" || at(199) != "Troll") return 1;
    if (netGameLaunchInfo[295] != 0xc3) return 2; /* byte copied unsigned, not sign-extended */
    if (copy_count != 23984 || last_copy_source != ram_player) return 3;
    if (order_n != 3 || order_log[0] != 2 || order_log[1] != 3 || order_log[2] != 4) return 4;
    /* level 2 prefix */
    reset(0); info_second = 2;
    setSRSInfo__Fv();
    if (at(7) != "(Courageous)Hall of Mi...") return 5;
    /* level 3 prefix and 22-character cut with ellipsis */
    reset(0); info_second = 3; info_text = "Hall of Mirrors";
    setSRSInfo__Fv();
    if (at(7) != "(Champion)Hall of Mirr...") return 6;
    /* level 2 */
    reset(0); info_second = 2; info_text = "A";
    setSRSInfo__Fv();
    if (at(7) != "(Courageous)A") return 7;
    /* a name of exactly 22 characters is kept; 23 is cut to 22 plus ... */
    reset(0); info_text = "0123456789012345678901";
    setSRSInfo__Fv();
    if (at(7) != "0123456789012345678901") return 8;
    reset(0); info_text = "01234567890123456789012";
    setSRSInfo__Fv();
    if (at(7) != "0123456789012345678901...") return 9;
    /* owner name: 9 characters kept, 10 cut to 9 plus ... */
    reset(0); fill_player(ram_player, "123456789", 0, 1);
    setSRSInfo__Fv();
    if (at(135) != "123456789") return 10;
    reset(0); fill_player(ram_player, "1234567890", 0, 1);
    setSRSInfo__Fv();
    if (at(135) != "123456789...") return 11;
    /* flag 4: temp storage, second int from the unknown word, no net game info call, offset 7 stays "Kelethin" */
    reset(4); norrath_sce_d_unknown_sdata_word_view_4852d8 = 2;
    setSRSInfo__Fv();
    if (at(7) != "(Courageous)Kelethin") return 12;
    if (at(135) != "Tempname" || at(199) != "Gnome" || netGameLaunchInfo[295] != 0x81) return 13;
    if (order_n != 2 || order_log[0] != 1 || order_log[1] != 3 || last_copy_source != temp_player) return 14;
    reset(4);
    setSRSInfo__Fv();
    if (at(7) != "Kelethin") return 15;
    reset(4); norrath_sce_d_unknown_sdata_word_view_4852d8 = 7;
    setSRSInfo__Fv();
    if (at(7).compare(0, 10, "(Champion)") != 0) return 16;
    /* flag 2 (takes precedence over flag 4): select info, no player copy, byte 295 is 1 */
    reset(6);
    std::strcpy(playerSelectInfo, "Selected");
    playerSelectInfo[32] = 5;
    setSRSInfo__Fv();
    if (at(7) != "Kelethin" || at(135) != "Selected" || at(199) != "Orc" || netGameLaunchInfo[295] != 1) return 17;
    if (order_n != 0) return 18;
    /* the index byte is signed */
    reset(2);
    playerSelectInfo[32] = 0;
    setSRSInfo__Fv();
    if (at(199) != "Elf") return 19;
    /* flags are taken from byte 0 only; unrelated bits do nothing */
    reset(1);
    setSRSInfo__Fv();
    if (order_log[0] != 2) return 20;
    std::puts("set srs info contract passed");
    return 0;
}
