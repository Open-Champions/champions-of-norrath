// Host contract for VIGetFeatureFlags. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
#include <cstdio>
extern "C" { unsigned sFeatureFlags; }
#include "get_feature_flags.cpp"
int main() {
    int bad = 0;
    const unsigned vals[] = {0u, 1u, 0x80000000u, 0xffffffffu, 0x12345678u};
    for (unsigned v : vals) {
        sFeatureFlags = v;
        if (VIGetFeatureFlags__Fv() != v || sFeatureFlags != v) { std::printf("value %08x\n", v); ++bad; }
    }
    return bad;
}
