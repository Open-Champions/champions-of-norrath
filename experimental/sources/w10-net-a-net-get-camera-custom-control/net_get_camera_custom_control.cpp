// Observed netGetCameraCustomControl__Fi at 0x00135378, 48 bytes. With a zero
// netNetworkMode byte it ignores the argument and returns 1 unless the word at
// offset 52 of the object gameCam points to equals 1. Otherwise it returns bit 0
// of the unsigned halfword netCameraYaw[player]; the argument is not range
// checked.
// Inferred: gameCam is the camera object and the word at 52 is a mode value the
// camera code compares with 1; netCameraYaw is an array of four halfwords (the
// image gives the symbol 8 bytes of room before kickState).
// Unknown: who writes netCameraYaw and what the other 15 bits hold; the name of
// the word at offset 52.
extern "C" {
extern signed char netNetworkMode;
extern unsigned char* gameCam;
extern unsigned short netCameraYaw[];
}

extern "C" int netGetCameraCustomControl__Fi(int player) {
    if (netNetworkMode == 0) return *reinterpret_cast<int*>(gameCam + 52) != 1;
    return netCameraYaw[player] & 1;
}
