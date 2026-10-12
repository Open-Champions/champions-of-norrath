// Host check only; not an EE build input. Stubs record the call order.
#include <cstring>
extern "C" {
int sLibNetInitialized;
void* net_buf;
unsigned char cd[60];
int log_len;
int log_kind[8];
void* log_arg[8];
int seen_flag_at_vi_free = -1;
int sceLibnetTerminate(void* client) { log_kind[log_len] = 1; log_arg[log_len++] = client; return 0; }
void VIFreeUSocketExApi__Fv() { seen_flag_at_vi_free = sLibNetInitialized; log_kind[log_len] = 2; log_arg[log_len++] = 0; }
void __wrap_free(void* block) { log_kind[log_len] = 3; log_arg[log_len++] = block; }
void unet_shutdown__Fv();
}
int main() {
    int marker;
    net_buf = &marker;
    sLibNetInitialized = 0;
    unet_shutdown__Fv();
    if (log_len != 0) return 1;
    sLibNetInitialized = 7;
    unet_shutdown__Fv();
    if (log_len != 3 || log_kind[0] != 1 || log_arg[0] != cd) return 2;
    if (log_kind[1] != 2 || seen_flag_at_vi_free != 0) return 3;  // cleared before the call
    if (log_kind[2] != 3 || log_arg[2] != &marker) return 4;
    if (net_buf != &marker || sLibNetInitialized != 0) return 5;  // pointer is not cleared
    return 0;
}
