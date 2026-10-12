// Observed msg_getPokeName__7VehiclePUsi at 0x2778e0, extent 204.
// The word at 0x4844a4 (frameNm, named by the existing authored views) is
// stored to vehicle+400 before any test. Then, in order:
//   word 1024 nonzero            -> return 0
//   haveRoom__7Vehicle == 0      -> return 0
//   isPassenger__7Vehiclei(seat) nonzero -> return 0
//   otherwise a label is copied into the caller's UTF-16 buffer and 2 is
//   returned. The label is the text-table entry "VehicleDrive" when word 2148
//   is zero and word 956 is -1, and "VehicleRide" in every other case.
// The strings are 13 and 12 bytes at 0x475f80 and 0x475f90.
// Unknown: what words 1024, 2148 and 956 mean.
extern "C" unsigned norrath_actor_view_4844a4;
extern "C" unsigned haveRoom__7Vehicle(void* vehicle);
extern "C" unsigned isPassenger__7Vehiclei(void* vehicle, int seat);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
const char drive_key[] = "VehicleDrive";
const char ride_key[] = "VehicleRide";
}

extern "C" int msg_getPokeName__7VehiclePUsi(void* vehicle, unsigned short* name, int seat) {
    unsigned char* v = static_cast<unsigned char*>(vehicle);
    *reinterpret_cast<AliasedWord*>(v + 400) = norrath_actor_view_4844a4;
    if (*reinterpret_cast<AliasedWord*>(v + 1024) != 0u) return 0;
    if (haveRoom__7Vehicle(vehicle) == 0u) return 0;
    if (isPassenger__7Vehiclei(vehicle, seat) != 0u) return 0;
    const char* key = ride_key;
    if (*reinterpret_cast<AliasedWord*>(v + 2148) == 0u && *reinterpret_cast<AliasedWord*>(v + 956) == 0xffffffffu)
        key = drive_key;
    ustrcpy__FPUsT0(name, textGetString__FPc(const_cast<char*>(key)));
    return 2;
}
