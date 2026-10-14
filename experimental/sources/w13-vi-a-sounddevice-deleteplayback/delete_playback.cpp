// Observed: DeletePlayback__13VISoundDeviceP10VIPlayback at 0x01126958 is 64 bytes. It calls
// Remove__7VILNode(playback) and then AddTail__7VILListP7VILNode(device + 3296, playback).
// Nothing is returned and nothing else is touched.
// Inferred: a playback object begins with a list node; the device keeps a list at +3296
// (probably a free or finished list) and deleting moves the node onto it, rather than
// releasing memory.
// Unknown: the name or meaning of the list at +3296, and the node/list layouts.
extern "C" {
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

void DeletePlayback__13VISoundDeviceP10VIPlayback(unsigned char* device, void* playback) {
    Remove__7VILNode(playback);
    AddTail__7VILListP7VILNode(device + 3296, playback);
}
}
