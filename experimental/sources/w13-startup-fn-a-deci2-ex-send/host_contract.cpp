// Host contract for sceDeci2ExSend. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// Deci2Call is scripted: it records the service number and the three argument words and returns a marker.
#include <cstdio>
static int seenService;
static __UINTPTR_TYPE__ seen[3];
extern "C" int Deci2Call(int service, void* arguments) {
    seenService = service;
    for (int i = 0; i < 3; ++i) seen[i] = static_cast<__UINTPTR_TYPE__*>(arguments)[i];
    return 0x1234 + service;
}
#include "deci2_ex_send.cpp"
int main() {
    int bad = 0;
    int buffer[4];
    const int sockets[] = {0, 7, -1};
    const unsigned lengths[] = {0u, 1u, 0xffffu, 0x10000u, 0x12345u, 0xffffffffu};
    for (int socket : sockets) {
        for (unsigned length : lengths) {
            seenService = 0;
            const int result = sceDeci2ExSend(socket, buffer, length);
            if (seenService != -6) { std::printf("service %d\n", seenService); ++bad; }
            if (result != 0x1234 + (-6)) { std::printf("result %x\n", result); ++bad; }
            if (static_cast<int>(seen[0]) != socket) { std::printf("socket word\n"); ++bad; }
            if (seen[1] != reinterpret_cast<__UINTPTR_TYPE__>(buffer)) { std::printf("buffer word\n"); ++bad; }
            if (seen[2] != (length & 0xffffu)) { std::printf("length word %lx for %x\n", (unsigned long)seen[2], length); ++bad; }
        }
    }
    return bad;
}
