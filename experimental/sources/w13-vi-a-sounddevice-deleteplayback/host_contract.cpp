#include "delete_playback.cpp"

static void* log_[8]; static int n;
extern "C" void Remove__7VILNode(void* node) { log_[n++] = (void*)1; log_[n++] = node; }
extern "C" void AddTail__7VILListP7VILNode(void* list, void* node) { log_[n++] = (void*)2; log_[n++] = list; log_[n++] = node; }

int main() {
    static unsigned char dev[4096]; int pb;
    DeletePlayback__13VISoundDeviceP10VIPlayback(dev, &pb);
    if (n != 5) return 1;
    if (log_[0] != (void*)1 || log_[1] != &pb) return 2;
    if (log_[2] != (void*)2 || log_[3] != dev + 3296 || log_[4] != &pb) return 3;
    return 0;
}
