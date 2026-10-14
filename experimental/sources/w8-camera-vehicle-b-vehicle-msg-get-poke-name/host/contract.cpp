// Host contract for msg_getPokeName__7VehiclePUsi. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" int msg_getPokeName__7VehiclePUsi(void* vehicle, unsigned short* name, int seat);
extern "C" { unsigned norrath_actor_view_4844a4 = 0x600dcafeu; }
static unsigned char vehicle[2300];
static unsigned room = 1, passenger = 0;
static int order = 0, room_at = 0, pass_at = 0, text_at = 0, copy_at = 0, pass_seat = -99;
static char key_seen[32];
static unsigned short drive_text[4] = {'D', 0, 0, 0}, ride_text[4] = {'R', 0, 0, 0};
static unsigned short* copied_src = nullptr;
static unsigned short* copied_dst = nullptr;
extern "C" unsigned haveRoom__7Vehicle(void* v) { room_at = ++order; return v == vehicle ? room : 99; }
extern "C" unsigned isPassenger__7Vehiclei(void* v, int seat) { pass_at = ++order; pass_seat = seat; return v == vehicle ? passenger : 99; }
extern "C" unsigned short* textGetString__FPc(char* key) {
    text_at = ++order;
    std::strncpy(key_seen, key, 31);
    return std::strcmp(key, "VehicleDrive") == 0 ? drive_text : ride_text;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { copy_at = ++order; copied_dst = d; copied_src = s; }
static void word(int off, unsigned v) { std::memcpy(vehicle + off, &v, 4); }
static unsigned word_at(int off) { unsigned v; std::memcpy(&v, vehicle + off, 4); return v; }
static int run(unsigned w1024, unsigned w2148, unsigned w956, unsigned has_room, unsigned is_pass, unsigned short* out) {
    std::memset(vehicle, 0, sizeof vehicle);
    word(1024, w1024); word(2148, w2148); word(956, w956);
    room = has_room; passenger = is_pass;
    order = room_at = pass_at = text_at = copy_at = 0; key_seen[0] = 0; copied_src = copied_dst = nullptr; pass_seat = -99;
    return msg_getPokeName__7VehiclePUsi(vehicle, out, 3);
}
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)
int main() {
    unsigned short out[8];
    EXPECT(run(1, 0, 0xffffffffu, 1, 0, out) == 0 && room_at == 0 && text_at == 0, "busy returns 0 before haveRoom");
    EXPECT(word_at(400) == 0x600dcafeu, "frameNm stored first");
    EXPECT(run(0, 0, 0xffffffffu, 0, 0, out) == 0 && room_at == 1 && pass_at == 0 && text_at == 0, "no room");
    EXPECT(run(0, 0, 0xffffffffu, 1, 5, out) == 0 && pass_at == 2 && pass_seat == 3 && text_at == 0, "already passenger");
    EXPECT(run(0, 0, 0xffffffffu, 1, 0, out) == 2, "drive result");
    EXPECT(std::strcmp(key_seen, "VehicleDrive") == 0 && copied_src == drive_text && copied_dst == out && text_at == 3 && copy_at == 4, "drive text");
    EXPECT(run(0, 1, 0xffffffffu, 1, 0, out) == 2 && std::strcmp(key_seen, "VehicleRide") == 0 && copied_src == ride_text, "2148 nonzero rides");
    EXPECT(run(0, 0, 0, 1, 0, out) == 2 && std::strcmp(key_seen, "VehicleRide") == 0, "956 not -1 rides");
    EXPECT(run(0, 0, 5, 1, 0, out) == 2 && std::strcmp(key_seen, "VehicleRide") == 0, "956 other rides");
    return 0;
}
