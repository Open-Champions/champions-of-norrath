#include "msg_trigger.cpp"
int main() {
    unsigned char object[16];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i + 1);
    msg_trigger__10GameObjecti(object, 5);
    for (unsigned i = 0; i < sizeof object; ++i) if (object[i] != i + 1) return 1;
    return 0;
}
