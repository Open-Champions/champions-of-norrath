// Host check for netGetCameraCustomControl. Not an EE plan input.
#include <initializer_list>
#include "net_get_camera_custom_control.cpp"
signed char netNetworkMode;
unsigned char* gameCam;
unsigned short netCameraYaw[4];
int main() {
    alignas(8) static unsigned char camera[64];
    gameCam = camera;
    netNetworkMode = 0;
    for (int v : {0, 1, 2, -1, 0x10000, 3}) {
        *reinterpret_cast<int*>(camera + 52) = v;
        if (netGetCameraCustomControl__Fi(2) != (v != 1)) return 1;
    }
    netNetworkMode = 1;
    unsigned short values[4] = {0x0000, 0x0001, 0xfffe, 0xffff};
    for (int i = 0; i < 4; ++i) netCameraYaw[i] = values[i];
    for (int i = 0; i < 4; ++i)
        if (netGetCameraCustomControl__Fi(i) != (values[i] & 1)) return 2;
    *reinterpret_cast<int*>(camera + 52) = 1;  // camera word ignored in net mode
    if (netGetCameraCustomControl__Fi(1) != 1 || netGetCameraCustomControl__Fi(0) != 0) return 3;
    netNetworkMode = -1;  // any nonzero byte selects the array
    if (netGetCameraCustomControl__Fi(3) != 1) return 4;
    return 0;
}
