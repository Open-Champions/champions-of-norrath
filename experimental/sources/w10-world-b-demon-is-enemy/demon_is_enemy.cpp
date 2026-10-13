// Observed isEnemy__5DemonP10GameObject at 0x001eb458, 132 bytes in SLUS_205.65. Arguments:
// the Demon and another object. First asks the Creature base, isEnemy__8CreatureP10GameObject,
// and returns its answer when that is nonzero. With a null object the answer is the base's
// (zero). Otherwise the other object's runtime type is read by calling the first function
// of the table referenced by the object's word at +72 (called with no argument, as in the
// other type-comparing callers) and compared with __tf4Soul() through
// __eq__C9type_infoRC9type_info; when they are the same and bit 0x08 of the object's flag
// word at +12 is clear, the answer is 3. In every other case it is 0.
// Inferred: the table word at +72 is the virtual table and its slot at byte offset 4 is the
// type-descriptor function (the slot used the same way by msg_run__7Trigger). Unknown: what
// result 3 means to callers, and what flag 0x08 stands for (the same bit hideHiddenObjects
// and the Skeleton version test).
extern "C" int isEnemy__8CreatureP10GameObject(unsigned char* self, unsigned char* other);
extern "C" void* __tf4Soul();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);

extern "C" int isEnemy__5DemonP10GameObject(unsigned char* self, unsigned char* other) {
    int result = isEnemy__8CreatureP10GameObject(self, other);
    if (result != 0) return result;
    if (other == 0) return result;
    unsigned char* table = *reinterpret_cast<unsigned char**>(other + 72);
    void* type = (*reinterpret_cast<void* (**)()>(table + 4))();
    if (__eq__C9type_infoRC9type_info(type, __tf4Soul()) != 0) {
        if ((*reinterpret_cast<unsigned*>(other + 12) & 0x8) == 0) result = 3;
    }
    return result;
}
