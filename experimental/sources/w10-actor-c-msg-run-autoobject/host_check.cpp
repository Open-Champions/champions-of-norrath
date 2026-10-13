#include <cstdio>
#include <cstring>
extern "C" void msg_run__10AutoObject(void*);
int main() {
    unsigned char object[256];
    unsigned char copy[256];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = copy[i] = static_cast<unsigned char>(i * 7 + 1);
    msg_run__10AutoObject(object);
    if (std::memcmp(object, copy, sizeof object) != 0) {
        std::fprintf(stderr, "fail: object changed\n");
        return 1;
    }
    return 0;
}
