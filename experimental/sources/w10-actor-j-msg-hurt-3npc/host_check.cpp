// Host contract for msg_hurt__3NPCfP10GameObjecti; not an EE input. The object bytes
// must be unchanged after the call.
#include "msg_hurt.cpp"
#include <cstring>
int main() {
    alignas(16) static unsigned char object[2048], copy[2048];
    for (unsigned i = 0; i < sizeof object; ++i) copy[i] = object[i] = static_cast<unsigned char>(i * 7);
    msg_hurt__3NPCfP10GameObjecti(object, 12.5f, object + 64, 3);
    return std::memcmp(object, copy, sizeof object) != 0;
}
