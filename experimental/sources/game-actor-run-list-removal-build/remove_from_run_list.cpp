#include "run_list_removal.hpp"
extern "C" unsigned norrath_actor_remove_from_run_list(void* object) {
    using namespace norrath_constructor_detail;
    const Address node = read_word(address(object) + 4u);
    if (node == 0u) return 0u;
    return norrath_list_remove(bytes(node));
}
