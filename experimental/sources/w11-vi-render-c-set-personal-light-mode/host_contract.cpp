#include "set_personal_light_mode.cpp"

#include <cstring>

int main() {
    alignas(4) unsigned char object[10800];
    const int modes[] = {0, 1, 2, -1, 0x7fffffff};
    for (int mode : modes) {
        std::memset(object, 0x5a, sizeof object);
        if (SetPersonalLightMode__7VIScene19VIPersonalLightMode(reinterpret_cast<VIScene*>(object), mode) != 0)
            return 1;
        int stored;
        std::memcpy(&stored, object + 10772, 4);
        if (stored != mode) return 2;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 10772 || i >= 10776) && object[i] != 0x5a) return 3;
    }
    return 0;
}
