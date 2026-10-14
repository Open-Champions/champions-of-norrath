// Host contract for _$_6Player; not an EE input. Includes the source. Callees record their arguments and
// which table word is stored at object+72 when they run; the player table is a plain host array.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_6Player[8];
signed char netPrediction;
char thereIsACutPlayer;
unsigned char player[96000];
static unsigned char arena[2048] __attribute__((aligned(16)));
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static std::string tab() { unsigned w; std::memcpy(&w, arena + 72, 4); return w == lo(vt_6Player) ? "6Player" : "other"; }
static std::string at(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void enumerateAlivePlayers__Fv() { calls += "enum:" + tab() + ";"; }
void listDestroy__FP8ListHead(void* h) { calls += "list" + at(h) + ";"; }
void animation_state_destructor(void* s, int f) __asm__("_$_14AnimationState");
void animation_state_destructor(void* s, int f) { calls += "anim" + at(s) + "," + std::to_string(f) + ";"; }
void light_destructor(void* l, int f) __asm__("_$_5Light");
void light_destructor(void* l, int f) { calls += "light" + at(l) + "," + std::to_string(f) + ";"; }
void creature_destructor(void* s, int f) __asm__("_$_8Creature");
void creature_destructor(void* s, int f) { calls += "base" + at(s) + "," + std::to_string(f) + ";"; }
void blockFree__FPv(void* b) { calls += "free" + at(b) + ";"; }
}

int main() {
    for (int index = 0; index < 4; ++index)
      for (int prediction : {0, 1, -1})
        for (int cut : {0, 1, 0x80})
          for (int flags : {0, 1, 2, 3, -1, -2}) {
            std::memset(arena, 0xa5, sizeof arena);
            std::memset(player, 0x5a, sizeof player);
            std::memcpy(arena + 960, &index, 4);
            arena[1106] = static_cast<unsigned char>(cut);
            netPrediction = static_cast<signed char>(prediction);
            thereIsACutPlayer = 0x77;
            calls.clear();
            destructor_Player(arena, flags);
            std::string want = "enum:6Player;list1396;anim1356,2;light1268,2;light1208,2;list1192;list1072;list1048;list1024;list1000;list976;base0,0;";
            if (flags & 1) want += "free0;";
            if (calls != want) { std::printf("fail: calls\n  want %s\n  got  %s\n", want.c_str(), calls.c_str()); return 1; }
            unsigned w; std::memcpy(&w, arena + 72, 4);
            if (w != lo(vt_6Player)) { std::puts("fail: table word"); return 2; }
            // expected player-table writes
            static unsigned char want_player[96000];
            std::memset(want_player, 0x5a, sizeof want_player);
            if (prediction == 0) {
                if (cut != 0) { /* cut flag set: only the global is cleared */ }
                else std::memset(want_player + 21872 + index * 23984, 0, 4);
                std::memset(want_player + 21876 + index * 23984, 0, 20);
            }
            if (std::memcmp(player, want_player, sizeof player) != 0) { std::puts("fail: player table"); return 3; }
            char wantCut = (prediction == 0 && cut != 0) ? 0 : 0x77;
            if (thereIsACutPlayer != wantCut) { std::puts("fail: thereIsACutPlayer"); return 4; }
          }
    return 0;
}
