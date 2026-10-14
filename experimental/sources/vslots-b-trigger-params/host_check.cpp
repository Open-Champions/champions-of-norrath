#define NORRATH_VSLOTS_B_HOST 1
#include "trigger_params.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[80];
    unsigned char target[108];
    std::memset(object, 0x15, sizeof object);
    std::memset(target, 0x26, sizeof target);
    vslots_b::write_word(vslots_b::address(object) + 76u, 0u);
    msg_trigger__13TriggerParamsi(object, 0);
    if (target[100] != 0x26) return 1;
    vslots_b::host_note(target);
    vslots_b::write_word(vslots_b::address(object) + 76u, vslots_b::address_bits(target));
    msg_trigger__13TriggerParamsi(object, 3);
    if (vslots_b::read_word(vslots_b::address(target) + 100u) != 0u) return 1;
    if (target[99] != 0x26 || target[104] != 0x26) return 1;
    msg_trigger__13TriggerParamsi(object, 0);
    if (vslots_b::read_word(vslots_b::address(target) + 100u) != 0xffffffffu) return 1;
    return 0;
}
