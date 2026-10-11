#include "feat_grants.hpp"
using namespace norrath_feat_grants_detail;

extern "C" int norrath_player_get_feat_rank(int player_index,NorrathSkillIdentity identity) {
    // This is the original unsigned <5 test, including its index4 behavior.
    // Original player owner covers4 records; no source runtime owner is implied.
    if(unsigned(player_index)>=5u)return 0;
    const Address record=player_record(player_index);
    Address catalog=rank_catalog(record),rank=record+243u;
    const Wide bits=static_cast<Wide>(identity);
    for(int index=0;index<64;++index) {
        if(read_wide_fresh(catalog)==bits) {
            const int result=read_signed_byte_fresh(rank);
            return result<0?0:result;
        }
        catalog+=56u;++rank;
    }
    return 0;
}

extern "C" void norrath_player_set_feat_rank(int player_index,NorrathSkillIdentity identity,int rank) {
    const Address record=player_record(player_index);
    Address catalog=rank_catalog(record),destination=record+243u;
    const Wide bits=static_cast<Wide>(identity);
    for(int index=0;index<64;++index) {
        if(read_wide_fresh(catalog)==bits) {
            if(read_signed_byte_fresh(destination)<rank)
                write_byte_ordered(destination,unsigned(rank));
            return;
        }
        catalog+=56u;++destination;
    }
}

extern "C" void norrath_player_give_skill(int skill_index,int player_index) {
    const Address record=player_record(player_index);
    const Address catalog=address(norrath_player_skill_catalog_view)+unsigned(skill_index)*4u;
    const Address descriptor=read_word_fresh(catalog);
    if(read_word_fresh(descriptor+8u)==3u)return;

    int chosen=-1;
    for(int index=0;index<15;++index) {
        if(read_signed_half_fresh(inventory_slot(record,index)+32u)==-1) {
            chosen=index;break;
        }
    }
    // Original always inspects all15 IDs for duplicates, even when no free
    // slot was found. A duplicate discards the captured candidate, not the scan.
    for(int index=0;index<15;++index)
        if(read_signed_half_fresh(inventory_slot(record,index)+32u)==skill_index)
            chosen=-1;
    if(chosen==-1)return;

    const Address destination=inventory_slot(record,chosen);
    norrath_block_memory_set(pointer(destination),0,104u);
    const Address placeholder=address(norrath_player_skill_placeholder_view);
    const unsigned ninth=read_byte_fresh(placeholder+8u);
    const Wide first_eight=read_wide_fresh(placeholder);
    write_byte_ordered(destination+8u,ninth);
    write_wide_ordered(destination,first_eight);
    write_half_ordered(destination+32u,unsigned(skill_index));
    // Descriptor is reread after the real clear and preceding destination writes.
    const Address fresh_descriptor=read_word_fresh(catalog);
    const unsigned kind=read_byte_fresh(fresh_descriptor+8u);
    write_byte_ordered(destination+36u,1u);
    write_byte_ordered(destination+37u,kind);
    const int rank=norrath_player_get_skill_rank(player_index,skill_index);
    norrath_player_set_skill_rank(player_index,skill_index,rank<1?1:rank);
}

extern "C" void norrath_player_give_feat(NorrathSkillIdentity identity,int player_index,int requested_slot) {
    const Address record=player_record(player_index);
    const Wide bits=static_cast<Wide>(identity);
    int chosen=requested_slot;
    if(requested_slot==-1) {
        const Wide captured=read_wide_fresh(record+320u);
        chosen=-1;
        for(int index=0;index<15;++index) {
            // Any intersecting captured bit suppresses all slot reads, exactly
            // as the original captured mask does; it is not refreshed mid-scan.
            if((captured&bits)==0u &&
               read_signed_half_fresh(inventory_slot(record,index)+32u)==-1) {
                chosen=index;break;
            }
        }
    }
    if(chosen==-1)return;
    const Address destination=inventory_slot(record,chosen);
    Address spell=address(norrath_player_feat_spell_table_view);
    for(;;) {
        if(norrath_string_compare(text(spell),text(address(norrath_player_feat_end_name_view)))==0)
            return;
        if(read_wide_fresh(spell+72u)==bits)break;
        spell+=104u;
    }

    if(read_signed_half_fresh(spell+32u)!=29) {
        norrath_factory_strcpy(text(destination),text(spell));
        // Capture these seven original fields after the real string copy and
        // before the first field store. No template/destination disjointness.
        const unsigned kind=read_half_fresh(spell+32u);
        const unsigned first_flag=read_byte_fresh(spell+36u);
        const unsigned second_flag=read_byte_fresh(spell+37u);
        const unsigned first_half=read_half_fresh(spell+38u);
        const unsigned second_half=read_half_fresh(spell+40u);
        const unsigned float_word=read_word_fresh(spell+44u);
        const unsigned subtype=read_half_fresh(spell+34u);
        write_byte_ordered(destination+36u,first_flag);
        write_byte_ordered(destination+37u,second_flag);
        write_half_ordered(destination+38u,first_half);
        write_half_ordered(destination+40u,second_half);
        write_word_ordered(destination+44u,float_word);
        write_half_ordered(destination+32u,kind);
        write_half_ordered(destination+34u,subtype);
        for(unsigned index=0;index<4u;++index)
            write_half_ordered(destination+48u+index*2u,read_half_fresh(spell+48u+index*2u));
        for(unsigned index=0;index<4u;++index)
            write_half_ordered(destination+56u+index*2u,read_half_fresh(spell+56u+index*2u));
        const unsigned second_float_word=read_word_fresh(spell+64u);
        const Wide identity_word=read_wide_fresh(spell+72u);
        write_word_ordered(destination+64u,second_float_word);
        write_wide_ordered(destination+72u,identity_word);
        for(unsigned index=0;index<4u;++index)
            write_word_ordered(destination+80u+index*4u,read_word_fresh(spell+80u+index*4u));
        write_word_ordered(destination+96u,read_word_fresh(spell+96u));
        // No memset or stores to padding42/43,68..71 or100..103 occur here.
    }

    // Type29 skips item copying but still follows the actual stat/bit effects.
    if(bits==64u) {
        const float changed=read_float_fresh(record+312u)+15.0f;
        write_float_ordered(record+312u,changed);
    } else if(bits==128u) {
        const float second=read_float_fresh(record+176u);
        const float first=read_float_fresh(record+172u);
        const float changed_second=second+3.0f,changed_first=first+3.0f;
        write_float_ordered(record+176u,changed_second);
        write_float_ordered(record+172u,changed_first);
        if(changed_second<changed_first)write_float_ordered(record+172u,changed_second);
    } else if(bits==256u) {
        const float second=read_float_fresh(record+200u);
        const float first=read_float_fresh(record+196u);
        const float changed_second=second+5.0f,changed_first=first+5.0f;
        write_float_ordered(record+200u,changed_second);
        write_float_ordered(record+196u,changed_first);
        const float fresh_first=read_float_fresh(record+196u);
        const float fresh_second=read_float_fresh(record+200u);
        if(fresh_second<fresh_first)write_float_ordered(record+196u,fresh_second);
    }
    const Wide fresh=read_wide_fresh(record+320u);
    write_wide_ordered(record+320u,fresh|bits);
}
