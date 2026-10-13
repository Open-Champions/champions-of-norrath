#include "rem_head.cpp"
#include <string.h>

int main() {
    VILLink a, b, c;
    VILList list;
    // Detach a middle-linked node: b is the head, with a before and c after.
    a.next = &b; a.prev = 0; b.next = &c; b.prev = &a; c.next = 0; c.prev = &b;
    list.head = &b;
    if (RemHead__7VILList(&list) != &b) return 1;
    if (a.next != &c || c.prev != &a || b.next || b.prev) return 2;
    if (list.head != &b) return 3;
    // Null next: nothing changes, null result.
    a.next = 0; a.prev = &c; list.head = &a;
    if (RemHead__7VILList(&list) != 0 || a.next || a.prev != &c) return 4;
    // Null prev with a next: node returned untouched.
    a.next = &c; a.prev = 0; c.prev = &b; list.head = &a;
    if (RemHead__7VILList(&list) != &a || a.next != &c || a.prev || c.prev != &b) return 5;
    return 0;
}
