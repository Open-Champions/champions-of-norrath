// Observed __eq__C9type_infoRC9type_info at 0x2fd2e0, 64 bytes. Identical
// typeinfo pointers compare equal without reading memory. Otherwise the
// first word of each object is its name pointer and the result is whether
// strcmp of the two names returns zero; nonzero means unequal.
// Inferred: this reconstruction name is that comparison, used by the
// script-native type queries against the __tf typeinfo objects.
// Unknown: the callers pass references; only the pointer behavior above is
// established.
extern "C" int strcmp(const char* left, const char* right);

extern "C" int __eq__C9type_infoRC9type_info(const void* left, const void* right) {
    if (left == right) return 1;
    const char* const* left_name = static_cast<const char* const*>(left);
    const char* const* right_name = static_cast<const char* const*>(right);
    return strcmp(*left_name, *right_name) == 0 ? 1 : 0;
}
