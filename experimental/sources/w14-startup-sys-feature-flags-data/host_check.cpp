// Host check for sFeatureFlags. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
#include <cstdio>
#include "feature_flags_data.cpp"
int main() {
    int bad = 0;
    if (sizeof sFeatureFlags != 4) { std::puts("size"); ++bad; }
    if (sFeatureFlags != 0) { std::puts("not zero at load"); ++bad; }
    return bad;
}
