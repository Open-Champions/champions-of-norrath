// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp   (the source is included)
// The extent is the distance between two recorded addresses of the original (sifParamFname at
// 0x008e6bf0, sifParamNext at 0x008e7040) and must hold the 1044-byte send area the memory-card calls use.
namespace {
constexpr unsigned long kStart = 0x008e6bf0ul;
constexpr unsigned long kNext = 0x008e7040ul;
constexpr unsigned long kSendArea = 1044;
static_assert(kStart % 16 == 0, "the original object is 16-byte aligned");
static_assert(kNext - kStart == 1104, "recorded span");
static_assert(kNext - kStart >= kSendArea, "the send area fits");
}
#include "sif_param_fname.cpp"
static_assert(sizeof(sifParamFname) == kNext - kStart, "object size equals the recorded span");
static_assert(sizeof(sifParamFname) >= kSendArea, "object holds the send area");
static_assert(__alignof__(sifParamFname) >= 16, "alignment holds the original address");
int main() {
    for (unsigned char byte : sifParamFname) if (byte != 0) return 1;
    sifParamFname[kSendArea - 1] = 0;     // the byte the callers clear is inside the object
    return 0;
}
