#include "native_gold.hpp"
using namespace norrath_skill_progression_detail;

extern "C" long long norrath_amx_native_give_player_gold(void*,const void* parameters) {
    const int count=signed_word(read_word_fresh(address(norrath_player_active_count_word_view)));
    if(count<=0)return 0ll;
    Address gold=address(norrath_player_progression_records_view)+232u;
    unsigned index=0u;
    do {
        // The amount is reread for every record, before its old gold value.
        const int amount=signed_word(read_word_fresh(address(parameters)+4u));
        index+=1u;
        const unsigned previous=read_word_fresh(gold);
        const int share=amount/count;
        write_word_ordered(gold,previous+static_cast<unsigned>(share));
        gold+=23984u;
    } while(signed_word(index)<count);
    return 0ll;
}

extern "C" long long norrath_amx_native_take_player_gold(void*,const void* parameters) {
    const int count=signed_word(read_word_fresh(address(norrath_player_active_count_word_view)));
    if(count<=0)return 0ll;
    Address gold=address(norrath_player_progression_records_view)+232u;
    unsigned index=0u;
    do {
        const int amount=signed_word(read_word_fresh(address(parameters)+4u));
        index+=1u;
        const unsigned previous=read_word_fresh(gold);
        const int share=amount/count;
        const unsigned difference=previous-static_cast<unsigned>(share);
        // SUBU wraps first; the original clamps the signed wrapped result.
        write_word_ordered(gold,signed_word(difference)<0?0u:difference);
        gold+=23984u;
    } while(signed_word(index)<count);
    return 0ll;
}

extern "C" long long norrath_amx_native_check_player_gold(void*,const void*) {
    unsigned remaining=read_word_fresh(address(norrath_player_active_count_word_view));
    unsigned total=0u;
    if(signed_word(remaining)<=0)return 0ll;
    Address gold=address(norrath_player_progression_records_view)+232u;
    do {
        const unsigned value=read_word_fresh(gold);
        gold+=23984u;
        remaining-=1u;
        total+=value;
    } while(remaining!=0u);
    return static_cast<long long>(signed_word(total));
}
