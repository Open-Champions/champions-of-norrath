// Observed megatileBankInit__FP16MegatileInstBankPv at 0x002660a8, 84 bytes.
// Arguments: a bank record and a base value. No result. The bank's first three
// words are two pointer-like fields and a count n. Both pointer-like fields have the
// base added (as 32-bit values). Then, starting at the updated second field, n
// 12-byte entries are walked and the first two words of each have the base added.
// Nothing is done for entries when n is zero or negative. Inferred: a relocation of
// stored offsets to addresses after load. Unknown: the meaning of the entries.
struct MegatileInstBank {
    unsigned first;
    unsigned second;
    int count;
};

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned* norrath_host_words(unsigned address);
#define MEGATILE_WORDS(address) norrath_host_words(address)
#else
#define MEGATILE_WORDS(address) reinterpret_cast<unsigned*>(address)
#endif

extern "C" void megatileBankInit__FP16MegatileInstBankPv(MegatileInstBank* bank, void* base) {
    unsigned b = static_cast<unsigned>(reinterpret_cast<unsigned long>(base));
    int n = bank->count;
    bank->first = bank->first + b;
    bank->second = bank->second + b;
    unsigned* entry = MEGATILE_WORDS(bank->second);
    for (; n > 0; n--, entry += 3) {
        entry[1] = entry[1] + b;
        entry[0] = entry[0] + b;
    }
}
