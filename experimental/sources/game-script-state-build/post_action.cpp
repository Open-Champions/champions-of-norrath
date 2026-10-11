#include "state.hpp"
extern "C" unsigned long long norrath_script_set_post_action(char* action) {
    using namespace norrath_constructor_detail;
    char* result=norrath_factory_strcpy(text(address(norrath_script_post_action_view)),action);
    return static_cast<unsigned long long>(static_cast<long long>(signed_word(address(result))));
}
