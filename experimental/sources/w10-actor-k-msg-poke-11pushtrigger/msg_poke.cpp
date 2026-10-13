// Observed msg_poke__11PushTriggeri at 0x001d1960, 148 bytes. The integer argument is unused.
// With tagStr(name) and tagInt(name) meaning findTagString__10GameObjectPci and
// findTagInt__10GameObjectPci called with the object, the name and 0: when tagStr("action") is
// non-null, objectDoMyActions__FP10GameObjecti is called with the object and 1 if
// tagInt("offOnly") is zero, else 0. Then, when tagInt("once") is nonzero (and the object pointer
// is non-null, which the original tests), the table entry at (word +72) + 8 is called on the
// object adjusted by the entry's signed halfword, with the argument 3. Nothing here ran on the
// EE. What the tags and the entry mean is unknown.
#include "../w10-actor-k-common/access.hpp"

extern "C" {
char* findTagString__10GameObjectPci(void* self, const char* name, int index);
int findTagInt__10GameObjectPci(void* self, const char* name, int index);
void objectDoMyActions__FP10GameObjecti(void* self, int flag);

void msg_poke__11PushTriggeri(void* self, int) {
    if (findTagString__10GameObjectPci(self, "action", 0)) {
        objectDoMyActions__FP10GameObjecti(self, findTagInt__10GameObjectPci(self, "offOnly", 0) == 0);
        if (findTagInt__10GameObjectPci(self, "once", 0) && self)
            w10k::call_entry(static_cast<unsigned char*>(self), 8, 3);
    }
}
}
