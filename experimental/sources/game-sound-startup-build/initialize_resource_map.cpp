#include "sound_startup.hpp"
using namespace norrath_sound_startup_detail;

extern "C" void norrath_sound_resource_map_initialize(void* storage,char* name,char* extension) {
    const Address object=address(storage);
    write_word_ordered(object+4u,address(name));
    write_word_ordered(object,0xffffffffu);
    write_word_ordered(object+8u,address(extension));
}
