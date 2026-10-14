#include "set_personal_light.cpp"

#include <cstring>

int main() {
    alignas(8) unsigned char object[10840];
    std::memset(object, 0x5a, sizeof object);
    VIVect3 position = {1.5f, -2.25f, 3.125f};
    alignas(8) unsigned char raw[24];
    for (int i = 0; i < 24; ++i) raw[i] = static_cast<unsigned char>(0x10 + i);
    const VIColor32F* color = reinterpret_cast<const VIColor32F*>(raw + 3);  // deliberately unaligned
    if (SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F(reinterpret_cast<VIScene*>(object), &position,
                                                            9.5f, color) != 0)
        return 1;
    float stored[4];
    std::memcpy(stored, object + 10776, 16);
    if (stored[0] != 1.5f || stored[1] != -2.25f || stored[2] != 3.125f || stored[3] != 9.5f) return 2;
    if (std::memcmp(object + 10792, raw + 3, 16) != 0) return 3;
    for (unsigned i = 0; i < sizeof object; ++i)
        if ((i < 10776 || i >= 10808) && object[i] != 0x5a) return 4;
    return 0;
}
