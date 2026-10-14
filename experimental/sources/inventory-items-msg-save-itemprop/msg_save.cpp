// Observed msg_save__8ItemPropPc at 0x155750, 180 bytes.
// The item description lives at object+328 (72 bytes). The function calls
// item::pack on it, copies those 72 bytes to the start of the buffer, calls
// item::unpack on it, then calls Prop::msg_save with the object and the buffer
// plus 72. It returns 72 plus what Prop::msg_save returned.

#include "../inventory-items-object/object.hpp"

extern "C" {
void pack__4itemPQ24item11description(void* description);
void unpack__4itemPQ24item11description(void* description);
int msg_save__4PropPc(void* object, unsigned char* buffer);
}

extern "C" int msg_save__8ItemPropPc(void* object_storage, unsigned char* buffer) {
    using namespace inventory_object;
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    pack__4itemPQ24item11description(object + 328);
    copy_bytes(buffer, object + 328, 72);
    unpack__4itemPQ24item11description(object + 328);
    return 72 + msg_save__4PropPc(object, buffer + 72);
}
