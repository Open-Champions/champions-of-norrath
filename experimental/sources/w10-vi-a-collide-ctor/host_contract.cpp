#include "ctor.cpp"

static void* pool_seen;
static int pool_calls;
extern "C" void __t6VIPool1ZP12VICollBuffer(void* pool) {
    pool_seen = pool;
    ++pool_calls;
}

int main() {
    unsigned char object[600];
    for (unsigned char& byte : object) byte = 0xa5;
    __9VICollide(object);
    if (pool_calls != 1 || pool_seen != object + 528) return 1;
    for (int offset = 0; offset < 600; ++offset) {
        bool zeroed = (offset >= 16 && offset < 144) || (offset >= 144 && offset < 528);
        if (zeroed ? object[offset] != 0 : object[offset] != 0xa5) return 2 + (offset & 0xff) % 3;
    }
    return 0;
}
