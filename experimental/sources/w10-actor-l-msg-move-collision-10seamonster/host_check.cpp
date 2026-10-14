// Host contract for msg_moveCollision__10SeaMonsterPP10GameObject; not an EE input. Neither the
// object nor the pointed-to slot may change.
#include <cstring>
#include "msg_move_collision.cpp"
int main() {
    static unsigned char o[256], ref[256];
    void* slot = o + 8; void* const slot_ref = slot;
    std::memset(o, 0x3c, sizeof o); std::memcpy(ref, o, sizeof o);
    msg_moveCollision__10SeaMonsterPP10GameObject(o, &slot);
    return std::memcmp(o, ref, sizeof o) != 0 || slot != slot_ref;
}
