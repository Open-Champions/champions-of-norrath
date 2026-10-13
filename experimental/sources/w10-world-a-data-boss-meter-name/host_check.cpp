#include "boss_meter_name.cpp"
#include <cstring>
int main() {
    if (sizeof(bossMeterName) != 128) return 1;
    for (unsigned i = 0; i < sizeof(bossMeterName); i++) if (((const unsigned char*)&bossMeterName[0])[i] != 0) return 2;
    return 0;
}
