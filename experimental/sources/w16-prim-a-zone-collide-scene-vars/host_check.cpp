#include "collide_scene_vars.cpp"
#include <initializer_list>
#include <string.h>

static int pre_result, inner_result, pre_calls, inner_calls, got_count, got_node;
static unsigned got_collide, got_trans;
static unsigned char *got_zone, *got_vars;
extern "C" int SetPreTranslations__9VICollideiP7VIVect3(unsigned c, int n, unsigned t) {
    ++pre_calls; got_collide = c; got_count = n; got_trans = t; return pre_result;
}
extern "C" int Collide__6VIZoneiR18VISceneCollRayVars(unsigned char* z, int node, unsigned char* v) {
    ++inner_calls; got_zone = z; got_node = node; got_vars = v; return inner_result;
}

int main() {
    for (int count : {-3, 0, 2}) {
        for (int pre : {-1, 0, 4}) {
            for (int tree : {0, 5}) {
                for (int inner : {0, 1, -4}) {
                    alignas(16) unsigned char zone[128], vars[64];
                    memset(zone, 0x10, sizeof zone);
                    memset(vars, 0x20, sizeof vars);
                    unsigned collide = 0x1234, trans = 0x5678;
                    memcpy(vars + 16, &collide, 4);
                    memcpy(zone + 84, &count, 4);
                    memcpy(zone + 88, &trans, 4);
                    memcpy(zone + 68, &tree, 4);
                    pre_result = pre; inner_result = inner;
                    pre_calls = inner_calls = 0;
                    int r = Collide__6VIZoneR18VISceneCollRayVars(zone, vars);
                    if (count > 0 && (pre_calls != 1 || got_collide != collide || got_count != count || got_trans != trans)) return 1;
                    if (count <= 0 && pre_calls != 0) return 2;
                    if (count > 0 && pre < 0) {
                        if (r != -1 || inner_calls != 0) return 3;
                        continue;
                    }
                    if (inner_calls != 1 || got_zone != zone || got_vars != vars) return 4;
                    if (got_node != (tree ? 0 : 0x40000000)) return 5;
                    if (r != (inner != 0 ? 1 : 0)) return 6;
                }
            }
        }
    }
    return 0;
}
