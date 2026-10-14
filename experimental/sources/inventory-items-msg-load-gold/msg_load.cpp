// Observed msg_load__4GoldPc at 0x154d98, 24 bytes.
// Stores the first word of the buffer at object+128 and returns 4.

#include "../inventory-items-object/object.hpp"

extern "C" int msg_load__4GoldPc(void* object_storage, const unsigned char* buffer) {
    using namespace inventory_object;
    write_word(static_cast<unsigned char*>(object_storage) + 128, read_word(buffer));
    return 4;
}
