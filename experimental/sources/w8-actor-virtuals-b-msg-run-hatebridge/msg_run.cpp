// Observed msg_run__10HateBridge at 0x00226468, 124 bytes.
// Remembers the flag word at object+280, runs the AutoProp run method, then asks
// objectActiveProcessing about the object with the (re-read) word at +280 and the
// constant 1000. The word becomes 1 when that call returned nonzero, else 0.
// Previously clear and now set: objectAlwaysDraw. Previously set and now clear:
// objectCancelAllAlwaysDraw. Unchanged: nothing more. Returns no value of its own.
// The meaning of the 1000 argument and of word +280 is not established here.

extern "C" {
void msg_run__8AutoProp(void* object);
int objectActiveProcessing__FP10GameObjectii(void* object, int flag, int value);
void objectAlwaysDraw__FP10GameObject(void* object);
void objectCancelAllAlwaysDraw__Fv();
}

namespace {
typedef int Word __attribute__((may_alias));
}

extern "C" void msg_run__10HateBridge(void* self) {
    Word* flag = reinterpret_cast<Word*>(static_cast<unsigned char*>(self) + 280);
    int before = *flag;
    msg_run__8AutoProp(self);
    int active = objectActiveProcessing__FP10GameObjectii(self, *flag, 1000) != 0;
    *flag = active;
    if (before != 0) {
        if (active == 0) objectCancelAllAlwaysDraw__Fv();
    } else if (active != 0) {
        objectAlwaysDraw__FP10GameObject(self);
    }
}
