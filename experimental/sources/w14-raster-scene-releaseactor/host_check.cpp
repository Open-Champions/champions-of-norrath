// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
// The four scene calls are stubbed: each records its arguments in call order and returns a scripted value.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "release_actor.cpp"
#include <cstdio>
#include <cstring>
static int order[8], nargs[8][2], calls;
static int script[4];
static unsigned char* sceneSeen[8];
static void note(int id, unsigned char* s, int a) { sceneSeen[calls] = s; order[calls] = id; nargs[calls][0] = a; ++calls; }
extern "C" int RemoveActor__7VIScenei(unsigned char* s, int a) { note(0, s, a); return script[0]; }
extern "C" int ReleaseSprite__7VIScenei(unsigned char* s, int a) { note(1, s, a); return script[1]; }
extern "C" int ReleaseStaticLighting__7VIScenei(unsigned char* s, int a) { note(2, s, a); return script[2]; }
extern "C" int Erase__t6VIList1Z7VIActori(unsigned char* s, int a) { note(3, s, a); return script[3]; }
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    unsigned char scene[2048];
    unsigned actorAddress = arena_base + 0x100;
    W(0x100 + 96) = 5; W(0x100 + 108) = 6;
    script[0] = 0; script[1] = 0; script[2] = 0; script[3] = 77;
    int r = ReleaseActor__7VIScenei(scene, static_cast<int>(actorAddress));
    bool ok = r == 77 && calls == 4 && order[0] == 0 && order[1] == 1 && order[2] == 2 && order[3] == 3 && nargs[1][0] == 5 && nargs[2][0] == 6 &&
              sceneSeen[3] == scene + 972 && nargs[3][0] == static_cast<int>(actorAddress) && sceneSeen[0] == scene && sceneSeen[1] == scene;
    // no sprite, no lighting: those calls are skipped
    calls = 0; W(0x100 + 96) = 0xffffffffu; W(0x100 + 108) = 0xffffffffu;
    r = ReleaseActor__7VIScenei(scene, static_cast<int>(actorAddress));
    ok = ok && r == 77 && calls == 2 && order[0] == 0 && order[1] == 3;
    // failures stop at the failing step with -1
    calls = 0; W(0x100 + 96) = 5; W(0x100 + 108) = 6; script[0] = -1;
    ok = ok && ReleaseActor__7VIScenei(scene, static_cast<int>(actorAddress)) == -1 && calls == 1;
    calls = 0; script[0] = 0; script[1] = -4;
    ok = ok && ReleaseActor__7VIScenei(scene, static_cast<int>(actorAddress)) == -1 && calls == 2;
    calls = 0; script[1] = 0; script[2] = -1;
    ok = ok && ReleaseActor__7VIScenei(scene, static_cast<int>(actorAddress)) == -1 && calls == 3;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
