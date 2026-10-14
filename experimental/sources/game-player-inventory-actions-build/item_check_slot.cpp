#include "inventory_actions.hpp"
using namespace norrath_inventory_actions_detail;

extern "C" int norrath_inventory_item_check_slot(int player_index,const void* description) {
    const Address slots=player_record(player_index)+21896u;
    const Address item=address(description);
    for(unsigned index=0;index<16u;++index) {
        const Address stored=read_word_fresh(slots+index*4u);
        // Null is compared exactly like every other incoming pointer.
        if(item==stored)return int(index);
    }
    return signed_word(0xfa0a1f00u);
}
