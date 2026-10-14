// Host contract for coreInterfaceFlare. Not an EE plan input.
#include "interface_flare.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" float coreInterfaceBrightness[4] = {1.0f, 3.0f, 4.0f, 5.0f};

static int failures = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::uint32_t bits(float value) {
    std::uint32_t out = 0;
    std::memcpy(&out, &value, 4);
    return out;
}

int main() {
    coreInterfaceFlare__Fi(0);
    expect(bits(coreInterfaceBrightness[0]) == 0x40000000u, "index 0 stores 2.0f");
    expect(bits(coreInterfaceBrightness[1]) == bits(3.0f), "index 0 leaves slot 1");
    expect(bits(coreInterfaceBrightness[2]) == bits(4.0f), "index 0 leaves slot 2");
    expect(bits(coreInterfaceBrightness[3]) == bits(5.0f), "index 0 leaves slot 3");

    coreInterfaceBrightness[0] = 1.0f;
    coreInterfaceFlare__Fi(3);
    expect(bits(coreInterfaceBrightness[3]) == 0x40000000u, "index 3 stores 2.0f");
    expect(bits(coreInterfaceBrightness[0]) == bits(1.0f), "index 3 leaves slot 0");
    expect(bits(coreInterfaceBrightness[2]) == bits(4.0f), "index 3 leaves slot 2");

    coreInterfaceFlare__Fi(1);
    expect(bits(coreInterfaceBrightness[1]) == 0x40000000u, "index 1 stores 2.0f");
    expect(bits(coreInterfaceBrightness[2]) != 0x40000000u, "index 1 does not store into slot 2");

    if (failures != 0) return 1;
    std::puts("coreInterfaceFlare contract passed");
    return 0;
}
