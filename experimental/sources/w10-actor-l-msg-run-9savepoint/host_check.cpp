// Host contract for msg_run__9Savepoint; not an EE input. Words holding addresses are offsets from
// a host base. The random source returns the constant 32768, so every unit-ball draw is the zero
// vector, every scaled draw is 0.5 and the velocity draws are -3 (all exact in float), which makes
// the burst's numbers checkable by hand: scale = 0.5 * (0.5 * 650 + 50) + 24 = 211.5.
#define W10_ACTOR_L_HOST 1
#include "../w10-actor-l-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls; static int rands, parts, rots, ident, async_result, active_result, nozone, nm_result = 3, nmloc = 2; static int conn[4], local[4];
static Point3 first_pos, first_vel; static float first_value; static int first_seen;
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - w10_actor_l_host_base) : 0u; }
extern "C" {
unsigned char saveParticle[8], saveParticle2[8], player[23984 * 4 + 256]; signed char MC_LastModeSuccessful, netNetworkMode; unsigned char netForceLocalUpdate;
int nmPlayers, nmPlayersAlive, frameNm; short gameDeathTimerSingle[4]; unsigned char* gameCam;
int objectActiveProcessing__FP10GameObjectii(void*, int v) { calls += "active(" + std::to_string(v) + ");"; return active_result; }
void msgHudSavePoint__FP9Savepoint(void*) { calls += "hud;"; }
int MC_GetAsyncMode__Fv() { calls += "async;"; return async_result; }
char* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return reinterpret_cast<char*>(0x40); }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int m) { calls += "animAdd(" + std::to_string(off(s)) + "," + std::to_string(reinterpret_cast<unsigned long>(h)) + "," + std::to_string(m) + ");"; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int w) { calls += "chunks(" + std::to_string(off(m)) + "," + std::to_string(w) + ");"; return 0x1122334455667788ull; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float r) { calls += "sound(" + std::to_string(r) + ");"; return 0; }
unsigned eRand__10GameObject(void*) { ++rands; return 32768u; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { calls += "nm;"; return nm_result; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void* s, const void* p, short yaw, int idx, void* out, float* scalar, void* m, float sc) {
    float v[3]; std::memcpy(v, p, 12); calls += "spos(" + std::to_string(off(s)) + "," + std::to_string(v[0]) + "," + std::to_string(yaw) + "," + std::to_string(idx) + "," + std::to_string(m == nullptr) + "," + std::to_string(sc) + ");";
    float o3[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(out, o3, 12); *scalar = 9.0f; }
void matIdent__FP8Matrix33(void* m) { ++ident; float* f = static_cast<float*>(m); for (int i = 0; i < 9; ++i) f[i] = (i % 4 == 0) ? 1.0f : 0.0f; }
void matRot__FP8Matrix33is(void* m, int axis, short a) { ++rots; float* f = static_cast<float*>(m); if (rots <= 2) calls += "rot(" + std::to_string(axis) + "," + std::to_string(a) + ");"; f[0] = 1.0f; f[1] = 0.5f; }
void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* m, Point3* in) { float* f = static_cast<float*>(m); out->x = f[0] * in->x; out->y = f[1]; out->z = 0.25f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, Point3* p, Point3* v, float value) {
    ++parts;
    if (!first_seen) { first_seen = 1; first_pos = *p; first_vel = *v; first_value = value; calls += std::string("part(") + (def == saveParticle ? "1" : "0") + ");"; }
    if (def == saveParticle2) { calls += "part2(" + std::to_string(p->x) + "," + std::to_string(p->y) + "," + std::to_string(p->z) + "," + std::to_string(v->x) + "," + std::to_string(v->z) + "," + std::to_string(value) + ");"; }
}
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return 0.5f; }
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* s, Point3* p, void* b, short* c) { calls += "adv(" + std::to_string(off(s)) + "," + std::to_string(off(p)) + "," + std::to_string(b == nullptr) + "," + std::to_string(c == nullptr) + ");"; }
void updatePlayersReady__9Savepoint(void*) { calls += "ready;"; }
int netPlayerIsConnected__Fi(int p) { calls += "conn" + std::to_string(p) + ";"; return conn[p]; }
void playerRaisePlayer__FiG6Point3(int p, const void* pos) { float v[3]; std::memcpy(v, pos, 12); calls += "raise(" + std::to_string(p) + "," + std::to_string(v[1]) + ");"; }
void enumerateAlivePlayers__Fv() { calls += "enum;"; }
int netPlayerIsLocal__Fi(int p) { calls += "local" + std::to_string(p) + ";"; return local[p]; }
void Reset__6Cameraii(void*, int a, int b) { calls += "camreset(" + std::to_string(a) + "," + std::to_string(b) + ");"; }
void Update__6Camera(void*) { calls += "camupd;"; }
void coreStartSwitchScene__Fi(int s) { calls += "scene(" + std::to_string(s) + ");"; }
int gameCheckNoSaveZone__Fv() { calls += "zone;"; return nozone; }
void MC_SaveCheckPoint__Fiii(int a, int b, int c) { calls += "cp(" + std::to_string(a) + std::to_string(b) + std::to_string(c) + ");"; }
unsigned short* textGetString__FPc(char* k) { calls += std::string("text(") + k + ");"; return reinterpret_cast<unsigned short*>(0x60); }
int netGetLocalPlayerNm__Fi(int i) { calls += "nmloc(" + std::to_string(i) + ");"; return nmloc; }
void hudSetInfoMessage__FPUsiic(unsigned short* t, int a, int b, char c) { calls += "info(" + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ");"; }
}
#include "msg_run.cpp"
alignas(16) static unsigned char arena[0x4000];
static unsigned char* const O = arena + 0x100;
static void put32(unsigned at, unsigned v) { std::memcpy(O + at, &v, 4); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, O + at, 4); return v; }
static void reset() {
    std::memset(arena, 0, sizeof arena); calls.clear(); rands = parts = rots = ident = 0; first_seen = 0; async_result = 0; active_result = 0x1f; nozone = 0;
    std::memset(player, 0, sizeof player); std::memset(gameDeathTimerSingle, 0, sizeof gameDeathTimerSingle);
    MC_LastModeSuccessful = 0; netNetworkMode = 0; netForceLocalUpdate = 0; nmPlayers = nmPlayersAlive = 0; frameNm = 1; gameCam = arena + 0x3000; nm_result = 3;
    for (int i = 0; i < 4; ++i) conn[i] = local[i] = 0;
    put32(36, 0x800); { unsigned m = 0xa00; std::memcpy(arena + 0x800 + 12, &m, 4); }
    float p[3] = {5.0f, 6.0f, 7.0f}; std::memcpy(O + 16, p, 12); O[196] = 0xfe;
}
int main() {
    w10_actor_l_host_base = arena; int bad = 0;
    // nothing active: only the processing call; byte 196 stores the result
    reset(); msg_run__9Savepoint(O); if (calls != "active(-2);" || O[196] != 0x1f) bad = 1;
    // A: pending flag with last mode successful, no burst
    reset(); put32(132, 1); MC_LastModeSuccessful = 1; msg_run__9Savepoint(O); if (calls != "active(-2);hud;" || MC_LastModeSuccessful != 0 || get32(132) != 1) bad = 1;
    reset(); put32(132, 1); async_result = 0; msg_run__9Savepoint(O); if (calls != "active(-2);async;" || get32(132) != 0) bad = 1;
    reset(); put32(132, 1); async_result = 4; msg_run__9Savepoint(O); if (calls != "active(-2);async;" || get32(132) != 1) bad = 1;
    // A + burst: counter 1, word128 zero -> init, sound, 66 passes of 80 particles
    reset(); put32(132, 1); async_result = 1; put32(204, 1); msg_run__9Savepoint(O);
    {
        const std::string want = "active(-2);async;lump(savepnt.lmp,rise.anm);animAdd(332,64,4);chunks(2560,1);lump(fx.lmp,game_lvl_save.anm);sound(1.000000);"
            "nm;spos(332,5.000000,0,2,1,1.000000);rot(2,4000);rot(2,1000);part(1);sin(1024);adv(332,272,1,1);";
        // word128 became 1 and was then incremented to 2 by section B; bit 7 of +82 is clear
        if (calls != want) bad = 1;
        if (parts != 66 * 80 || rots != 67 || ident != 1 || get32(204) != 2 || get32(128) != 2 || get32(132) != 1) bad = 1;
        // rands: 3 (ball) + 66 * (3 + 80 * 3) = 3 + 66*243
        if (rands != 3 + 66 * 243) bad = 1;
        unsigned long long d; std::memcpy(&d, O + 120, 8); if (d != 0x1122334455667788ull) bad = 1;
        // first particle: out = (1,2,3); direction = (1*1, 0.5, 0.25); scale 211.5
        if (first_pos.x != 1.0f + 211.5f || first_pos.y != 2.0f + 0.5f * 211.5f || first_pos.z != 3.0f + 0.25f * 211.5f || first_vel.x != -3.0f || first_vel.z != -3.0f || first_value != 0.5f) bad = 1;
        // light floats: f = 0.5 * 4 + 120 = 122 at +136+28 and +36; 1 / 14884 at +40 and +32
        float f28, f36, f40, f32; std::memcpy(&f28, O + 136 + 28, 4); std::memcpy(&f36, O + 136 + 36, 4); std::memcpy(&f40, O + 136 + 40, 4); std::memcpy(&f32, O + 136 + 32, 4);
        if (f28 != 122.0f || f36 != 122.0f || f40 != 1.0f / (122.0f * 122.0f) || f32 != f40) bad = 1;
    }
    // counter 3 -> becomes 4: both words cleared after the burst; counter 2 stays
    reset(); put32(132, 1); async_result = 1; put32(204, 3); put32(128, 7); msg_run__9Savepoint(O); if (get32(204) != 0 || get32(132) != 0 || get32(128) != 8) bad = 1;
    reset(); put32(132, 1); async_result = 1; put32(204, 2); put32(128, 7); msg_run__9Savepoint(O); if (get32(204) != 3 || get32(132) != 1) bad = 1;
    // B wrap and the bit-7 animation
    reset(); put32(128, 0xffffffffu); msg_run__9Savepoint(O); if (get32(128) != 1) bad = 1;
    reset(); put32(128, 4); O[82] = 0x80; O[80] = 9; msg_run__9Savepoint(O); if (O[80] != 0 || calls.find("lump(savepnt.lmp,float.anm);animAdd(332,64,3);sin(2560);adv(332,272,1,1);") == std::string::npos) bad = 1;
    // B else: two particle spawns from zero vectors
    reset(); O[197] = 1; msg_run__9Savepoint(O);
    if (calls != "active(-2);part(0);part2(5.000000,6.000000,57.000000,0.000000,0.000000,0.000000);") bad = 1;
    // C: frameNm gate and player scan
    reset(); frameNm = 8; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    reset(); frameNm = 16; O[200] = 1; nmPlayers = 3; nmPlayersAlive = 2; conn[0] = conn[1] = conn[2] = 1; local[1] = 1; netNetworkMode = 1;
    { float neg = -1.0f, pos = 2.0f; std::memcpy(player + 172, &neg, 4); std::memcpy(player + 172 + 23984, &neg, 4); std::memcpy(player + 172 + 2 * 23984, &pos, 4); }
    msg_run__9Savepoint(O);
    if (calls != "active(-2);ready;conn0;conn1;conn2;raise(1,6.000000);enum;local1;camreset(0,0);camupd;local1;scene(4);zone;" || gameDeathTimerSingle[1] != 240 || netForceLocalUpdate != 1) bad = 1;
    reset(); frameNm = 0; O[200] = 1; nmPlayers = 3; nmPlayersAlive = 3; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    reset(); frameNm = 0; O[200] = 1; nmPlayers = 1; nmPlayersAlive = 0; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    reset(); frameNm = 0; O[200] = 1; nmPlayers = 2; nmPlayersAlive = 1; conn[0] = 1; gameDeathTimerSingle[0] = 5; msg_run__9Savepoint(O); if (calls != "active(-2);ready;conn0;conn1;zone;" || gameDeathTimerSingle[0] != 5) bad = 1;
    // save checkpoint branches
    reset(); frameNm = 0; nozone = 1; O[198] = 1; O[199] = 1; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    reset(); frameNm = 0; O[198] = 1; O[199] = 1; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;cp(000);" || O[198] != 0 || O[197] != 1) bad = 1;
    reset(); frameNm = 0; O[198] = 1; O[199] = 0xff; O[201] = 1; nmloc = 2; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;text(CheckPointNeedPlayers);nmloc(0);info(96,120,2,0);") bad = 1;
    reset(); frameNm = 0; O[198] = 1; O[199] = 0xff; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    reset(); frameNm = 0; O[198] = 1; O[199] = 0; O[201] = 1; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    reset(); frameNm = 0; O[198] = 0; O[199] = 1; msg_run__9Savepoint(O); if (calls != "active(-2);ready;zone;") bad = 1;
    return bad;
}
