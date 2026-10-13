// Host contract for msg_presave__8RollDoor; not an EE input. The object must stay untouched.
#include <cstring>
#include "msg_presave.cpp"
int main() {
    static unsigned char o[256], ref[256];
    std::memset(o, 0xa5, sizeof o); std::memcpy(ref, o, sizeof o);
    msg_presave__8RollDoor(o);
    return std::memcmp(o, ref, sizeof o) != 0;
}
