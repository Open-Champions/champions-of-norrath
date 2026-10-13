// Observed: Init__7VILList at 0x010727d0 is 20 bytes. The list header is three
// words. Word 0 receives the address of word 1, word 1 receives zero and word 2
// receives the address of the list itself, so the first-node link points at the
// header's own second word and the last-node link at the header. It returns nothing.
// Inferred: the layout is a two-sentinel list in which words 1 and 0..1 overlap
// as the tail node's links.
extern "C" {
void Init__7VILList(void* list) {
    unsigned* words = static_cast<unsigned*>(list);
    words[0] = reinterpret_cast<unsigned>(words + 1);
    words[1] = 0;
    words[2] = reinterpret_cast<unsigned>(list);
}
}
