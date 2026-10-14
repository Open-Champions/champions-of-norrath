#include "collide_env_ray.cpp"
#include <initializer_list>
#include <string.h>

static int calls, got_node, inner_result;
static void *gz, *gr, *g1, *g2;
extern "C" int Collide__6VIZoneiR8VIEnvRayR7VIVect3T3(unsigned char* z, int node, void* r, void* a, void* b) {
    ++calls; gz = z; got_node = node; gr = r; g1 = a; g2 = b; return inner_result;
}

int main() {
    for (int tree : {0, 9}) {
        for (int inner : {0, 1, -2}) {
            alignas(16) unsigned char zone[128], r[4], a[4], b[4];
            memset(zone, 0x10, sizeof zone);
            memcpy(zone + 68, &tree, 4);
            inner_result = inner;
            calls = 0;
            int ret = Collide__6VIZoneR8VIEnvRayR7VIVect3T2(zone, r, a, b);
            if (calls != 1 || gz != zone || gr != r || g1 != a || g2 != b) return 1;
            if (got_node != (tree ? 0 : 0x40000000)) return 2;
            if (ret != (inner ? 1 : 0)) return 3;
        }
    }
    return 0;
}
