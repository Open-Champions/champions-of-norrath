// Host check for getPicSize__4itemiPQ24item11descriptionPiT3. Not an EE plan input.
#include "get_pic_size.cpp"
#include <cstring>
static int name_calls, find_calls, seen_a;
static void* seen_desc;
static char* seen_buffer;
alignas(8) static unsigned char record[16];
static unsigned char* found;
static char found_for[64];
extern "C" void getPicName__4itemPciPQ24item11description(char* buffer, int a, void* description) {
    ++name_calls;
    seen_a = a;
    seen_desc = description;
    seen_buffer = buffer;
    std::strcpy(buffer, "pic-name");
}
extern "C" unsigned char* picFind__4itemPc(char* name) {
    ++find_calls;
    if (name != seen_buffer) return reinterpret_cast<unsigned char*>(1);
    std::strcpy(found_for, name);
    return found;
}
int main() {
    int desc_marker;
    int w = 123, h = 456;
    found = nullptr;
    getPicSize__4itemiPQ24item11descriptionPiT3(9, &desc_marker, &w, &h);
    if (name_calls != 1 || find_calls != 1 || seen_a != 9 || seen_desc != &desc_marker) return 1;
    if (std::strcmp(found_for, "pic-name") != 0) return 2;
    if (w != 0 || h != 0) return 3;
    std::memset(record, 0x55, sizeof record);
    short a = 300, b = -17;
    std::memcpy(record + 6, &a, 2);
    std::memcpy(record + 8, &b, 2);
    found = record;
    getPicSize__4itemiPQ24item11descriptionPiT3(2, nullptr, &w, &h);
    if (w != 300 || h != -17) return 4;
    for (int i = 0; i < 16; ++i) if (i != 6 && i != 7 && i != 8 && i != 9 && record[i] != 0x55) return 5;
    return 0;
}
