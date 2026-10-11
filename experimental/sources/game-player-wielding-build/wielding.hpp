#pragma once
#include "feat_grants.hpp"
extern "C" {
int norrath_inventory_check_wielding_status(int player_index,const void* description);
// Exact canonical declaration already used by genuine entity restoration.
char* norrath_factory_strstr(const char* text,const char* needle);
extern unsigned char norrath_player_wielding_name_fragment_view[];
}
namespace norrath_wielding_detail {
using namespace norrath_feat_grants_detail;
// Original scans exactly four signed-half effect IDs at36+8*i and takes the
// first matching signed-half value at38+8*i. Missing value is zero, not success.
inline int requirement(Address item,int effect_id) {
    for(unsigned index=0;index<4u;++index) {
        const Address effect=item+36u+index*8u;
        if(read_signed_half_fresh(effect)==effect_id)
            return read_signed_half_fresh(effect+2u);
    }
    return 0;
}
}
