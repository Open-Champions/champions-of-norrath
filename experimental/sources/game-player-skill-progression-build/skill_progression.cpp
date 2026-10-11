#include "skill_progression.hpp"
using namespace norrath_skill_progression_detail;

extern "C" int norrath_player_get_skill_rank(int player_index,int skill_index) {
    Address slot=player_record(player_index)+23712u;
    if(read_signed_half_fresh(slot)==-1)return 0;
    // The original tests the first sentinel, then reads that first ID again.
    int id=read_signed_half_fresh(slot);
    for(;;) {
        if(id==skill_index)return read_signed_half_fresh(slot+2u);
        slot+=4u;
        id=read_signed_half_fresh(slot);
        if(id==-1)return 0;
    }
}

extern "C" void norrath_player_set_skill_rank(int player_index,int skill_index,int rank) {
    Address slot=player_record(player_index)+23712u;
    if(read_signed_half_fresh(slot)!=-1) {
        int id=read_signed_half_fresh(slot);
        for(;;) {
            if(id==skill_index) {
                write_half_ordered(slot+2u,unsigned(rank));
                return;
            }
            slot+=4u;
            id=read_signed_half_fresh(slot);
            if(id==-1)break;
        }
    }
    // Original SH order is ID then rank. The following slot is untouched;
    // no new terminator or list-capacity policy is added by this routine.
    write_half_ordered(slot,unsigned(skill_index));
    write_half_ordered(slot+2u,unsigned(rank));
}

extern "C" void norrath_inventory_adjust_skill(int player_index,char skill_mode,
                                                NorrathSkillIdentity identity) {
    const Address record=player_record(player_index);
    if(signed_word(read_word_fresh(record+236u))<=0)return;
    if(static_cast<unsigned char>(skill_mode)==0u) {
        // This branch preserves all64 incoming identity bits in A2/S0.
        if(norrath_player_get_feat_rank(player_index,identity)>=20)return;
        const unsigned points=read_word_fresh(record+236u);
        write_word_ordered(record+236u,points-1u);
        norrath_player_give_feat(identity,player_index,-1);
        const int rank=norrath_player_get_feat_rank(player_index,identity);
        norrath_player_set_feat_rank(player_index,identity,increment_rank(rank));
        return;
    }

    // The other branch explicitly projects the same incoming identity to
    // signed32 before every actual skill-rank/table/helper argument.
    const int skill_index=signed_word(static_cast<unsigned>(identity));
    if(norrath_player_get_skill_rank(player_index,skill_index)>=20)return;
    const unsigned points=read_word_fresh(record+236u);
    write_word_ordered(record+236u,points-1u);
    if(norrath_player_get_skill_rank(player_index,skill_index)>0) {
        const int rank=norrath_player_get_skill_rank(player_index,skill_index);
        norrath_player_set_skill_rank(player_index,skill_index,increment_rank(rank));
        return;
    }

    const Address catalog_slot=address(norrath_player_skill_catalog_view)+unsigned(skill_index)*4u;
    const Address descriptor=read_word_fresh(catalog_slot);
    if(read_word_fresh(descriptor+8u)==3u)
        norrath_player_set_skill_rank(player_index,skill_index,1);
    else
        norrath_player_give_skill(skill_index,player_index);
}
