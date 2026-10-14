// Host check only; not a build input. Compiled with NORRATH_HOST_ARENA so the object layout accounts for the
// eight-byte host vtable pointer (the player-state word still lands at +960). Checks the null object, the
// type-function call through the list slot, the four player states plus an out-of-range value, the enemy
// colour selection by flag bit and visibility switch, and the position/short copy on every non-early path.
#define NORRATH_HOST_ARENA 1
#include "draw_character_call_back.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static MapObject* list[8];
MapObject** mapObjList = list;
int gameShowEnemiesInMap;
static MapObject objects[8];
static unsigned char vtable_bytes[16];
static int player_token_object, other_token_object;
static void* player_token = &player_token_object;
static void* other_token = &other_token_object;
static std::string trace;
static bool object_is_player;

static void* type_of(void* slot) {
    trace += "T" + std::to_string(static_cast<MapObject**>(slot) - list) + ";";
    return object_is_player ? player_token : other_token;
}
extern "C" void* __tf6Player() { trace += "P;"; return player_token; }
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right) { trace += "E;"; return left == right ? 1 : 0; }

int main() {
    static_assert(sizeof(void*) == 8, "host layout assumption");
    typedef void* (*TypeFunction)(void*);
    const TypeFunction function = type_of;
    std::memset(vtable_bytes, 0, sizeof vtable_bytes);
    std::memcpy(vtable_bytes + 4, &function, sizeof function);
    bool ok = true;
    long scenarios = 0;
    for (int index : {0, 3, 7}) for (int null_object : {0, 1}) for (int player : {0, 1}) for (int state : {-1, 0, 1, 2, 3, 4, 100})
    for (int show : {0, 1, 5}) for (unsigned flags : {0u, 0x01000000u, 0xfeffffffu, 0xffffffffu, 0x00ffffffu}) {
        std::memset(objects, 0x6b, sizeof objects);
        for (int i = 0; i < 8; ++i) list[i] = &objects[i];
        MapObject& object = objects[index];
        object.vtable = vtable_bytes;
        object.flags = flags;
        object.position = Point3{1.5f, -2.25f, 3.125f};
        object.short_value = 0xfedc;
        object.player_state = state;
        if (null_object) list[index] = nullptr;
        object_is_player = player;
        gameShowEnemiesInMap = show;
        Point3 position = {9, 9, 9};
        short short_out = 0x1111;
        int first = -777, second = -778, third = -779;
        trace.clear();
        const int result = drawCharacterCallBack__FiP6Point3PsPiN23(index, &position, &short_out, &first, &second, &third);

        int want_result, want_first = -777, want_second = -778, want_third = -779;
        bool copied = false;
        std::string want_trace;
        if (null_object) want_result = -1;
        else {
            want_trace = "T" + std::to_string(index) + ";P;E;";
            if (player) {
                want_result = 1; copied = true;
                switch (state) {
                case 0: want_first = 182; want_second = 47; want_third = 58; break;
                case 1: want_first = 0; want_second = 191; want_third = 0; break;
                case 2: want_first = 180; want_second = 178; want_third = 0; break;
                case 3: want_first = 29; want_second = 32; want_third = 161; break;
                default: break;
                }
            } else if (show == 0) {
                want_result = 0;
            } else {
                want_result = 1; copied = true;
                if (flags & 0x01000000u) { want_first = 0; want_second = 64; } else { want_first = 64; want_second = 0; }
                want_third = 0;
            }
        }
        const bool position_ok = copied ? (position.x == 1.5f && position.y == -2.25f && position.z == 3.125f) : (position.x == 9 && position.y == 9 && position.z == 9);
        const bool short_ok = copied ? short_out == static_cast<short>(0xfedc) : short_out == 0x1111;
        if (result != want_result || trace != want_trace || first != want_first || second != want_second || third != want_third || !position_ok || !short_ok) {
            ok = false;
            std::printf("MISMATCH index=%d null=%d player=%d state=%d show=%d flags=%x got %d '%s' %d %d %d\n", index, null_object, player, state, show, flags, result, trace.c_str(), first, second, third);
            break;
        }
        ++scenarios;
    }
    std::puts(ok ? ("ok (" + std::to_string(scenarios) + " scenarios)").c_str() : "FAIL");
    return !ok;
}
