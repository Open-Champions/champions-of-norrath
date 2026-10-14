// Host contract for _$_7Vehicle. Not an EE plan input.
#include "destructor.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int order[8];
static int steps = 0;
static void* list_arg;
static void* anim_arg;
static int anim_flags = 99, game_flags = 99;
static void* game_arg;
static void* free_arg;
static unsigned vt_at_list, vt_at_anim, vt_at_game;
static int count_at_list = -99;
static unsigned char* current;
unsigned char _vt$7Vehicle[248];
unsigned char* vehicleList[32];
int vehicleCount;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static unsigned vt_of(void* o) { unsigned v; std::memcpy(&v, static_cast<unsigned char*>(o) + 72, 4); return v; }

extern "C" void listDestroy__FP8ListHead(void* list) { order[steps++] = 1; list_arg = list; vt_at_list = vt_of(current); count_at_list = vehicleCount; }
extern "C" void _$_14AnimationState(void* state, int flags) { order[steps++] = 2; anim_arg = state; anim_flags = flags; vt_at_anim = vt_of(current); }
extern "C" void _$_10GameObject(void* object, int flags) { order[steps++] = 3; game_arg = object; game_flags = flags; vt_at_game = vt_of(object); }
extern "C" void blockFree__FPv(void* object) { order[steps++] = 4; free_arg = object; }

static int word(const unsigned char* p, int off) { int v; std::memcpy(&v, p + off, 4); return v; }
static void set_word(unsigned char* p, int off, int v) { std::memcpy(p + off, &v, 4); }

int main() {
    static unsigned char vehicles[6][2304];
    for (int count = 1; count <= 6; ++count) {
        for (int slot = 0; slot < count; ++slot) {
            for (int flags = 0; flags < 4; ++flags) {
                std::memset(vehicles, 0x11, sizeof vehicles);
                std::memset(vehicleList, 0, sizeof vehicleList);
                for (int i = 0; i < count; ++i) {
                    vehicleList[i] = vehicles[i];
                    set_word(vehicles[i], 2156, i);
                }
                vehicleCount = count;
                steps = 0;
                current = vehicles[slot];
                _$_7Vehicle(vehicles[slot], flags);
                expect(vehicleCount == count - 1, "count decreases by one");
                for (int i = 0; i < slot; ++i) expect(vehicleList[i] == vehicles[i] && word(vehicles[i], 2156) == i, "earlier entries untouched");
                for (int i = slot; i < count - 1; ++i) {
                    expect(vehicleList[i] == vehicles[i + 1], "later entries shift down");
                    expect(word(vehicles[i + 1], 2156) == i, "moved entry's index decreases by one");
                }
                if (slot < count - 1) expect(word(vehicles[slot], 2156) == slot, "the destroyed vehicle's own index word is not changed");
                unsigned vt = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(_vt$7Vehicle));
                expect(vt_of(vehicles[slot]) == vt && vt_at_list == vt && vt_at_anim == vt && vt_at_game == vt, "vtable stored before the sub-destructors");
                expect(count_at_list == count - 1, "count already decreased when the list is destroyed");
                expect(list_arg == vehicles[slot] + 2136 && anim_arg == vehicles[slot] + 120 && game_arg == vehicles[slot], "sub-object addresses");
                expect(anim_flags == 0 && game_flags == 0, "sub-destructors get flags 0");
                expect(order[0] == 1 && order[1] == 2 && order[2] == 3, "list, animation, game object order");
                if (flags & 1) expect(steps == 4 && order[3] == 4 && free_arg == vehicles[slot], "freed last when bit 0 set");
                else expect(steps == 3, "not freed when bit 0 clear");
            }
        }
    }
    return failures ? 1 : 0;
}
