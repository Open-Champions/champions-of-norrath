// Observed __tf10GameObject at 0x312dd0, 60 bytes.
// Returns the eight-byte __ti10GameObject object. When its first word is zero,
// calls __rtti_user with that object and the bytes "10GameObject" at 0x461e98.
// A nonzero first word skips the call. The bytes after that NUL are not used.

extern "C" {
unsigned char norrath_ti_10GameObject[8];
void norrath_rtti_user(void* typeinfo, const char* name);
}

extern "C" void* norrath_vt_tf_10GameObject() {
    const unsigned char* raw = norrath_ti_10GameObject;
    const unsigned first = unsigned(raw[0]) | (unsigned(raw[1]) << 8) |
                           (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
    if (first == 0u) norrath_rtti_user(norrath_ti_10GameObject, "10GameObject");
    return norrath_ti_10GameObject;
}
