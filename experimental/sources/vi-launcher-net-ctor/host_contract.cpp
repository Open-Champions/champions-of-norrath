#define memset norrath_host_memset
#include "net_ctor.cpp"
#undef memset

static int calls;
static void* seen_destination[2];
static int seen_value[2];
static unsigned seen_count[2];

extern "C" void* norrath_host_memset(void* destination, int value, unsigned count) {
    if (calls < 2) {
        seen_destination[calls] = destination;
        seen_value[calls] = value;
        seen_count[calls] = count;
    }
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    for (unsigned index = 0; index < count; ++index) bytes[index] = static_cast<unsigned char>(value);
    ++calls;
    return destination;
}

int main() {
    unsigned char object[108];
    for (unsigned char& byte : object) byte = 0xa5;
    __15VINetworkStatus(reinterpret_cast<VINetworkStatus*>(object));
    if (calls != 2) return 1;
    if (seen_destination[0] != object || seen_value[0] != 0 || seen_count[0] != 60) return 2;
    if (seen_destination[1] != object + 64 || seen_value[1] != 0 || seen_count[1] != 40) return 3;
    for (unsigned char byte : object) {
        if (byte != 0) return 4;
    }
    return 0;
}
