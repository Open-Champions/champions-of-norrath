// Observed what__C9exception at 0x002ff468, 64 bytes. A null object calls __throw_bad_typeid
// (0x002ff3c8) and the code after the call reads a word through that call's return register,
// the same read as the normal path. Otherwise the object's first word is its table address;
// the function pointer at table offset 4 is called with the object and returns a pointer to
// a record whose first word is a character pointer. That character pointer is the result.
// This is the shape of a type-name query on the object's dynamic type; the table slot's own
// name is not established. __throw_bad_typeid is declared as returning the record pointer so
// the shared read is kept; whether it ever returns is unknown.
extern "C" {
typedef const char* const* (*TypeRecordSlot)(const void* self);

const char* const* throw_bad_typeid_result() __asm__("__throw_bad_typeid");

const char* what__C9exception(const void* self) {
    const char* const* record;
    if (self == nullptr) {
        record = throw_bad_typeid_result();
    } else {
        const TypeRecordSlot* table = *static_cast<const TypeRecordSlot* const*>(self);
        record = table[1](self);
    }
    return *record;
}
}
