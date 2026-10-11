// Pinned __do_global_dtors at 0x002f9e58. The cursor advances before calls;
// frame deregistration is guarded separately, including callback reentry.
using Callback = void (*)();
extern "C" {
extern Callback __DTOR_LIST__[];
extern unsigned char __EH_FRAME_BEGIN__[];
void __deregister_frame_info(const void*);
void __do_global_dtors() {
    static Callback* cursor = __DTOR_LIST__ + 1;
    static unsigned completed;
    while (*cursor != nullptr) {
        Callback callback = *cursor++;
        callback();
    }
    if (completed == 0) {
        completed = 1;
        __deregister_frame_info(__EH_FRAME_BEGIN__);
    }
}
}
