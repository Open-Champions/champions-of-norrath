// Observed qsort_translate_table at 0x002e7a38, 472 bytes: a recursive quicksort over an
// array of 10-byte records (two-byte aligned, copied as 8 + 2 bytes) taking the array, a
// first index and a last index (inclusive, signed). The ordering key is the unsigned byte at
// offset 1 of each record; the sort is not stable.
//  - Nothing happens when first >= last.
//  - The middle index is (first + last) / 2 (rounded toward zero for the signed sum). The
//    record at first and the middle record exchange places, so the pivot sits at first.
//  - For each index after first up to last, a record whose key is below the pivot key is
//    exchanged with the record one past the running "below" boundary, and the boundary moves.
//  - The pivot is exchanged with the record at the boundary, then the part below the
//    boundary and the part above it are sorted by recursive calls (lower part first).
// Inferred: the name suggests a translation table, but what the records hold beyond the key
// byte is unknown, as is the caller.
// Nothing here was executed on the EE.
struct TranslateRecord {
    unsigned short words[5];
};

static inline unsigned key_of(const TranslateRecord& record) {
    return reinterpret_cast<const unsigned char*>(&record)[1];
}

static inline void exchange(TranslateRecord& a, TranslateRecord& b) {
    const TranslateRecord held = a;
    a = b;
    b = held;
}

extern "C" void qsort_translate_table(TranslateRecord* table, int first, int last) {
    if (first >= last) return;
    exchange(table[first], table[(first + last) / 2]);
    int boundary = first;
    for (int i = first + 1; i <= last; ++i) {
        if (key_of(table[i]) < key_of(table[first])) {
            ++boundary;
            exchange(table[boundary], table[i]);
        }
    }
    exchange(table[first], table[boundary]);
    qsort_translate_table(table, first, boundary - 1);
    qsort_translate_table(table, boundary + 1, last);
}
