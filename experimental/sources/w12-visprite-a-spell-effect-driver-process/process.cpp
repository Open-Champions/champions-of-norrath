// Observed: Process__19VISpellEffectDriver at 0x01182fc8, 124 bytes. With the
// object as this, first = the word at this + 72:
//   The first-link word of first is read before anything else (it is the next
//   node). When it is zero the list is empty and the loop is skipped. Otherwise
//   for each node n in turn, with the next node's own first-link word read
//   before n is processed: r = Process__18VISpellEventDriver(n); when r is
//   zero, Remove__7VILNode(n) then _$_18VISpellEventDriver(n, 3). The walk
//   continues while the next node's first-link word is non-zero.
//   Afterwards the word at this + 72 is read again; the result is 1 when the
//   first-link word of that node is non-zero and 0 otherwise.
// Inferred: this + 72 is the first-pointer of an embedded list of event
// drivers whose end marker has a zero first link, and the result says whether
// any event driver remains. Unknown: the list type name.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int Process__18VISpellEventDriver(void* node);
void Remove__7VILNode(void* node);
void _$_18VISpellEventDriver(void* node, int flags);

int Process__19VISpellEffectDriver(void* self) {
    char* base = static_cast<char*>(self);
    unsigned node = *reinterpret_cast<unsigned*>(base + 72);
    unsigned next = *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(node));
    while (next) {
        char* n = NORRATH_TRANSLATE(node);
        if (Process__18VISpellEventDriver(n) == 0) {
            Remove__7VILNode(n);
            _$_18VISpellEventDriver(n, 3);
        }
        node = next;
        next = *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(node));
    }
    unsigned first = *reinterpret_cast<unsigned*>(base + 72);
    return *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(first)) != 0;
}
}
