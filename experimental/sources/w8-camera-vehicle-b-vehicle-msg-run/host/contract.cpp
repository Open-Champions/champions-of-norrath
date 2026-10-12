// Host contract for msg_run__7Vehicle. Not an EE plan input.
// Built with -DNORRATH_W8_HOST on the source file as well.
#include <cstdio>
#include <cstring>
#include <string>
struct Point3 { float x, y, z; };
extern "C" void msg_run__7Vehicle(void* vehicle);

// ---- data the function reads ------------------------------------------------
extern "C" {
int localNmPlayersAlive = 0;
int gameControlPaused = 0;
unsigned norrath_actor_view_4844a4 = 0x7777u;
unsigned char player[4 * 23984];
unsigned char pad[4 * 88];
}
alignas(8) static unsigned char vehicle[2400];
alignas(8) static unsigned char vtable[256];
alignas(8) static unsigned char seats[4][300];
alignas(8) static unsigned char bodies[4][32];
static unsigned char* tokens[64];

extern "C" unsigned char* norrath_w8_host_resolve(unsigned bits) { return tokens[bits]; }

// ---- call trace ---------------------------------------------------------------
static std::string trace;
static void note(const char* name) { trace += name; trace += ' '; }
static int off_grid = 0, line_update = 0, slot232_result = 0, show_result = 0, connected[4] = {1, 1, 1, 1};
static int wall_hits[16], wall_index = 0, final_results[8], final_index = 0;
static int icon_a = 0x41, icon_b = 0x42, remove_calls = 0, removed[8], bound_after_remove = -1;
static float step_seen[16]; static int step_count = 0;
static void* step_args[16][2];
static int hud_time[4], hud_player[4], hud_count = 0; static unsigned short* hud_text[4];
static unsigned short text_wait[2] = {'W', 0}, text_help[2] = {'H', 0};
static unsigned short* usprintf_out = nullptr; static unsigned short* usprintf_format = nullptr; static float usprintf_args[8];
static unsigned long long fp_seen[8]; static int fp_count = 0; static int cmp_results[2];
static Point3 transform_in; static void* transform_out = nullptr; static void* transform_matrix = nullptr;
static Point3 transform_result = {1.0f, 2.0f, 3.0f};
static int dp_cmp_negative = 0;

extern "C" int objectIsOffGrid__FP10GameObject(void* o) { note("offgrid"); return o == vehicle ? off_grid : 99; }
extern "C" int gameLineUpdatePosition__FP17gameLinePathState(void* s) { note(s == vehicle + 2272 ? "line" : "line?"); return line_update; }
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* m, Point3* in) {
    note("transform"); transform_out = out; transform_matrix = m; transform_in = *in; *out = transform_result;
}
extern "C" unsigned short* textGetString__FPc(char* key) {
    note(std::strcmp(key, "vehicleWait") == 0 ? "text-wait" : std::strcmp(key, "vehicleHelpDriver") == 0 ? "text-help" : "text?");
    return std::strcmp(key, "vehicleWait") == 0 ? text_wait : text_help;
}
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* t, int time, int p, char flag) {
    note("hudset"); if (hud_count < 4) { hud_text[hud_count] = t; hud_time[hud_count] = time; hud_player[hud_count] = p; if (flag != 0) note("flag?"); } ++hud_count;
}
extern "C" void hudClearInfoMessage__Fv() { note("hudclear"); }
extern "C" int hudInfoMessageShowing__Fv() { note("hudshowing"); return show_result; }
extern "C" int gameButtonToIcon__Fi(int b) { note(b == 64 ? "icon64" : b == 128 ? "icon128" : "icon?"); return b == 64 ? icon_a : icon_b; }
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* f, float a, float b, float c, float d, float e, float g, float h, float i) {
    note("usprintf"); usprintf_out = out; usprintf_format = f;
    usprintf_args[0] = a; usprintf_args[1] = b; usprintf_args[2] = c; usprintf_args[3] = d; usprintf_args[4] = e; usprintf_args[5] = g; usprintf_args[6] = h; usprintf_args[7] = i;
    out[0] = 'U'; out[1] = 0;
}
#define VEHICLE_CALL(name) extern "C" void name##__7Vehicle(void* v) { note(#name); if (v != vehicle) note("badarg"); }
VEHICLE_CALL(startForces) VEHICLE_CALL(doAI) VEHICLE_CALL(doSteering) VEHICLE_CALL(doBrakes) VEHICLE_CALL(doThrottle)
VEHICLE_CALL(doSuspension) VEHICLE_CALL(doDriveline) VEHICLE_CALL(doMisc) VEHICLE_CALL(backup) VEHICLE_CALL(checkVehicleCollision)
extern "C" void getNewRotAndPos__7VehicleP6Point3T1fii(void* v, void* a, void* b, float step, int c, int d) {
    note("newrot"); if (v != vehicle || c != 0 || d != 0) note("badarg");
    if (step_count < 16) { step_seen[step_count] = step; step_args[step_count][0] = a; step_args[step_count][1] = b; } ++step_count;
}
extern "C" int checkWallCollision__7Vehicle(void*) { note("wall"); return wall_hits[wall_index < 16 ? wall_index++ : 15]; }
extern "C" int finalizePosition__7Vehicle(void*) { note("final"); return final_results[final_index < 8 ? final_index++ : 7]; }
extern "C" void removePassenger__7Vehiclei(void*, int p) {
    note("remove"); if (remove_calls < 8) removed[remove_calls] = p; ++remove_calls;
    // the count word shrinks, as a real removal would; the function must re-read it
    if (bound_after_remove >= 0) { int v = bound_after_remove; std::memcpy(vehicle + 976, &v, 4); }
}
extern "C" int netPlayerIsConnected__Fi(int p) { note("connected"); return connected[p]; }
extern "C" unsigned long long fptodp(float v) {
    note("fptodp"); double d = v; unsigned long long b; std::memcpy(&b, &d, 8); fp_seen[0] = b; return b;
}
extern "C" int dpcmp(unsigned long long a, unsigned long long b) {
    note("dpcmp"); double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); return x < y ? -1 : x > y ? 1 : 0;
}
extern "C" unsigned long long dpsub(unsigned long long a, unsigned long long b) {
    note("dpsub"); double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double r = x - y; unsigned long long o; std::memcpy(&o, &r, 8); return o;
}
static int slot232_calls = 0, slot192_calls = 0; static void* slot_self[2];
static int slot232(void* self) { note("slot232"); ++slot232_calls; slot_self[0] = self; return slot232_result; }
static int slot192(void* self) { note("slot192"); ++slot192_calls; slot_self[1] = self; return 55; }

// ---- helpers ----------------------------------------------------------------
static void putw(unsigned char* b, int off, unsigned v) { std::memcpy(b + off, &v, 4); }
static void puti(unsigned char* b, int off, int v) { std::memcpy(b + off, &v, 4); }
static void putf(unsigned char* b, int off, float v) { std::memcpy(b + off, &v, 4); }
static unsigned getw(const unsigned char* b, int off) { unsigned v; std::memcpy(&v, b + off, 4); return v; }
static float getf(const unsigned char* b, int off) { float v; std::memcpy(&v, b + off, 4); return v; }
static void reset() {
    std::memset(vehicle, 0, sizeof vehicle); std::memset(vtable, 0, sizeof vtable); std::memset(seats, 0, sizeof seats);
    std::memset(bodies, 0, sizeof bodies); std::memset(player, 0, sizeof player); std::memset(pad, 0, sizeof pad);
    trace.clear(); off_grid = line_update = slot232_result = show_result = 0;
    for (int i = 0; i < 4; ++i) connected[i] = 1;
    std::memset(wall_hits, 0, sizeof wall_hits); wall_index = 0; std::memset(final_results, 0, sizeof final_results); final_index = 0;
    remove_calls = 0; bound_after_remove = -1; step_count = 0; hud_count = 0; slot232_calls = slot192_calls = 0;
    usprintf_out = nullptr; fp_count = 0;
    localNmPlayersAlive = 0; gameControlPaused = 0; norrath_actor_view_4844a4 = 0x7777u;
    tokens[1] = vtable; tokens[2] = reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned long>(&slot232));
    tokens[3] = reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned long>(&slot192));
    for (int i = 0; i < 4; ++i) { tokens[10 + i] = seats[i]; tokens[20 + i] = bodies[i]; }
    putw(vehicle, 72, 1);
    short adjust = 8; std::memcpy(vtable + 192, &adjust, 2); std::memcpy(vtable + 232, &adjust, 2);
    putw(vtable, 196, 3); putw(vtable, 236, 2);
    puti(vehicle, 956, -1);
    for (int i = 0; i < 4; ++i) puti(vehicle, 960 + 4 * i, -1);
    putf(vehicle, 840, 1.0f);  // keeps the quiet branch of step F off unless a test asks for it
}
static const char* const quiet =
    "offgrid line startForces doSteering doBrakes doThrottle slot232 doSuspension doDriveline doMisc slot192 newrot wall final checkVehicleCollision ";
static int failures = 0;
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n   trace: %s\n", m, trace.c_str()); ++failures; } } while (0)
static bool trace_is(const char* want) { return trace == want; }
static bool starts(const char* want) { return trace.compare(0, std::strlen(want), want) == 0; }

int main() {
    // off grid: nothing else happens
    reset(); off_grid = 1; putf(vehicle, 808, 5.0f);
    msg_run__7Vehicle(vehicle);
    EXPECT(trace_is("offgrid "), "off-grid returns immediately");
    EXPECT(getf(vehicle, 808) == 5.0f, "off-grid leaves +808");

    // baseline order, step F "newrot" branch (840 not below 0.25)
    reset(); msg_run__7Vehicle(vehicle);
    EXPECT(trace_is(quiet), "baseline call order");
    EXPECT(slot_self[0] == vehicle + 8 && slot_self[1] == vehicle + 8, "slots get the adjusted object");
    EXPECT(step_count == 1, "one newrot call in the baseline");
    { unsigned b; float f = step_seen[0]; std::memcpy(&b, &f, 4); EXPECT(b == 0x3c888888u, "newrot step value is the 1/60 word"); }
    EXPECT(step_args[0][0] == vehicle + 2004 && step_args[0][1] == vehicle + 1992, "newrot points");

    // step F: all three quiet conditions zero the floats and skip the first newrot
    reset(); putf(vehicle, 840, 0.2f); putf(vehicle, 1188, 9.f); putf(vehicle, 1192, 9.f); putf(vehicle, 1196, 9.f); putw(vehicle, 2144, 0);
    msg_run__7Vehicle(vehicle);
    EXPECT(trace_is("offgrid line startForces doSteering doBrakes doThrottle slot232 doSuspension doDriveline doMisc slot192 wall final checkVehicleCollision "), "quiet branch skips newrot");
    EXPECT(getf(vehicle, 1188) == 0.f && getf(vehicle, 1192) == 0.f && getf(vehicle, 1196) == 0.f, "quiet branch zeroes");
    reset(); putf(vehicle, 840, 0.25f);
    msg_run__7Vehicle(vehicle); EXPECT(trace_is(quiet), "840 equal to 0.25 is not quiet");
    reset(); putf(vehicle, 840, 0.2f); putw(vehicle, 2144, 1);
    msg_run__7Vehicle(vehicle); EXPECT(trace_is(quiet) && getw(vehicle, 2144) == 0, "2144 nonzero is not quiet and is cleared");
    reset(); putf(vehicle, 840, 0.2f); putf(vehicle, 740, 0.5f);
    msg_run__7Vehicle(vehicle); EXPECT(trace_is(quiet), "740 nonzero is not quiet");
    reset(); putf(vehicle, 840, 5.0f); putw(vehicle, 1024, 1); putf(vehicle, 1188, 9.f);
    msg_run__7Vehicle(vehicle);
    EXPECT(trace_is("offgrid line startForces doSteering doBrakes doThrottle slot232 doSuspension doDriveline doMisc slot192 wall final checkVehicleCollision ") && getf(vehicle, 1188) == 0.f, "1024 forces the quiet branch");

    // slot 232 nonzero skips the three; doAI runs when 2148 is nonzero
    reset(); slot232_result = 1; putw(vehicle, 2148, 1);
    msg_run__7Vehicle(vehicle);
    EXPECT(trace_is("offgrid line startForces doAI doSteering doBrakes doThrottle slot232 slot192 newrot wall final checkVehicleCollision "), "slot 232 nonzero and doAI");

    // step B
    reset(); putf(vehicle, 808, 3.5f); msg_run__7Vehicle(vehicle);
    { unsigned b = 0x3f7fffffu; float k; std::memcpy(&k, &b, 4); EXPECT(getf(vehicle, 808) == 3.5f - k, "808 drops by the constant"); }
    reset(); putf(vehicle, 808, 0.5f); msg_run__7Vehicle(vehicle); EXPECT(getf(vehicle, 808) == 0.0f, "808 clamps at zero");
    reset(); msg_run__7Vehicle(vehicle); EXPECT(getf(vehicle, 808) == 0.0f, "808 zero stays");

    // step C
    reset(); line_update = 1; putf(vehicle, 896, 10.0f); putf(vehicle, 16, 100.f); putf(vehicle, 20, 200.f); putf(vehicle, 24, 300.f);
    msg_run__7Vehicle(vehicle);
    EXPECT(starts("offgrid line transform startForces"), "transform follows line update");
    EXPECT(transform_out == vehicle + 656 && transform_matrix == vehicle + 1116, "transform out and matrix");
    EXPECT(transform_in.x == 0.f && transform_in.y == -5.0f && transform_in.z == 0.f, "transform input");
    EXPECT(getf(vehicle, 656) == 101.f && getf(vehicle, 660) == 202.f && getf(vehicle, 664) == 303.f, "position added");
    reset(); line_update = 0; msg_run__7Vehicle(vehicle);
    EXPECT(trace.find("transform") == std::string::npos, "no transform without line update");

    // step D: wait message
    reset(); localNmPlayersAlive = 3; puti(vehicle, 956, 2); puti(vehicle, 980, 2); putw(vehicle, 1032, 1);
    msg_run__7Vehicle(vehicle);
    EXPECT(starts("offgrid line text-wait hudset startForces") && hud_count == 1 && hud_time[0] == 180 && hud_player[0] == 2 && hud_text[0] == text_wait, "wait message");
    EXPECT(getw(vehicle, 1036) == 1 && getw(vehicle, 1032) == 1, "wait sets 1036, leaves 1032");
    // same count: clear test instead
    reset(); localNmPlayersAlive = 3; puti(vehicle, 956, 2); puti(vehicle, 980, 3); putw(vehicle, 1036, 1);
    msg_run__7Vehicle(vehicle);
    EXPECT(starts("offgrid line hudclear startForces") && getw(vehicle, 1036) == 0, "equal count clears");
    // each failed gate gives the clear test, not the wait message
    for (int gate = 0; gate < 3; ++gate) {
        reset(); localNmPlayersAlive = gate == 0 ? 1 : 3; puti(vehicle, 956, gate == 2 ? -1 : 2); puti(vehicle, 980, 9);
        if (gate == 1) putw(vehicle, 2148, 1);
        putw(vehicle, 1036, 1);
        msg_run__7Vehicle(vehicle);
        EXPECT(starts("offgrid line hudclear") && hud_count == 0, "failed gate clears instead");
    }
    // help message
    reset(); putw(vehicle, 1032, 1); puti(vehicle, 956, 3);
    msg_run__7Vehicle(vehicle);
    EXPECT(starts("offgrid line hudshowing text-help icon64 icon128 usprintf hudset startForces"), "help call order");
    EXPECT(getw(vehicle, 1032) == 0, "help clears 1032");
    EXPECT(usprintf_format == text_help && usprintf_args[0] == 65.0f && usprintf_args[1] == 66.0f, "help icons");
    for (int i = 2; i < 8; ++i) EXPECT(usprintf_args[i] == 0.0f, "help trailing zeros");
    EXPECT(hud_count == 1 && hud_time[0] == 300 && hud_player[0] == 3 && hud_text[0] == usprintf_out, "help message shown");
    reset(); putw(vehicle, 1032, 1); show_result = 1; msg_run__7Vehicle(vehicle);
    EXPECT(hud_count == 0 && getw(vehicle, 1032) == 1, "help waits while a message is showing");
    reset(); putw(vehicle, 1032, 1); putw(vehicle, 1024, 1); msg_run__7Vehicle(vehicle);
    EXPECT(hud_count == 0 && getw(vehicle, 1032) == 1 && trace.find("hudshowing") == std::string::npos, "help waits while 1024 is set");
    reset(); msg_run__7Vehicle(vehicle); EXPECT(trace.find("hudshowing") == std::string::npos, "no help query when 1032 is clear");

    // step G: wall loop
    reset(); for (int i = 0; i < 5; ++i) wall_hits[i] = 1;
    msg_run__7Vehicle(vehicle);
    EXPECT(wall_index == 5 && step_count == 6, "five hits: five checks and six newrot calls");
    EXPECT(trace.find("backup") != std::string::npos, "backup after five hits");
    reset(); for (int i = 0; i < 4; ++i) wall_hits[i] = 1;
    msg_run__7Vehicle(vehicle);
    EXPECT(wall_index == 5 && step_count == 5 && trace.find("backup") != std::string::npos, "four hits then clear: backup runs");
    reset(); for (int i = 0; i < 3; ++i) wall_hits[i] = 1;
    msg_run__7Vehicle(vehicle);
    EXPECT(wall_index == 4 && step_count == 4 && trace.find("backup") == std::string::npos, "three hits then clear: no backup");
    // finalize retries
    reset(); for (int i = 0; i < 8; ++i) final_results[i] = 1;
    msg_run__7Vehicle(vehicle);
    EXPECT(final_index == 4 && wall_index == 4 && step_count == 1 + 3, "finalize retries three times");
    reset(); final_results[0] = 1; wall_hits[0] = 1; wall_hits[1] = 1; wall_hits[2] = 0;
    msg_run__7Vehicle(vehicle);
    EXPECT(wall_index == 3 + 1 && final_index == 2 && trace.find("backup") == std::string::npos, "retry restarts the counter");
    // byte 110
    reset(); vehicle[110] = 3; msg_run__7Vehicle(vehicle); EXPECT(vehicle[110] == 2, "110 decrements");
    reset(); vehicle[110] = 0; msg_run__7Vehicle(vehicle); EXPECT(vehicle[110] == 0, "110 zero stays");
    reset(); vehicle[110] = 0x80; msg_run__7Vehicle(vehicle); EXPECT(vehicle[110] == 0x7f, "110 negative decrements");

    // step H: -1 players
    reset(); puti(vehicle, 976, 2); putw(vehicle, 2088, 10); putw(vehicle, 2092, 11);
    putf(seats[0], 284, 20.0f); putf(seats[1], 284, 0.0f);
    msg_run__7Vehicle(vehicle);
    EXPECT(getf(seats[0], 284) == 20.0f * [] { unsigned b = 0x3f599998u; float f; std::memcpy(&f, &b, 4); return f; }(), "seat decays");
    EXPECT(trace.find("dpsub") == std::string::npos && getf(seats[1], 284) == 0.0f, "no abs for a positive value; zero skipped");
    reset(); puti(vehicle, 976, 1); putw(vehicle, 2088, 10); putf(seats[0], 284, 0.5f);
    msg_run__7Vehicle(vehicle);
    EXPECT(getf(seats[0], 284) == 0.0f, "small positive decays to zero");
    reset(); puti(vehicle, 976, 1); putw(vehicle, 2088, 10); putf(seats[0], 284, -0.5f);
    msg_run__7Vehicle(vehicle);
    EXPECT(getf(seats[0], 284) == 0.0f && trace.find("dpsub") != std::string::npos, "small negative uses dpsub and zeroes");
    reset(); puti(vehicle, 976, 1); putw(vehicle, 2088, 10); putf(seats[0], 284, -2.0f);
    msg_run__7Vehicle(vehicle);
    EXPECT(getf(seats[0], 284) < -1.0f, "large negative stays");
    reset(); puti(vehicle, 976, 1); putf(seats[0], 284, 0.5f);  // null seat: nothing to do
    msg_run__7Vehicle(vehicle); EXPECT(getf(seats[0], 284) == 0.5f, "null seat untouched");

    // step H: player present
    // seat steering: p = 1
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 1); putw(vehicle, 2088, 10);
    putw(player + 23984, 21872, 21);           // player 1 body is token 21
    putw(pad + 88, 12, 0);                      // pad flags clear
    putf(pad + 88, 20, 2.0f); putf(pad + 88, 24, 4.0f);
    putf(seats[0], 280, 1.0f); putf(seats[0], 284, 10.0f);
    msg_run__7Vehicle(vehicle);
    EXPECT(getf(seats[0], 280) == 1.0f + -2.0f * 3.0f, "seat first float");
    EXPECT(getf(seats[0], 284) == 6.0f, "seat second float");
    EXPECT(remove_calls == 0, "healthy player stays");
    // clamps
    putf(pad + 88, 24, -100.0f); putf(seats[0], 284, 10.0f); trace.clear();
    msg_run__7Vehicle(vehicle); EXPECT(getf(seats[0], 284) == 15.0f, "upper clamp");
    putf(pad + 88, 24, 100.0f); putf(seats[0], 284, 10.0f);
    msg_run__7Vehicle(vehicle); EXPECT(getf(seats[0], 284) == -15.0f, "lower clamp");
    // flag 0x400000 on the body or pause skips the update
    putw(bodies[1], 12, 0x400000u); putf(seats[0], 284, 10.0f); putf(seats[0], 280, 1.0f);
    msg_run__7Vehicle(vehicle); EXPECT(getf(seats[0], 280) == 1.0f && getf(seats[0], 284) == 10.0f, "0x400000 skips");
    putw(bodies[1], 12, 0); gameControlPaused = 1;
    msg_run__7Vehicle(vehicle); EXPECT(getf(seats[0], 280) == 1.0f, "paused skips");
    gameControlPaused = 0;
    // 1024 set: no removal checks even for a missing body, and the update still runs
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 2); putw(vehicle, 2088, 10); putw(vehicle, 1024, 1);
    putf(pad + 176, 20, 1.0f); putf(seats[0], 284, 0.0f);
    msg_run__7Vehicle(vehicle);
    EXPECT(remove_calls == 0 && trace.find("connected") == std::string::npos, "1024 skips removal checks");
    EXPECT(getf(seats[0], 280) == -3.0f, "update still runs with 1024");
    // removal: null body
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 3); putw(vehicle, 2088, 10);
    msg_run__7Vehicle(vehicle);
    EXPECT(remove_calls == 1 && removed[0] == 3 && trace.find("connected") == std::string::npos, "null body removes without a connect query");
    // removal: flag 8
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 0); putw(vehicle, 2088, 10); putw(player, 21872, 20); putw(bodies[0], 12, 8);
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 1 && removed[0] == 0, "flag 8 removes");
    // removal: disconnected
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 0); putw(vehicle, 2088, 10); putw(player, 21872, 20); connected[0] = 0;
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 1, "disconnected removes");
    // pad bit 0x10 path
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 2); putw(vehicle, 2088, 10); putw(player + 2 * 23984, 21872, 22);
    putw(pad + 176, 12, 0x10); putw(vehicle, 1004 + 8, 0x1234);
    msg_run__7Vehicle(vehicle);
    EXPECT(remove_calls == 1 && removed[0] == 2 && player[2 * 23984 + 23969] == 20, "pad bit with a different frame removes and marks the player");
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 2); putw(vehicle, 2088, 10); putw(player + 2 * 23984, 21872, 22);
    putw(pad + 176, 12, 0x10); putw(vehicle, 1004 + 8, 0x7777);
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 0 && player[2 * 23984 + 23969] == 0, "same frame keeps");
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 2); putw(vehicle, 2088, 10); putw(player + 2 * 23984, 21872, 22);
    putw(pad + 176, 12, 0x10); putw(vehicle, 1004 + 8, 0x1234); gameControlPaused = 1;
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 0 && player[2 * 23984 + 23969] == 0, "paused keeps");
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 2); putw(vehicle, 2088, 10); putw(player + 2 * 23984, 21872, 22);
    putw(pad + 176, 12, 0x10); putw(vehicle, 1004 + 8, 0x1234); putw(bodies[2], 12, 0x400000);
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 0 && player[2 * 23984 + 23969] == 0, "0x400000 keeps");
    reset(); puti(vehicle, 976, 1); puti(vehicle, 960, 2); putw(vehicle, 2088, 10); putw(player + 2 * 23984, 21872, 22);
    putw(pad + 176, 12, 0x01); putw(vehicle, 1004 + 8, 0x1234);
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 0, "pad bit clear keeps");
    // the loop bound is re-read after a removal, and the seat word after it too
    reset(); puti(vehicle, 976, 3); for (int i = 0; i < 3; ++i) puti(vehicle, 960 + 4 * i, 3); putw(vehicle, 2088, 10);
    bound_after_remove = 1;  // first removal drops the count to 1, ending the loop
    msg_run__7Vehicle(vehicle);
    EXPECT(remove_calls == 1, "bound re-read after removal");
    reset(); puti(vehicle, 976, 3); for (int i = 0; i < 3; ++i) puti(vehicle, 960 + 4 * i, 3);
    msg_run__7Vehicle(vehicle); EXPECT(remove_calls == 3, "three seats without a bound change run three times");
    return failures;
}
