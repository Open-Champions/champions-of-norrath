#include "text_storage.cpp"
static_assert(sizeof(norrath_text_missing_entries) == 640, "16 entries of 40 bytes");
int main() {
    for (char* p : norrath_text_hash_slots) if (p) return 1;
    for (auto& e : norrath_text_missing_entries) for (unsigned short h : e) if (h) return 2;
    return norrath_text_missing_cursor;
}
