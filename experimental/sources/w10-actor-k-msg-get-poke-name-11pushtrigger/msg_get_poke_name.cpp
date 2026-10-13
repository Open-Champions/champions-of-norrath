// Observed msg_getPokeName__11PushTriggerPUsi at 0x001d1850, 268 bytes. Writes a text into the
// 16-bit-character buffer given as the second argument and returns 0 or 1. Steps, with
// tag(name, n) meaning findTagString__10GameObjectPci(object, name, n):
//  1. For n = 0, 1, 2, ...: take k = tag("key", n); stop the loop when it is null. Otherwise call
//     scriptStart__FPc(k); a zero result means a lock: take m = tag("lockMessage", 0); when m is
//     null store a zero character at out[0], else copy textGetString__FPc(m) into out with
//     ustrcpy__FPUsT0. Either way return 0 at once.
//  2. When every key was accepted (or there were none): when tag("message", 0) is null store a
//     zero character at out[0]; otherwise tag("message", 0) is asked a second time, its result
//     is passed through textGetString and copied into out. Return 1.
// Nothing here ran on the EE. The meaning of the tags beyond their spelling is unknown.
extern "C" {
char* findTagString__10GameObjectPci(void* self, const char* name, int index);
int scriptStart__FPc(const char* name);
unsigned short* textGetString__FPc(const char* name);
void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from);

int msg_getPokeName__11PushTriggerPUsi(void* self, unsigned short* out) {
    for (int n = 0;; ++n) {
        char* key = findTagString__10GameObjectPci(self, "key", n);
        if (!key) break;
        if (!scriptStart__FPc(key)) {
            char* lock = findTagString__10GameObjectPci(self, "lockMessage", 0);
            if (!lock) {
                out[0] = 0;
                return 0;
            }
            ustrcpy__FPUsT0(out, textGetString__FPc(lock));
            return 0;
        }
    }
    if (!findTagString__10GameObjectPci(self, "message", 0)) {
        out[0] = 0;
        return 1;
    }
    ustrcpy__FPUsT0(out, textGetString__FPc(findTagString__10GameObjectPci(self, "message", 0)));
    return 1;
}
}
