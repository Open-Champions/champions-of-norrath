#include "energy_rtti.hpp"
using namespace norrath_health_regeneration_detail;

extern "C" void* norrath_creature_find_running_spell(void* actor,const void* wanted) {
    Address node=read_word_fresh(address(actor)+888u);
    while(node!=0u) {
        const Address spell=read_word_fresh(node+12u);
        if(spell!=0u) {
            const Address table=read_word_fresh(spell+20u);
            const Address entry=read_word_fresh(table+4u);
            using TypeFunction=const void* (*)();
            const void* actual=reinterpret_cast<TypeFunction>(entry)();
            if(norrath_type_info_equal(actual,wanted)!=0)
                return pointer(read_word_fresh(node+12u));
        }
        node=read_word_fresh(node);
    }
    return nullptr;
}

extern "C" int norrath_spell_blessed_regeneration(void* spell) {
    const Address s=address(spell);
    const Address owner=read_word_fresh(s+8u);
    const int level=read_signed_byte_fresh(s);
    const Address table=read_word_fresh(owner+96u)+88u;
    const int displacement=read_signed_half_fresh(table);
    const Address entry=read_word_fresh(table+4u);
    using RegenerationFunction=int (*)(void*,int,int);
    return reinterpret_cast<RegenerationFunction>(entry)(pointer(owner+unsigned(displacement)),0,level);
}

extern "C" int norrath_skill_regeneration_amount(void* descriptor,int rank) {
    const Address owner=address(descriptor);
    const Address table=read_word_fresh(owner+96u)+88u;
    const int displacement=read_signed_half_fresh(table);
    const Address entry=read_word_fresh(table+4u);
    using RegenerationFunction=int (*)(void*,int,int);
    return reinterpret_cast<RegenerationFunction>(entry)(pointer(owner+unsigned(displacement)),0,rank);
}
