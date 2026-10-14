// Observed: NewPlayback__13VISoundDevice at 0x01126908, 76 bytes. It calls RemHead__7VILList on the list at device+3296; if that
// returns a node it calls AddTail__7VILListP7VILNode on the list at device+3284 with that node. The node (possibly null) is returned.
// Inferred: device+3296 is a free list and device+3284 a live list of VIPlayback records (the neighbouring DeletePlayback,
// FirstPlayback and NextPlayback members at 0x01126958 to 0x011269f0 are the other users); this routine moves one record from free to live.
// Unknown: the VILList layout (its callees are not authored here) and the playback record layout.
extern "C" {
void* RemHead__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

void* NewPlayback__13VISoundDevice(void* device) {
    unsigned char* base = static_cast<unsigned char*>(device);
    void* node = RemHead__7VILList(base + 3296);
    if (node) AddTail__7VILListP7VILNode(base + 3284, node);
    return node;
}
}
