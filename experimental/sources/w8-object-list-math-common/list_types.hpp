#pragma once
// Layouts observed in the private SLUS_205.65 image for the list helpers.
// Observed: a node is 20 bytes, with next at +0, previous at +4, a partner node
// at +8, a payload pointer at +12 and a halfword at +16 that the allocator
// clears. A head is one word holding the first node, and a linked node's previous
// word holds the address of that head. Observed: the medium and slow run-list
// head tables step by four bytes (15 and 60 heads), so a head is four bytes.
// The single heads named runList and deleteList each occupy an eight-byte slot;
// the second word of a slot is never touched by the routines in this package.
struct ListNode {
    ListNode* next;
    ListNode* prev;
    ListNode* partner;
    void* value;
    short flags;
};

struct ListHead {
    ListNode* first;
};

#if defined(__mips__)
static_assert(sizeof(void*) == 4 && sizeof(int) == 4, "EE word width");
static_assert(sizeof(ListHead) == 4, "head extent");
static_assert(__builtin_offsetof(ListNode, prev) == 4 && __builtin_offsetof(ListNode, partner) == 8 &&
                  __builtin_offsetof(ListNode, value) == 12 && __builtin_offsetof(ListNode, flags) == 16,
              "node offsets");
#endif
