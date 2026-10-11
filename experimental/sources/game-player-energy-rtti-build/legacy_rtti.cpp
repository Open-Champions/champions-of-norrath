#include "energy_rtti.hpp"
using namespace norrath_health_regeneration_detail;

extern "C" {
alignas(8) unsigned char norrath_spell_type_info_state[8]{};
alignas(8) unsigned char norrath_spell_blessed_type_info_state[12]{};
}

extern "C" const void* norrath_spell_blessed_type_info() {
    const Address derived=address(norrath_spell_blessed_type_info_state);
    if(read_word_fresh(derived)==0u) {
        const Address base=address(norrath_spell_type_info_state);
        if(read_word_fresh(base)==0u)
            norrath_type_info_init_user(pointer(base),
                reinterpret_cast<const char*>(norrath_spell_type_name_view));
        norrath_type_info_init_single_inheritance(pointer(derived),
            reinterpret_cast<const char*>(norrath_spell_blessed_type_name_view),pointer(base));
    }
    return pointer(derived);
}

extern "C" int norrath_type_info_equal(const void* left,const void* right) {
    if(left==right)return 1;
    const Address left_name=read_word_fresh(address(left));
    const Address right_name=read_word_fresh(address(right));
    return norrath_string_compare(reinterpret_cast<const char*>(pointer(left_name)),
        reinterpret_cast<const char*>(pointer(right_name)))==0?1:0;
}

extern "C" void norrath_type_info_init_user(void* storage,const char* name) {
    const Address out=address(storage);
    if(out==0u)return;
    write_word_ordered(out,address(name));
    write_word_ordered(out+4u,address(norrath_legacy_user_type_info_vtable_view));
}

extern "C" void norrath_type_info_init_single_inheritance(void* storage,const char* name,const void* base) {
    const Address out=address(storage);
    if(out==0u)return;
    write_word_ordered(out+8u,address(base));
    write_word_ordered(out,address(name));
    write_word_ordered(out+4u,address(norrath_legacy_single_inheritance_type_info_vtable_view));
}
