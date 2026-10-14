#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_address(int value) { return arena + value; }
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }

#include "raster_actor.cpp"

static int log[16], logCount;
static int cullResult, vcallResult, addResult;
static const void* cullSphere; static void* cullStack;
static void* mulA; static const void* mulB; static const void* mulC;
static void* mvRaster; static const void* mvMatrix;
static void* slRaster; static const void* slColor; static unsigned char slScene1092[4], slScene1096[4];
static unsigned char* sceneForHooks;
static void* addVector; static int addFirst, addSecond; static void* addActor;
extern "C" int CullByTop__11VIClipStackRC8VISphere(void* stack, const void* sphere) { log[logCount++] = 1; cullStack = stack; cullSphere = sphere; return cullResult; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* a, const void* b, const void* c) { log[logCount++] = 2; mulA = a; mulB = b; mulC = c; }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* r, const void* m) { log[logCount++] = 3; mvRaster = r; mvMatrix = m; }
extern "C" void SetStaticLighting__8VIRasterRC10VIColor32F(void* r, const void* c) {
    log[logCount++] = 4; slRaster = r; slColor = c;
    memcpy(slScene1092, sceneForHooks + 1092, 4); memcpy(slScene1096, sceneForHooks + 1096, 4);
}
static void* vSelf; static const void* vCamera; static void* vScene; static void* vRaster; static int vFlag;
static int fakeMethod(void* self, const void* camera, void* scene, void* raster, int flag) {
    log[logCount++] = 5; vSelf = self; vCamera = camera; vScene = scene; vRaster = raster; vFlag = flag; return vcallResult;
}
extern "C" int Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(void* vector, const VISceneDisplayElem* e) {
    log[logCount++] = 6; addVector = vector; addFirst = e->first; addActor = e->actor; addSecond = e->second; return addResult;
}

static void w32(unsigned char* p, int v) { memcpy(p, &v, 4); }
static void wf(unsigned char* p, float v) { memcpy(p, &v, 4); }
static int r32(const unsigned char* p) { int v; memcpy(&v, p, 4); return v; }

int main() {
    alignas(16) static unsigned char scene[20000], camera[128], clip[16];
    sceneForHooks = scene;
    unsigned char* raster = arena + 0x100;
    unsigned char* records = arena + 0x200;
    unsigned char* object = arena + 0x400;
    unsigned char* table = arena + 0x600;
    int actorAddr = 0x1000;
    unsigned char* actor = arena + actorAddr;
    struct Case { int stamp; int flags; float dist; int kind; int cull; int count; float near_; int vres; int addres; } cases[] = {
        {0, 0, 1.0f, 3, 0, 0, 100.0f, 1, 0},     // normal draw path, queued as display element
        {7, 0, 1.0f, 3, 0, 0, 100.0f, 1, 0},     // stamp equal: nothing
        {0, 1, 1.0f, 3, 0, 0, 100.0f, 1, 0},     // flag bit 0: nothing
        {0, 4, 1.0f, 3, 0, 0, 100.0f, 1, 0},     // flag bit 2: nothing
        {0, 2, 50.0f, 3, 0, 0, 100.0f, 1, 0},    // beyond range: nothing
        {0, 0, 1.0f, 3, 1, 0, 100.0f, 1, 0},     // culled by clip stack
        {0, 0, 1.0f, 5, 0, 0, 100.0f, 1, 0},     // candidate list append
        {0, 0, 1.0f, 5, 0, 0, 0.5f, 1, 0},       // candidate too far for list
        {0, 0, 1.0f, 5, 0, 128, 100.0f, 1, 0},   // list full
        {0, 0, 1.0f, 5, 0, 127, 100.0f, 0, 0},   // last slot; method returns 0
        {0, 0, 1.0f, 3, 0, 0, 100.0f, 1, -1},    // Add fails
    };
    for (const Case& c : cases) {
        memset(arena, 0, sizeof arena); memset(scene, 0x13, sizeof scene);
        memset(camera, 0, sizeof camera);
        wf(camera + 28, 1.0f); wf(camera + 32, 2.0f); wf(camera + 36, 3.0f);
        w32(scene + 1348, 7);
        norrath_host_store_pointer(scene + 1004, records);
        norrath_host_store_pointer(scene + 19356, raster);
        wf(scene + 6268, 25.0f);
        w32(scene + 6792, c.count);
        w32(actor + 128, c.stamp); w32(actor + 64, c.flags);
        wf(actor + 68, 1.0f + c.dist); wf(actor + 72, 2.0f); wf(actor + 76, 3.0f);
        w32(actor + 96, 2); w32(actor + 108, 0x4455);
        norrath_host_store_pointer(records + 24, object);
        wf(object + 48, 10.0f); w32(object + 52, c.kind);
        norrath_host_store_pointer(object + 60, table);
        short adj = 12; memcpy(table + 24, &adj, 2);
        int (*fn)(void*, const void*, void*, void*, int) = fakeMethod; memcpy(table + 28, &fn, sizeof fn);
        cullResult = c.cull; vcallResult = c.vres; addResult = c.addres; logCount = 0;
        unsigned char before[20000]; memcpy(before, scene, sizeof scene);
        int ret = RasterActor__7VISceneR8VICameraiR11VIClipStack(scene, camera, actorAddr, clip);
        bool stop = c.stamp == 7 || (c.flags & 5) || c.dist * c.dist >= 100.0f || c.cull;
        if (stop) {
            if (ret != 0 || memcmp(before, scene, sizeof scene) != 0) return 1;
            if ((c.stamp == 7 || (c.flags & 5)) && logCount != 0) return 2;
            if (c.cull && (logCount != 1 || log[0] != 1 || cullSphere != actor + 136 || cullStack != clip)) return 3;
            if (!c.cull && c.dist * c.dist >= 100.0f && logCount != 0) return 4;
            continue;
        }
        if (r32(actor + 128) != 7) return 5;
        if (mulA != scene + 1232 || mulB != actor || mulC != camera + 48 || mvRaster != raster || mvMatrix != scene + 1232) return 6;
        bool listed = c.kind == 5 && c.dist * c.dist < 25.0f && c.count < 128;
        if (c.kind == 5 && c.dist * c.dist < 25.0f) {
            if (c.count < 128) {
                if (r32(scene + 6792) != c.count + 1 || norrath_host_load_pointer(scene + 6280 + 4 * c.count) != actor) return 7;
            } else if (r32(scene + 6792) != 128) return 8;
        } else if (r32(scene + 6792) != c.count) return 9;
        (void)listed;
        if (slRaster != raster || slColor != actor + 112 || r32(slScene1092) != 0x4455 || r32(slScene1096) != -1) return 10;
        if (vSelf != object + 12 || vCamera != camera || vScene != scene || vRaster != raster || vFlag != 1) return 11;
        if (c.vres == 0) {
            if (ret != 0 || logCount != 5 || r32(scene + 1092) != -1 || r32(scene + 1096) != -1) return 12;
        } else {
            if (logCount != 6 || addVector != scene + 1112 || addFirst != -1 || addActor != actor || addSecond != 2) return 13;
            if (c.addres < 0) { if (ret != -1 || r32(scene + 1092) != 0x4455 || r32(scene + 1096) != -1) return 14; }
            else if (ret != 0 || r32(scene + 1092) != -1 || r32(scene + 1096) != -1) return 15;
        }
    }
    return 0;
}
