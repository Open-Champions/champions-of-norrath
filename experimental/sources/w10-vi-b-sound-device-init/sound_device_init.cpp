// Observed Init__13VISoundDevice at 0x01124768, 320 bytes. Offsets are bytes
// from the object. In order it:
//  - runs Init__7VILList on the lists at 3284 and 3296;
//  - clears 128 words at 2584..3095 and 46 words at 3100..3283;
//  - appends the 46 56-byte nodes starting at offset 8 to the list at 3296
//    in address order with AddTail__7VILListP7VILNode;
//  - stores 1 at 3432, 3412; 0x16062 at 3308; 1.0f at 3424, 3428 and 4468;
//    zero at 3096, 3384, 3408, 3404, 4472, 4460, 4464 and 3420; a 64-bit zero
//    at 0; and zero bytes at 3436, 3692, 3948 and 4204.
// It returns zero. What the lists, nodes and fields mean is not established.
extern "C" {
struct VISoundDevice;
struct VILNode;
struct VILList;

void Init__7VILList(VILList* list);
void AddTail__7VILListP7VILNode(VILList* list, VILNode* node);

int Init__13VISoundDevice(VISoundDevice* device) {
    char* base = reinterpret_cast<char*>(device);
    VILList* list = reinterpret_cast<VILList*>(base + 3296);
    Init__7VILList(reinterpret_cast<VILList*>(base + 3284));
    Init__7VILList(list);
    unsigned* table = reinterpret_cast<unsigned*>(base + 2584);
    for (int i = 127; i >= 0; --i) table[i] = 0;
    unsigned* words = reinterpret_cast<unsigned*>(base + 3100);
    for (int i = 45; i >= 0; --i) words[i] = 0;
    for (int i = 0; i < 46; ++i)
        AddTail__7VILListP7VILNode(list, reinterpret_cast<VILNode*>(base + 8 + 56 * i));
    *reinterpret_cast<unsigned*>(base + 3432) = 1;
    *reinterpret_cast<unsigned long long*>(base) = 0;
    *reinterpret_cast<unsigned*>(base + 3096) = 0;
    *reinterpret_cast<unsigned*>(base + 3384) = 0;
    *reinterpret_cast<unsigned*>(base + 3408) = 0;
    *reinterpret_cast<unsigned*>(base + 3404) = 0;
    *reinterpret_cast<unsigned*>(base + 3412) = 1;
    *reinterpret_cast<unsigned*>(base + 4472) = 0;
    *reinterpret_cast<unsigned char*>(base + 3436) = 0;
    *reinterpret_cast<unsigned char*>(base + 3692) = 0;
    *reinterpret_cast<unsigned char*>(base + 3948) = 0;
    *reinterpret_cast<unsigned char*>(base + 4204) = 0;
    *reinterpret_cast<float*>(base + 4468) = 1.0f;
    *reinterpret_cast<unsigned*>(base + 4460) = 0;
    *reinterpret_cast<unsigned*>(base + 4464) = 0;
    *reinterpret_cast<unsigned*>(base + 3308) = 0x16062;
    float* gains = reinterpret_cast<float*>(base + 3424);
    gains[1] = 1.0f;
    gains[0] = 1.0f;
    *reinterpret_cast<unsigned*>(base + 3420) = 0;
    return 0;
}
}
