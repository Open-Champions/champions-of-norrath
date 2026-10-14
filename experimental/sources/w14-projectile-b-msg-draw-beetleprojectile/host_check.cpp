// Host contract for msg_draw__16beetleProjectile; not an EE input. The function must leave the object alone. The authored function
// was also compared with the original in a private R5900 run (four scenarios with random memory: no changed bytes outside the stack).
#include "msg_draw.cpp"
#include <cstring>
int main() {
    unsigned char object[64], copy[64];
    for (int i = 0; i < 64; ++i) object[i] = static_cast<unsigned char>(i * 3 + 1);
    std::memcpy(copy, object, 64);
    msg_draw__16beetleProjectile(object);
    return std::memcmp(object, copy, 64) == 0 ? 0 : 1;
}
