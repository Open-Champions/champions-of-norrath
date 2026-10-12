// Observed __register_frame_info at 0x002fc428, 104 bytes.
// Clears five words of the caller object, stores the frame pointer at word 2,
// then links the object at `objects` between WaitSema and SignalSema.
// The semaphore id is loaded again after the wait. What those services do
// with the id is unknown. This body does not allocate and does not read the
// frame bytes.
#include "frame_object.hpp"

extern "C" {
extern volatile int __sce_eh_sema_id;
int WaitSema(int id);
int SignalSema(int id);
FrameObject* objects;

void __register_frame_info(const void* begin, void* object) {
#if !defined(NORRATH_HOST_CONTRACT)
    static_assert(sizeof(void*) == 4, "EE frame object is six 32-bit words");
    static_assert(sizeof(FrameObject) == 24, "EE frame object is 24 bytes");
#endif
    auto* record = static_cast<FrameObject*>(object);
    record->begin = begin;
    record->word1 = nullptr;
    record->word0 = nullptr;
    record->word3 = nullptr;
    record->word4 = nullptr;
    WaitSema(__sce_eh_sema_id);
    record->next = objects;
    objects = record;
    SignalSema(__sce_eh_sema_id);
}
}
