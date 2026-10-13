// Host contract for msg_draw__9Generator. Includes the source; not an EE input.
#include "msg_draw.cpp"
int main() {
    unsigned char object[1024];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i ^ 0x5a);
    msg_draw__9Generator(object);
    for (unsigned i = 0; i < sizeof object; ++i)
        if (object[i] != static_cast<unsigned char>(i ^ 0x5a)) return 1;
    return 0;
}
