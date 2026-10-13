// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
static unsigned char scratch[0x4000];
extern "C" volatile unsigned char* norrath_host_scratchpad_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
#include "raster-enable-point-light.cpp"

int main() {
    alignas(8) unsigned char raster[19000];
    std::memset(raster, 0x44, sizeof raster);
    std::memset(scratch, 0x77, sizeof scratch);
    if (EnablePointLight__8VIRasterii(raster, 2, 0x1ff) != 0) return 1;
    if (scratch[0x222 + 2] != 0xff) return 2;
    int flag; std::memcpy(&flag, raster + 17888, 4);
    if (flag != 1) return 3;
    if (raster[17884] != 0x44 || raster[17892] != 0x44) return 4;
    if (scratch[0x222] != 0x77 || scratch[0x222 + 1] != 0x77 || scratch[0x222 + 3] != 0x77) return 5;
    if (EnablePointLight__8VIRasterii(raster, 0, 0) != 0) return 6;
    if (scratch[0x222] != 0) return 7;
    return 0;
}
