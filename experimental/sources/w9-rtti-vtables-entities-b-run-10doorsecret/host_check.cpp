#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "run.cpp"

int frameNm = 0;
int world = 0;

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::vector<std::string> calls;
static Point3 move_point;
static int move_index, move_extra;
static void* move_world;
static int collision_index, collision_flag;
static void* collision_world;
static void* seq_object;
static float hermite_args[5];
static unsigned char* current_object;
static char sequence_marker;
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* w, int index, Point3 point, short extra) {
    calls.push_back("move");
    move_world = w;
    move_index = index;
    move_point = point;
    move_extra = extra;
    return 0;
}
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* w, int index, int enable) {
    calls.push_back("collision");
    collision_world = w;
    collision_index = index;
    collision_flag = enable;
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) {
    calls.push_back(object == current_object ? "remove" : "remove-wrong-object");
}
extern "C" char* lumpFindResource__FPcT0(const char* lump, const char* name) {
    calls.push_back(std::string("lump:") + lump + ":" + name);
    return &sequence_marker;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate) {
    calls.push_back(header == &sequence_marker && rate == 1.0f ? "seq" : "seq-wrong");
    seq_object = object;
    return 0;
}
extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    calls.push_back("hermite");
    hermite_args[0] = t; hermite_args[1] = a; hermite_args[2] = b; hermite_args[3] = c; hermite_args[4] = d;
    return t * 0.5f + 0.125f;
}

int main() {
    alignas(8) unsigned char object[128];
    for (int elapsed : {0, 5, 19, 20, 21, 100, 119, 120, 121, 130, 150, 179, 180, 181, 200, 299, 300, 301, 400, -5}) {
        for (short start : {short(7), short(-3)}) {
            std::memset(object, 0x22, sizeof object);
            const short door = -9, off82 = 40;
            std::memcpy(object + 76, &door, 2);
            std::memcpy(object + 80, &start, 2);
            std::memcpy(object + 82, &off82, 2);
            const int opened = 1000;
            std::memcpy(object + 84, &opened, 4);
            frameNm = opened + elapsed;
            world = 4660;
            current_object = object;
            calls.clear();
            msg_run__10DoorSecret(object);

            short got80, got82;
            std::memcpy(&got80, object + 80, 2);
            std::memcpy(&got82, object + 82, 2);
            const short want80 = elapsed < 20 ? static_cast<short>(start + 1) : start;
            short want82 = off82;
            std::vector<std::string> want;
            if (elapsed == 120) {
                want.push_back("lump:fx.lmp:door_stnslid_open.anm");
                want.push_back("seq");
            }
            if (elapsed > 120) {
                want.push_back("hermite");
                float s = static_cast<float>(elapsed - 120) * (1.0f / 60.0f);
                if (s > 1.0f) s = 1.0f;
                want82 = static_cast<short>(static_cast<int>((s * 0.5f + 0.125f) * 400.0f));
            }
            if (elapsed > 180) want.push_back("collision");
            if (elapsed > 300) want.push_back("remove");
            want.push_back("move");
            expect(calls == want, "call order");
            expect(got80 == want80, "halfword +80");
            expect(got82 == want82, "halfword +82");
            if (elapsed > 120) {
                float s = static_cast<float>(elapsed - 120) * (1.0f / 60.0f);
                if (s > 1.0f) s = 1.0f;
                float d = hermite_args[0] - s;
                expect((d < 0 ? -d : d) < 1e-4f, "progress argument");
                expect(hermite_args[1] == 0.0f && hermite_args[2] == 1.0f && hermite_args[3] == 0.0f && hermite_args[4] == 1.5f, "hermite constants");
            }
            if (elapsed > 180) expect(collision_world == reinterpret_cast<void*>(4660) && collision_index == door && collision_flag == 0, "collision call");
            expect(move_world == reinterpret_cast<void*>(4660) && move_index == door && move_extra == 0, "move call");
            expect(move_point.x == static_cast<float>(-want80) * 0.25f && move_point.y == static_cast<float>(want82) * 0.25f && move_point.z == 0.0f, "move point");
            expect(object[78] == 0x22 && object[79] == 0x22 && object[88] == 0x22, "neighbours untouched");
        }
    }
    return failures ? 1 : 0;
}
