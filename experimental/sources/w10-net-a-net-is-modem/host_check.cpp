// Host check for netIsModem. Not an EE plan input.
#include "net_is_modem.cpp"
signed char isModem;
int main() {
    for (int v = -128; v < 128; ++v) {
        isModem = static_cast<signed char>(v);
        if (netIsModem__Fv() != v) return 1;  // sign extension is part of the result
    }
    return 0;
}
