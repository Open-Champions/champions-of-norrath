// Observed msg_save__4GoldPc at 0x154d80, 24 bytes.
// Stores the word at object+128 at the start of the buffer and returns 4.

#include "../inventory-items-object/object.hpp"

extern "C" int msg_save__4GoldPc(void* object_storage, unsigned char* buffer) {
    using namespace inventory_object;
    write_word(buffer, read_word(static_cast<const unsigned char*>(object_storage) + 128));
    return 4;
}
