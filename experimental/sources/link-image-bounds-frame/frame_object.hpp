// Six-word frame object written by the registrar at 0x002fc428.
// Words 0, 1, and 4 are cleared. Word 2 receives the frame pointer.
// Word 3 is cleared by that registrar; the deregistrar frees it only when
// it is neither zero nor the searched frame pointer. Word 5 is the previous
// list head. On the EE these words are 32 bits.
struct FrameObject {
    void* word0;
    void* word1;
    const void* begin;
    void* word3;
    void* word4;
    FrameObject* next;
};

// Observed local BSS word `objects` at 0x008eab18. The pinned symbol is local.
// This definition is global so the registrar and deregistrar can share it.
extern "C" FrameObject* objects;
