// Observed strstr at 0x00305328, 132 bytes. It returns the first position of
// haystack at which the whole needle matches, or null. An empty needle gives
// the haystack itself, including for an empty haystack. The walk stops at the
// haystack's zero, so a needle longer than the remaining text never matches.
extern "C" char* strstr(const char* text, const char* needle) {
    if (*needle == '\0') return const_cast<char*>(text);
    for (; *text != '\0'; ++text) {
        unsigned matched = 0u;
        while (needle[matched] != '\0' && text[matched] == needle[matched]) ++matched;
        if (needle[matched] == '\0') return const_cast<char*>(text);
    }
    return nullptr;
}
