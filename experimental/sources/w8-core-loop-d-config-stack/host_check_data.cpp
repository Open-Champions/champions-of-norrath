// Host contract for pushedConfig. Not an EE plan input.
#include "pushed_config.cpp"
int main() {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&pushedConfig);
    for (unsigned i = 0; i < sizeof pushedConfig; ++i) if (p[i]) return 1;
    return sizeof pushedConfig == 304 ? 0 : 2;
}
