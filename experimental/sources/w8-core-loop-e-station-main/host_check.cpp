// Host check for station_main__FiPCcPcT2T1T1i. Not an EE plan input.
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
#define printf norrath_host_printf
#include "station_main.cpp"
#undef printf

static std::string trace;
static std::vector<int> process_results;
static size_t process_at = 0;
static int init_result = 0;
static void* object_seen = nullptr;
static const char *a_name, *a_text, *a_copied;
static char *a_user, *a_pass;
static int a_mode, a_launch;

extern "C" int norrath_host_printf(const char* format, ...) {
    char text[128];
    va_list ap; va_start(ap, format);
    std::vsnprintf(text, sizeof text, format, ap);
    va_end(ap);
    trace += "[" + std::string(text) + "]";
    return 0;
}
extern "C" void* __9VIStation(void* o) { trace += "C;"; object_seen = o; return o; }
extern "C" void _$_9VIStation(void* o, int f) { trace += "D" + std::to_string(f) + (o == object_seen ? "" : "?") + ";"; }
extern "C" int Init__9VIStationiPCcPcT3T2T2i(void* o, int mode, const char* name, char* user, char* pass,
                                             const char* text, const char* copied, int launch) {
    trace += "I" + std::string(o == object_seen ? "" : "?") + ";";
    a_mode = mode; a_name = name; a_user = user; a_pass = pass; a_text = text; a_copied = copied; a_launch = launch;
    return init_result;
}
extern "C" int Process__9VIStation(void*) { trace += "P;"; return process_results[process_at++]; }
extern "C" void Free__9VIStation(void*) { trace += "F;"; }
extern "C" int sceGsSyncV(int m) { trace += "V" + std::to_string(m) + ";"; return 0; }

int main() {
    char user[8] = "u", pass[8] = "p";
    const char* t135 = "t";
    const char* copied = "c";
    init_result = -4;
    int r = station_main__FiPCcPcT2T1T1i(7, "name", user, pass, t135, copied, 9);
    if (r != -1 || trace != "[creating VIStation...\n]C;[created VIStation\n]V0;V0;I;D2;") return 1;
    if (a_mode != 7 || std::string(a_name) != "name" || a_user != user || a_pass != pass || a_text != t135
        || a_copied != copied || a_launch != 9) return 2;
    trace.clear(); init_result = 0; process_results = {0, 0, 2}; process_at = 0;
    r = station_main__FiPCcPcT2T1T1i(1, "n", user, pass, t135, copied, 0);
    if (r != 1) return 3;
    if (trace != "[creating VIStation...\n]C;[created VIStation\n]V0;V0;I;P;[station_main: process\n]P;[station_main: process\n]P;"
                 "[station_main: free\n]F;[station_main: free ok!\n]D2;") return 4;
    trace.clear(); process_results = {5}; process_at = 0;
    r = station_main__FiPCcPcT2T1T1i(1, "n", user, pass, t135, copied, 0);
    if (r != 0) return 5;
    trace.clear(); process_results = {1}; process_at = 0; init_result = 3;
    r = station_main__FiPCcPcT2T1T1i(1, "n", user, pass, t135, copied, 0);
    if (r != 0) return 6;
    return 0;
}
