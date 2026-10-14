#define printf norrath_host_printf
#include "net_check.cpp"
#undef printf

static int prints;
static int calls;
static int scripted_result[4];
static int scripted_state[4];
static int seen_id[4];
static void* seen_buffer[4];
static int* seen_state_slot[4];
static unsigned char* active;

static int shrink_count;

extern "C" int norrath_host_printf(const char* text, ...) {
    if (text[0] != 'w' || text[1] != 'h' || text[2] != 'o' || text[3] != 'b' || text[4] != '?') return 1;
    ++prints;
    return 0;
}

extern "C" int sceInetCtlGetState(VINetworkStatus* object, void* buffer, int id, int* state) {
    (void)object;
    if (calls < 4) {
        seen_id[calls] = id;
        seen_buffer[calls] = buffer;
        seen_state_slot[calls] = state;
    }
    if (state == nullptr || *state != 0) return 50;
    int result = scripted_result[calls];
    *state = scripted_state[calls];
    if (shrink_count != 0) *reinterpret_cast<int*>(active + 104) = 1;
    ++calls;
    return result;
}

int main() {
    unsigned char object[108] = {};
    active = object;
    auto* status = reinterpret_cast<VINetworkStatus*>(object);
    if (CheckNetwork__15VINetworkStatus(status) != 2 || calls != 0) return 1;
    *reinterpret_cast<unsigned*>(object + 60) = 0x40u;
    *reinterpret_cast<int*>(object + 104) = 0;
    if (CheckNetwork__15VINetworkStatus(status) != 1 || calls != 0) return 2;
    *reinterpret_cast<int*>(object + 104) = -2;
    if (CheckNetwork__15VINetworkStatus(status) != 1 || calls != 0) return 3;

    *reinterpret_cast<int*>(object + 104) = 2;
    *reinterpret_cast<int*>(object + 64) = 11;
    *reinterpret_cast<int*>(object + 68) = 12;
    scripted_result[0] = 1;
    scripted_state[0] = 3;
    scripted_result[1] = 0;
    scripted_state[1] = 1;
    prints = 0;
    if (CheckNetwork__15VINetworkStatus(status) != 1) return 4;
    if (prints != 2 || calls != 2) return 5;
    if (seen_id[0] != 11 || seen_id[1] != 12) return 6;
    if (seen_buffer[0] != reinterpret_cast<void*>(0x40u)) return 7;
    if (seen_state_slot[0] == nullptr) return 8;

    *reinterpret_cast<int*>(object + 104) = 2;
    shrink_count = 1;
    calls = 0;
    prints = 0;
    if (CheckNetwork__15VINetworkStatus(status) != 1) return 11;
    if (calls != 1 || prints != 1) return 12;
    shrink_count = 0;

    *reinterpret_cast<int*>(object + 104) = 2;
    scripted_result[0] = 0;
    scripted_state[0] = 3;
    calls = 0;
    prints = 0;
    if (CheckNetwork__15VINetworkStatus(status) != 0) return 9;
    if (calls != 1 || prints != 1) return 10;
    return 0;
}
