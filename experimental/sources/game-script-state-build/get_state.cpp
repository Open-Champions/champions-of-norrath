#include "state.hpp"
extern "C" long long norrath_script_get_game_state(char* name) {
    using namespace norrath_constructor_detail;
    char folded[256] = {};
    norrath_factory_strcpy(folded,name);
    lower_nonempty_tag(address(folded));
    Address entry=address(norrath_script_game_state_data_view);
    while (signed_byte(*bytes(entry))!=0) {
        const int comparison=norrath_string_compare(text(entry),folded);
        if (comparison==0) {
            const unsigned length=norrath_factory_strlen(text(entry));
            return static_cast<long long>(signed_byte(*bytes(entry+length+1u)));
        }
        const unsigned length=norrath_factory_strlen(text(entry));
        entry+=length+2u;
    }
    norrath_factory_strcpy(text(entry),folded);
    const unsigned length=norrath_factory_strlen(text(entry));
    *bytes(entry+length+1u)=0u;
    return 0;
}
