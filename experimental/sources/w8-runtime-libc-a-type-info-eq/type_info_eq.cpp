// Observed __eq__C9type_infoRC9type_info at 0x002fd2e0, 60 bytes. Two type_info
// references are equal when they are the same object. Otherwise the first word
// of each object (a pointer to the class name text) is compared with strcmp at
// 0x003044fc, and a zero result means equal. The result is 1 or 0.
// Inferred: the first word is the name pointer; it is the argument to strcmp.
// The name pointer is not tested for null, so a null name faults in strcmp.
extern "C" {
int strcmp(const char* left, const char* right);

int __eq__C9type_infoRC9type_info(const void* left, const void* right) {
    if (left == right) return 1;
    const char* const* left_name = static_cast<const char* const*>(left);
    const char* const* right_name = static_cast<const char* const*>(right);
    return strcmp(*left_name, *right_name) == 0;
}
}
