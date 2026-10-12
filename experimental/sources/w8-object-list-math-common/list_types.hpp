#pragma once
// Layouts observed in the private SLUS_205.65 image for the list helpers.
// Observed: a node is 20 bytes, with next at +0, previous at +4, a partner node
// at +8, a payload pointer at +12 and a halfword at +16 that the allocator
// clears. A head's first word is the first node, and a linked node's previous
// word holds the address of that head. Inferred: the head type is eight bytes
// because the run, delete and always-draw heads sit eight bytes apart; the second
// word is never read by the routines in this package.
struct ListNode {
    ListNode* next;
    ListNode* prev;
    ListNode* partner;
    void* value;
    short flags;
};

struct ListHead {
    ListNode* first;
    unsigned second_word;
};

#if defined(__mips__)
static_assert(sizeof(void*) == 4 && sizeof(int) == 4, "EE word width");
static_assert(sizeof(ListHead) == 8, "head extent");
static_assert(__builtin_offsetof(ListNode, prev) == 4 && __builtin_offsetof(ListNode, partner) == 8 &&
                  __builtin_offsetof(ListNode, value) == 12 && __builtin_offsetof(ListNode, flags) == 16,
              "node offsets");
#endif
