// Observed unet_startup_async_status__Fv at 0x0101c918, 116 bytes. With
// unet_startup_thread_id (0x0118e6bc) equal to -1 it returns 0. Otherwise it
// asks ReferThreadStatus(id, 0) for the status. Any value other than 16 returns
// 1 (still running). On 16 it sets net_ready (0x0118e6b8) to 1, then calls
// TerminateThread and DeleteThread on the id read from memory, stores -1 in
// unet_startup_thread_id and returns 0. The net_ready store sits in the
// delay slot of the TerminateThread call. Unknown: what status value 16 means
// beyond the observed comparison.
extern "C" {
extern int net_ready;
extern int unet_startup_thread_id;
int ReferThreadStatus(int thread, void* info);
int TerminateThread(int thread);
int DeleteThread(int thread);

int unet_startup_async_status__Fv() {
    if (unet_startup_thread_id == -1) return 0;
    if (ReferThreadStatus(unet_startup_thread_id, nullptr) != 16) return 1;
    net_ready = 1;
    TerminateThread(unet_startup_thread_id);
    DeleteThread(unet_startup_thread_id);
    unet_startup_thread_id = -1;
    return 0;
}
}
