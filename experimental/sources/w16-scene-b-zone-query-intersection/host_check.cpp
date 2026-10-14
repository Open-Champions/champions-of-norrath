// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "query_intersection.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static int spt_calls, spt_count, spt_result, qi_calls, qi_node, qi_result;
static void *spt_collide, *spt_translations;
static unsigned char *qi_zone, *qi_vars;
extern "C" int SetPreTranslations__9VICollideiP7VIVect3(void* collide, int count, void* translations) {
    ++spt_calls; spt_collide = collide; spt_count = count; spt_translations = translations;
    return spt_result;
}
extern "C" int QueryIntersection__6VIZoneiR20VISceneIntersectVars(unsigned char* zone, int node, unsigned char* vars) {
    ++qi_calls; qi_zone = zone; qi_node = node; qi_vars = vars;
    return qi_result;
}

int main() {
    bool ok = true;
    unsigned char* zone = arena + 0x100;
    unsigned char* vars = arena + 0x200;
    for (int count : {0, 2, -1})
    for (unsigned tree : {0u, 0x500u})
    for (int spt : {0, -1})
    for (int qi : {0, 4, -2}) {
        std::memset(arena, 0, sizeof arena);
        unsigned* z = reinterpret_cast<unsigned*>(zone);
        z[21] = static_cast<unsigned>(count);
        z[22] = 0x800;
        z[17] = tree;
        reinterpret_cast<unsigned*>(vars)[9] = 0x900;
        spt_calls = qi_calls = 0; spt_result = spt; qi_result = qi;
        int r = QueryIntersection__6VIZoneR20VISceneIntersectVars(zone, vars);
        bool set = count > 0;
        if (spt_calls != (set ? 1 : 0)) ok = false;
        if (set && (spt_collide != arena + 0x900 || spt_count != count || spt_translations != arena + 0x800)) ok = false;
        bool go = !(set && spt < 0);
        if (qi_calls != (go ? 1 : 0)) ok = false;
        if (go && (qi_zone != zone || qi_vars != vars || qi_node != (tree ? 0 : 0x40000000))) ok = false;
        int want = !go ? -1 : (qi < 0 ? -1 : 0);
        if (r != want) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
