// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "replace_static_proxy_actors.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
alignas(16) static unsigned char arena[0x8000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static int find_fail_id, replace_fail_actor;
static std::vector<unsigned> find_ids;
struct Rep { int actor, value; };
static std::vector<Rep> reps;
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* value) {
    find_ids.push_back(id);
    if (dictionary != arena + 0x7000) find_ids.push_back(0xdead);
    *type = 99;
    *value = static_cast<int>(id) * 2;
    return static_cast<int>(id) != find_fail_id;
}
extern "C" int ReplaceProxyActor__7VISceneii(unsigned char* scene, int actor, int replacement) {
    if (scene != arena) reps.push_back({-1, -1});
    reps.push_back({actor, replacement});
    return actor == replace_fail_actor ? -3 : 0;
}

static void build() {
    std::memset(arena, 0, sizeof arena);
    find_ids.clear();
    reps.clear();
    unsigned* w = reinterpret_cast<unsigned*>(arena);
    w[19352 / 4] = 0x7000;
    unsigned* zone = reinterpret_cast<unsigned*>(arena + 0x5000);
    zone[92 / 4] = 2;
    zone[96 / 4] = 0x5400;
    unsigned* room0 = reinterpret_cast<unsigned*>(arena + 0x5400);
    unsigned* room1 = reinterpret_cast<unsigned*>(arena + 0x5400 + 120);
    room0[60 / 4] = 2;
    room0[64 / 4] = 0x5800;
    room1[60 / 4] = 3;
    room1[64 / 4] = 0x5840;
    unsigned* t0 = reinterpret_cast<unsigned*>(arena + 0x5800);
    unsigned* t1 = reinterpret_cast<unsigned*>(arena + 0x5840);
    t0[0] = 0x6000; t0[1] = 0x6100;
    t1[0] = 0x6200; t1[1] = 0x6300; t1[2] = 0x6400;
    const unsigned flags[5] = {4, 0, 4, 1, 6};
    for (unsigned i = 0; i < 5; ++i) {
        unsigned* actor = reinterpret_cast<unsigned*>(arena + 0x6000 + 0x100 * i);
        actor[64 / 4] = flags[i];
        actor[100 / 4] = 500 + i;
    }
}

int main() {
    bool ok = true;
    for (int variant = 0; variant < 3; ++variant) {
        build();
        find_fail_id = variant == 1 ? 502 : -1;
        replace_fail_actor = variant == 2 ? 0x6200 : -1;
        int r = ReplaceStaticProxyActors__7VISceneR6VIZone(arena, arena + 0x5000);
        std::vector<unsigned> want_ids = {500, 502, 504};
        if (variant == 1) want_ids = {500, 502};
        if (variant == 2) want_ids = {500, 502};
        if (find_ids != want_ids) ok = false;
        if (r != (variant == 0 ? 0 : -1)) ok = false;
        size_t want_reps = variant == 0 ? 3 : variant == 1 ? 1 : 2;
        if (reps.size() != want_reps) ok = false;
        else {
            const Rep all[3] = {{0x6000, 1000}, {0x6200, 1004}, {0x6400, 1008}};
            for (size_t i = 0; i < reps.size(); ++i)
                if (reps[i].actor != all[i].actor || reps[i].value != all[i].value) ok = false;
        }
    }
    build();
    reinterpret_cast<unsigned*>(arena + 0x5000)[92 / 4] = 0;
    if (ReplaceStaticProxyActors__7VISceneR6VIZone(arena, arena + 0x5000) != 0 || !find_ids.empty() || !reps.empty()) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
