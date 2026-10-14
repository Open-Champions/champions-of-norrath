// Host contract for msg_collisionWorld__6GoblinG6Point3; not an EE input.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
unsigned char* w10g_host_base;
void* w10g_host_fn(unsigned) { return nullptr; }
#include "msg_collision_world.cpp"
static int calls; static void *seenState, *seenHeader; static float fa, fb; static int seenFlags;
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    ++calls; seenState = s; seenHeader = h; fa = a; fb = b; seenFlags = f;
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
int main() {
    w10g_host_base = arena; int bad = 0;
    unsigned char* object = arena + 0x1000;
    auto setup = [&](short state, unsigned short flags) {
        std::memset(arena, 0, sizeof arena); calls = 0;
        std::memcpy(object + 572, &state, 2); std::memcpy(object + 294, &flags, 2);
        unsigned table = 0x100, header = 0x777; std::memcpy(object + 36, &table, 4); std::memcpy(arena + table + 172, &header, 4);
    };
    Point3 p{1, 2, 3};
    setup(8, 0); msg_collisionWorld__6GoblinG6Point3(object, p);
    unsigned bitsA, bitsB; std::memcpy(&bitsA, &fa, 4); std::memcpy(&bitsB, &fb, 4);
    if (calls != 1 || seenState != object + 288 || seenHeader != arena + 0x777 || seenFlags != 32) { std::puts("fail: call arguments"); bad = 1; }
    if (bitsA != 0x3dcccccc || bitsB != 0x3dcccccc) { std::puts("fail: float constant bits"); bad = 1; }
    setup(8, 0x20); msg_collisionWorld__6GoblinG6Point3(object, p); if (calls) { std::puts("fail: bit 5 blocks"); bad = 1; }
    setup(8, 0xffdf); msg_collisionWorld__6GoblinG6Point3(object, p); if (calls != 1) { std::puts("fail: other bits do not block"); bad = 1; }
    setup(7, 0); msg_collisionWorld__6GoblinG6Point3(object, p); if (calls) { std::puts("fail: state must be 8"); bad = 1; }
    setup(-8, 0); msg_collisionWorld__6GoblinG6Point3(object, p); if (calls) { std::puts("fail: state must be 8 (signed)"); bad = 1; }
    if (!bad) std::puts("goblin msg_collisionWorld ok");
    return bad;
}
