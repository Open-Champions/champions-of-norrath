// Observed: Next__Ct6VIPool1ZP16VIStaticLightingi at 0x01224c18 is 44 bytes. With (this, index) it reads the
// word at this+8, indexes it in 12-byte steps, loads the word at +4 of that element and returns
// it, except that the value -1 is returned as 0x7fffffff.
// Inferred: the element's word at +4 is the next index of a chain, -1 ends the chain and
// 0x7fffffff is the iterator's end marker; only this code shape is evidence.
// Unknown: the other two words of each 12-byte element.
struct Element {
    int word0;
    int next;
    int word8;
};

struct Container {
    int word0;
    int word4;
    Element* elements;
};

extern "C" {
int Next__Ct6VIPool1ZP16VIStaticLightingi(const Container* self, int index) {
    int next = self->elements[index].next;
    return next == -1 ? 0x7fffffff : next;
}
}
