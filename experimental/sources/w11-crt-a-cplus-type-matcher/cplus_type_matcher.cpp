// Observed __cplus_type_matcher at 0x002fef20, 96 bytes, pinned SLUS_205.65. The address of this
// function is word 0 of every record that __cp_push_exception (0x002fef80) builds. Arguments:
// the exception record, a match value taken from a catch clause, and a table pointer.
//   - A non-null table whose halfword at +4 is not 4 gives a null result (a different language).
//     A null table skips that test.
//   - A match value of -1 (a catch-all clause) gives 1.
//   - Otherwise the match value is called as a function with no arguments; its result, the
//     record's type word (+12), the record's second object word (+40) and the address of the
//     record's first object word (+8) go to __throw_type_match_rtti_2 (0x002fe0d0), and the
//     answer is 1 when that call returned nonzero, else 0.
// The record words are 32 bits wide on the target, so they are read as unsigned words and passed
// on as words; only the match value and the record address are real pointers here.
// Inferred: the language value 4 and the -1 catch-all convention, from how the function uses
// them. Unknown: the table's declared type and what __throw_type_match_rtti_2 does with its
// arguments. Nothing was executed on the EE.
extern "C" {
int __throw_type_match_rtti_2(void* match, unsigned type, unsigned original, void* value);

void* __cplus_type_matcher(void* record, void* match_info, const unsigned short* table) {
    if (table != 0 && table[2] != 4) return 0;
    if (match_info == reinterpret_cast<void*>(-1)) return reinterpret_cast<void*>(1);
    void* const match = reinterpret_cast<void* (*)()>(match_info)();
    const unsigned char* const bytes = static_cast<const unsigned char*>(record);
    const unsigned type = *reinterpret_cast<const unsigned*>(bytes + 12);
    const unsigned original = *reinterpret_cast<const unsigned*>(bytes + 40);
    const int found = __throw_type_match_rtti_2(match, type, original, const_cast<unsigned char*>(bytes) + 8);
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(found != 0));
}
}
