// Observed __cp_push_exception at 0x002fef80, 144 bytes. Registers a thrown C++ object. It
// takes the object, its type_info and its destructor, allocates a 48-byte record with
// __eh_alloc(48) (0x002fa4a8) and fills it:
//   +0  the address of __cplus_type_matcher (0x002fef20)
//   +4  halfword 4         +6  halfword 1
//   +8  the object         +12 the type_info        +16 the destructor
//   +20 zero               +24 the previous head of the exception chain
//   +32 zero (64-bit)      +40 the object again
// (+28 and +36 are not written.) The chain head is the word __get_eh_info() (0x002fa108) points
// at: its current value goes to +24 and the new record replaces it. Nothing is returned.
// Inferred only: that this is the C++ exception chain, from the callee names. Unknown: what the
// halfwords and the second object word mean. Nothing was executed on the EE.
extern "C" {
// Words are 32 bits wide on the target; addresses are held as unsigned words here.
struct CpException {
    unsigned matcher;
    short first;
    short second;
    unsigned object;
    unsigned type;
    unsigned destructor;
    int zero20;
    unsigned previous;
    int unwritten28;
    long long zero32;
    unsigned object_again;
    int unwritten44;
};

void* __eh_alloc(unsigned size);
unsigned* __get_eh_info();
void __cplus_type_matcher();

static inline unsigned address_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void __cp_push_exception(void* object, void* type, void (*destructor)()) {
    CpException* record = static_cast<CpException*>(__eh_alloc(48));
    record->second = 1;
    record->first = 4;
    record->matcher = address_word(reinterpret_cast<void*>(__cplus_type_matcher));
    record->type = address_word(type);
    record->destructor = address_word(reinterpret_cast<void*>(destructor));
    record->object_again = address_word(object);
    record->object = address_word(object);
    record->zero32 = 0;
    record->zero20 = 0;
    unsigned* head = __get_eh_info();
    record->previous = *head;
    *head = address_word(record);
}
}
