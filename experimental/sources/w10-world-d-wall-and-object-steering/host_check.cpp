// Host contract for wallAndObjectSteering__FP8CreatureffP10GameObject. Not an EE plan input.
// Scripted hitscan answers stand in for the world; icos/isin are replaced by simple stand-ins.
#include <cstdio>
#include <cstring>
#include <vector>
struct _worldHeader;
struct GameObject;
struct Creature;
struct Point3 { float x, y, z; };
_worldHeader* world = nullptr;
static std::vector<float> answers;
static std::vector<GameObject*> hit_objects;
static size_t calls;
extern "C" float icos__Fi(int a) { return a == 0 ? 1.0f : (a == 16384 ? 0.0f : (a == -16384 ? 0.0f : 0.5f)); }
extern "C" float isin__Fi(int a) { return a == 0 ? 0.0f : (a == 16384 ? 1.0f : (a == -16384 ? -1.0f : 0.25f)); }
extern "C" float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
    _worldHeader*, Point3, Point3, float, GameObject*, GameObject** slot, int mode, int flag) {
    if (mode != 3 || flag != 1) return 99.0f;
    *slot = calls < hit_objects.size() ? hit_objects[calls] : nullptr;
    return calls < answers.size() ? answers[calls++] : (++calls, 1.0f);
}
extern "C" Point3* wallAndObjectSteering__FP8CreatureffP10GameObject(Point3*, Creature*, float, float, GameObject*);
alignas(8) static unsigned char creature[800];
static Point3 run(GameObject* ignore) {
    Point3 out = {9, 9, 9};
    calls = 0;
    wallAndObjectSteering__FP8CreatureffP10GameObject(&out, reinterpret_cast<Creature*>(creature), 2.0f, 0.0f, ignore);
    return out;
}
static unsigned char other_object[4];
int main() {
    hit_objects.assign(5, reinterpret_cast<GameObject*>(other_object));     // no slot holds the ignore object
    float* pos = reinterpret_cast<float*>(creature + 16);
    pos[0] = 1; pos[1] = 2; pos[2] = 3;
    *reinterpret_cast<float*>(creature + 28) = 4.0f;               // span
    *reinterpret_cast<short*>(creature + 56) = 0;                  // heading
    int& state = *reinterpret_cast<int*>(creature + 772);
    state = 2;
    answers = {1.0f, 1.0f, 1.0f};                                  // nothing ahead: state cleared
    Point3 r = run(nullptr);
    if (state != 0 || r.x != 0 || r.y != 0 || r.z != 0 || calls != 3) return 1;
    state = 1;                                                     // existing state 1 steers without new probes
    answers = {0.5f, 1.0f, 1.0f};
    r = run(nullptr);
    if (state != 1 || calls != 3) return 2;
    if (r.x != 0.5f * 4.0f * 2.0f || r.y != 0.25f * 4.0f * 2.0f || r.z != 0) return 3;   // h - 12000 stand-ins
    state = 0;                                                     // blocked ahead, side probe D is open, E is blocked
    answers = {0.5f, 1.0f, 1.0f, 1.0f, 0.3f};
    r = run(nullptr);
    if (state != 2 || calls != 5) return 4;                        // E below 1.0 selects state 2
    state = 0;
    answers = {0.5f, 1.0f, 1.0f, 0.3f, 1.0f};
    r = run(nullptr);
    if (state != 1) return 5;                                      // D below 1.0 selects state 1
    // With the ignore object in slot 3 and D below the smallest of A, B, C the state is 2.
    state = 0;
    hit_objects[3] = reinterpret_cast<GameObject*>(creature);
    answers = {0.5f, 1.0f, 1.0f, 0.25f, 0.3f};
    r = run(reinterpret_cast<GameObject*>(creature));
    if (state != 2) return 6;
    // The ignore object in slot 0 at the smallest distance leaves the state alone.
    state = 0;
    hit_objects[0] = reinterpret_cast<GameObject*>(creature);
    answers = {0.5f, 1.0f, 1.0f, 0.25f, 0.3f};
    r = run(reinterpret_cast<GameObject*>(creature));
    if (state != 0 || r.x != 0 || r.y != 0) return 7;
    std::printf("wall and object steering contract passed\n");
    return 0;
}
