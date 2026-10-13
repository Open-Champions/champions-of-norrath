// Observed __builtin_vec_new at 0x002fed78, 116 bytes (weak), pinned SLUS_205.65. Passes its single
// argument to __builtin_new (0x002fec60) and returns the result. The size is not touched, so
// zero and failure are handled by __builtin_new; no array bookkeeping is added.
// Not authored: behind the normal path the original carries a cleanup and handler region
// (calls to __cp_pop_exception, __rethrow, __start_cp_handler, __check_eh_spec and
// terminate__Fv, with a bad_alloc record) that only runs while an exception passes through the
// call. The -fno-exceptions profile cannot produce those tables, so only the normal path exists.
extern "C" {
void* __builtin_new(unsigned size);

__attribute__((weak)) void* __builtin_vec_new(unsigned size) {
    return __builtin_new(size);
}
}
