// Observed isBoss__FP10GameObject at 0x0010b1c8, 732 bytes in SLUS_205.65.
// Answers whether an object is a boss, as 0 or 1:
//  * bit 0x20 of the flag word at +12 clear: 0;
//  * the 64-bit word at +160 having its lowest bit set: 1;
//  * otherwise the object's runtime type is read (the first function of the table referenced
//    by the word at +72, called with no argument, after a null check that calls
//    __throw_bad_typeid) and compared, in this order, with the types of AntQueen,
//    Arenabeast, CloudGiant, Innoruuk, Minion, MummyKing, SpiderQueen, UndeadKnight and
//    VampireLord, each through __eq__C9type_infoRC9type_info(type, __tf<Name>()), with the type
//    read again before every comparison. A match answers 1, except that CloudGiant answers 1
//    only when the word at +976 is 1 (otherwise the checks go on). The last comparison's
//    result is the answer: 1 for VampireLord, else 0.
// Inferred: the table word at +72 is the virtual table and its slot at byte offset 4 the type
// function; the names are the bosses of those classes, as the function's name suggests.
// Unknown: the meaning of flag 0x20, of the bit at +160 and of the word at +976 on CloudGiant.
extern "C" void __throw_bad_typeid();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);
extern "C" void* __tf8AntQueen();
extern "C" void* __tf10Arenabeast();
extern "C" void* __tf10CloudGiant();
extern "C" void* __tf8Innoruuk();
extern "C" void* __tf6Minion();
extern "C" void* __tf9MummyKing();
extern "C" void* __tf11SpiderQueen();
extern "C" void* __tf12UndeadKnight();
extern "C" void* __tf11VampireLord();

namespace {
void* type_of(unsigned char* object) {
    if (object == 0) __throw_bad_typeid();
    unsigned char* table = *reinterpret_cast<unsigned char**>(object + 72);
    return (*reinterpret_cast<void* (**)()>(table + 4))();
}
}

extern "C" int isBoss__FP10GameObject(unsigned char* object) {
    if ((*reinterpret_cast<unsigned*>(object + 12) & 0x20) == 0) return 0;
    if ((*reinterpret_cast<unsigned long long*>(object + 160) & 1) != 0) return 1;
    void* type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf8AntQueen()) != 0) return 1;
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf10Arenabeast()) != 0) return 1;
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf10CloudGiant()) != 0) {
        if (*reinterpret_cast<int*>(object + 976) == 1) return 1;
    }
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf8Innoruuk()) != 0) return 1;
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf6Minion()) != 0) return 1;
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf9MummyKing()) != 0) return 1;
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf11SpiderQueen()) != 0) return 1;
    type = type_of(object);
    if (__eq__C9type_infoRC9type_info(type, __tf12UndeadKnight()) != 0) return 1;
    type = type_of(object);
    return __eq__C9type_infoRC9type_info(type, __tf11VampireLord()) != 0;
}
