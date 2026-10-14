#include "search_svdata.cpp"

int main() {
    RpcServiceEntry a1 = {}, a2 = {}, b1 = {}, c1 = {}, dup = {};
    RpcServerRecord first = {}, empty = {}, last = {};
    RpcServiceList list = {};
    a1.id = 0x100; a1.next = &a2;
    a2.id = 0x101;
    b1.id = 0x200;
    c1.id = 0x300; c1.next = &dup;
    dup.id = 0x200;  // a repeated id later in the walk must not win
    first.entries = &a1; first.next = &empty;
    empty.entries = 0; empty.next = &last;   // a record without entries is skipped
    last.entries = &c1;
    // Put b1 in a record between so the order matters.
    RpcServerRecord middle = {};
    middle.entries = &b1; middle.next = &last;
    empty.next = &middle;
    list.records = &first;
    if (_search_svdata(0x100, &list) != &a1) return 1;
    if (_search_svdata(0x101, &list) != &a2) return 2;
    if (_search_svdata(0x200, &list) != &b1) return 3;
    if (_search_svdata(0x300, &list) != &c1) return 4;
    if (_search_svdata(0x999, &list) != 0) return 5;
    // Id zero matches an entry with id zero only.
    if (_search_svdata(0, &list) != 0) return 6;
    c1.id = 0;
    if (_search_svdata(0, &list) != &c1) return 7;
    list.records = 0;
    if (_search_svdata(0x100, &list) != 0) return 8;
    return 0;
}
