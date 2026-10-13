// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
static unsigned char scratch[0x4000];
extern "C" unsigned char* norrath_host_scratchpad_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
#include "raster-set-point-light.cpp"

static const void* seen_matrix;
static const void* seen_in;
static void* seen_out;
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* in, void* out) {
    seen_matrix = matrix; seen_in = in; seen_out = out;
    float* o = static_cast<float*>(out);
    o[0] = 91.0f; o[1] = 92.0f; o[2] = 93.0f;
}

int main() {
    alignas(8) unsigned char raster[19000];
    alignas(8) unsigned char matrix[64];
    alignas(8) unsigned char light[40];
    std::memset(raster, 0x44, sizeof raster);
    std::memset(scratch, 0x77, sizeof scratch);
    for (int i = 0; i < 40; ++i) light[i] = static_cast<unsigned char>(i + 1);
    if (SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight(raster, 3, matrix, light + 0) != 0) return 1;
    unsigned char* slot = scratch + 0x1c0 + 3 * 32;
    if (seen_matrix != matrix || seen_in != light || seen_out != slot) return 2;
    float v[3]; std::memcpy(v, slot, 12);
    if (v[0] != 91.0f || v[1] != 92.0f || v[2] != 93.0f) return 3;
    if (std::memcmp(slot + 12, light + 12, 4) != 0) return 4;
    if (std::memcmp(slot + 16, light + 16, 16) != 0) return 5;
    if (slot[-1] != 0x77 || slot[32] != 0x77) return 6;
    int flag; std::memcpy(&flag, raster + 17888, 4);
    if (flag != 1) return 7;
    return 0;
}
