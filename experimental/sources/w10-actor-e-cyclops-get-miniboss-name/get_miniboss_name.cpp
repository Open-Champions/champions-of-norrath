#include "../vslots-b-bytes/bytes.hpp"
// Observed getMinibossName__7Cyclops at 0x203c98, 60 bytes.
// Formats the signed byte at object+76 with the text format "MBCyclops%d" into a
// 16-byte buffer that is private to this function (a function-local static in the original)
// and returns that buffer. Later calls overwrite earlier results.
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" char* getMinibossName__7Cyclops(void* self) {
    using namespace vslots_b;
    static char buffer[16];
    const unsigned raw = read_u8(address(self) + 76u);
    sprintf(buffer, "MBCyclops%d", raw < 0x80u ? static_cast<int>(raw) : static_cast<int>(raw) - 256);
    return buffer;
}
