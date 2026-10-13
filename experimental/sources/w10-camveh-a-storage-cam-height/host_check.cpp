// Host contract for the camera height storage. Not an EE plan input.
#include "storage.cpp"

#include <cstring>

int main() {
    const unsigned char wantMin[4] = {0x00, 0x50, 0xc3, 0xc7};
    const unsigned char wantMax[4] = {0x00, 0x50, 0xc3, 0x47};
    if (std::memcmp(&gameMinCamHeight, wantMin, 4) != 0) return 1;
    if (std::memcmp(&gameMaxCamHeight, wantMax, 4) != 0) return 2;
    return 0;
}
