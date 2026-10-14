// Observed __deregister_frame_info at 0x002fc558, 188 bytes.
// Waits on __sce_eh_sema_id, then walks objects. A node matches when word 2
// is the caller's frame pointer. The walk unlinks that node. If word 3 is
// nonzero and is not that same pointer, it is passed to __wrap_free. The
// semaphore is signaled and the node is returned. If no node matches, the
// semaphore is signaled and abort is called. The id is loaded again for the
// signal. This body does not free the node itself.
#include "frame_object.hpp"

extern "C" {
extern volatile int __sce_eh_sema_id;
int WaitSema(int id);
int SignalSema(int id);
void __wrap_free(void* pointer);
__attribute__((noreturn)) void abort();

FrameObject* __deregister_frame_info(const void* begin) {
#if !defined(NORRATH_HOST_CONTRACT)
    static_assert(sizeof(void*) == 4, "EE frame object is six 32-bit words");
    static_assert(sizeof(FrameObject) == 24, "EE frame object is 24 bytes");
#endif
    WaitSema(__sce_eh_sema_id);
    FrameObject** link = &objects;
    for (FrameObject* node = objects; node != nullptr; node = node->next) {
        if (node->begin != begin) {
            link = &node->next;
            continue;
        }
        void* extra = node->word3;
        *link = node->next;
        if (extra != nullptr && extra != begin)
            __wrap_free(extra);
        SignalSema(__sce_eh_sema_id);
        return node;
    }
    SignalSema(__sce_eh_sema_id);
    abort();
    __builtin_unreachable();
}
}
