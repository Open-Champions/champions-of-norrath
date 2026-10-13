// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp scf_local_time.cpp
extern "C" {
static int zone_value, summer_value, order, zone_at, summer_at, adjust_at, adjust_calls, g_minutes;
static unsigned char* g_clock;
int sceScfGetTimeZone() { zone_at = ++order; return zone_value; }
int sceScfGetSummerTime() { summer_at = ++order; return summer_value; }
void AdjustTime(unsigned char* clock, int minutes) { adjust_at = ++order; ++adjust_calls; g_clock = clock; g_minutes = minutes; }
void sceScfGetLocalTimefromRTC(unsigned char* clock);
}
int main() {
    unsigned char rec[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    struct { int zone, summer, minutes; } cases[] = {{540, 0, 0}, {540, 1, 60}, {0, 0, -540}, {-300, 0, -840}, {600, 1, 120}, {540, 2, 120}};
    for (auto& c : cases) {
        order = adjust_calls = 0; g_clock = nullptr;
        zone_value = c.zone; summer_value = c.summer;
        sceScfGetLocalTimefromRTC(rec);
        if (adjust_calls != 1 || g_clock != rec || g_minutes != c.minutes) return 1;
        if (zone_at != 1 || summer_at != 2 || adjust_at != 3) return 2;
    }
    for (int i = 0; i < 8; ++i) if (rec[i] != i + 1) return 3;  // record untouched by this routine
    return 0;
}
