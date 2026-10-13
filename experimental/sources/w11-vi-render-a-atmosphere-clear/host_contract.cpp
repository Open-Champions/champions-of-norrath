#include "clear.cpp"
#include <string.h>

static int order[64]; static int order_n;
static void* stop_dev[4]; static int stop_id[4]; static int stop_n;
static int rel_mat[8]; static void* rel_raster[8]; static void* rel_dict[8]; static int rel_n;
static void* vdel[8]; static int vdel_n;
static void* emit[16]; static int emit_flags[16]; static int emit_n;
static int fireballs, splashes;
extern "C" void Stop__13VISoundDevicei(void* d, int id) { stop_dev[stop_n] = d; stop_id[stop_n++] = id; order[order_n++] = 1; }
extern "C" void ReleaseMaterial__8VIRasteriR12VIDictionary(void* r, int m, void* d) { rel_raster[rel_n] = r; rel_mat[rel_n] = m; rel_dict[rel_n++] = d; order[order_n++] = 2; }
extern "C" void ClearAllFireballs__12VIAtmosphere(void*) { ++fireballs; order[order_n++] = 3; }
extern "C" void ClearAllSplashes__12VIAtmosphere(void*) { ++splashes; order[order_n++] = 4; }
extern "C" void _$_17VIParticleEmitter(void* e, int f) { emit[emit_n] = e; emit_flags[emit_n++] = f; order[order_n++] = 5; }
extern "C" void __builtin_vec_delete(void* p) { vdel[vdel_n++] = p; order[order_n++] = 6; }

static unsigned int addr(void* p) { return static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static void put(unsigned char* b, int o, unsigned int v) { memcpy(b + o, &v, 4); }
static unsigned int get(unsigned char* b, int o) { unsigned int v; memcpy(&v, b + o, 4); return v; }
static void reset() { order_n = stop_n = rel_n = vdel_n = emit_n = fireballs = splashes = 0; }

int main() {
    static unsigned char object[4600], scene[19400], buf[8], heap[16 + 3 * 2048], plain[16];
    static int sound, raster, dict;
    put(scene, 19360, addr(&sound)); put(scene, 19356, addr(&raster)); put(scene, 19352, addr(&dict));
    put(heap, 0, 3);   // cookie lives at heap+0, the array starts at heap+16
    unsigned char* array = heap + 16;
    // fully active
    memset(object, 0, sizeof object);
    put(object, 1228, 1); put(object, 4496, 11); put(object, 4500, 12); put(object, 3920, 21); put(object, 1244, 22);
    put(object, 56, 23); put(object, 160, 24); put(object, 164, 25); put(object, 12, addr(plain)); put(object, 4476, addr(array));
    put(object, 48, addr(buf));
    reset();
    Clear__12VIAtmosphereR7VIScene(object, scene);
    if (stop_n != 2 || stop_dev[0] != &sound || stop_id[0] != 11 || stop_id[1] != 12) return 1;
    if (rel_n != 5) return 2;
    int mats[5] = {21, 22, 23, 24, 25};
    for (int i = 0; i < 5; ++i) if (rel_mat[i] != mats[i] || rel_raster[i] != &raster || rel_dict[i] != &dict) return 3;
    if (emit_n != 6) return 4;
    for (int e = 2; e >= 0; --e) {
        int k = (2 - e) * 2;
        if (emit[k] != array + e * 2048 + 1056 || emit[k + 1] != array + e * 2048 + 64 || emit_flags[k] != 2 || emit_flags[k + 1] != 2) return 5;
    }
    if (vdel_n != 3 || vdel[0] != plain || vdel[1] != heap || vdel[2] != buf) return 6;
    if (fireballs != 1 || splashes != 1) return 7;
    int expect[] = {1, 1, 2, 2, 2, 2, 2, 6, 3, 5, 5, 5, 5, 5, 5, 6, 4, 6};
    if (order_n != 18) return 8;
    for (int i = 0; i < 18; ++i) if (order[i] != expect[i]) return 10 + i;
    if (get(object, 4496) != 0xffffffffu || get(object, 4500) != 0xffffffffu) return 30;
    if (get(object, 1228) != 0 || get(object, 12) != 0 || get(object, 4476) != 0 || get(object, 48) != 0) return 31;

    // inactive: only the pointer clean-up runs
    memset(object, 0, sizeof object);
    put(object, 4496, 11); put(object, 12, addr(plain));
    reset();
    Clear__12VIAtmosphereR7VIScene(object, scene);
    if (stop_n != 0 || rel_n != 0 || emit_n != 0 || vdel_n != 1 || vdel[0] != plain) return 40;
    if (fireballs != 1 || splashes != 1 || get(object, 4496) != 11) return 41;

    // an empty array is only released
    memset(object, 0, sizeof object);
    put(heap, 0, 0); put(object, 4476, addr(array));
    reset();
    Clear__12VIAtmosphereR7VIScene(object, scene);
    if (emit_n != 0 || vdel_n != 1 || vdel[0] != heap || get(object, 4476) != 0) return 50;
    return 0;
}
