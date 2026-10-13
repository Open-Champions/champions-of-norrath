// Observed __13VISoundDevice at 0x01124618, 332 bytes. Offsets are bytes
// from the object. In order it:
//  - runs the VILNode constructor on 46 nodes of 56 bytes starting at 8;
//  - runs the VILList constructor on the lists at 3284, 3296, 3312, 3324,
//    3340 and 3352;
//  - clears 128 words at 2584..3095 and 46 words at 3100..3283;
//  - runs Init__7VILList on the list at 3296 and appends the 46 nodes to it
//    in address order with AddTail__7VILListP7VILNode;
//  - stores 1 at 3412, 0x16062 at 3308, zero at 3420, a 64-bit zero at 0, and
//    zero at 3096, 3384, 3408, 3404 and 4472, and a zero byte at 3948.
// It returns nothing. What the nodes, lists and the stored values mean is
// not established here.
extern "C" {
struct VISoundDevice;
struct VILNode;
struct VILList;

void __7VILNode(VILNode* node);
void __7VILList(VILList* list);
void Init__7VILList(VILList* list);
void AddTail__7VILListP7VILNode(VILList* list, VILNode* node);

void __13VISoundDevice(VISoundDevice* device) {
    char* base = reinterpret_cast<char*>(device);
    for (int i = 0; i < 46; ++i) __7VILNode(reinterpret_cast<VILNode*>(base + 8 + 56 * i));
    __7VILList(reinterpret_cast<VILList*>(base + 3284));
    __7VILList(reinterpret_cast<VILList*>(base + 3296));
    __7VILList(reinterpret_cast<VILList*>(base + 3312));
    __7VILList(reinterpret_cast<VILList*>(base + 3324));
    __7VILList(reinterpret_cast<VILList*>(base + 3340));
    __7VILList(reinterpret_cast<VILList*>(base + 3352));
    unsigned* table = reinterpret_cast<unsigned*>(base + 2584);
    for (int i = 127; i >= 0; --i) table[i] = 0;
    unsigned* words = reinterpret_cast<unsigned*>(base + 3100);
    for (int i = 45; i >= 0; --i) words[i] = 0;
    VILList* list = reinterpret_cast<VILList*>(base + 3296);
    Init__7VILList(list);
    for (int i = 0; i < 46; ++i)
        AddTail__7VILListP7VILNode(list, reinterpret_cast<VILNode*>(base + 8 + 56 * i));
    *reinterpret_cast<unsigned*>(base + 3412) = 1;
    *reinterpret_cast<unsigned*>(base + 3308) = 0x16062;
    *reinterpret_cast<unsigned*>(base + 3420) = 0;
    *reinterpret_cast<unsigned long long*>(base) = 0;
    *reinterpret_cast<unsigned*>(base + 3096) = 0;
    *reinterpret_cast<unsigned*>(base + 3384) = 0;
    *reinterpret_cast<unsigned*>(base + 3408) = 0;
    *reinterpret_cast<unsigned*>(base + 3404) = 0;
    *reinterpret_cast<unsigned*>(base + 4472) = 0;
    *reinterpret_cast<unsigned char*>(base + 3948) = 0;
}
}
