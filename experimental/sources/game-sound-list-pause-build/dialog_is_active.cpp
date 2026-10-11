#include "list_pause.hpp"
using namespace norrath_sound_list_pause_detail;

extern "C" int norrath_dialog_is_active() {
    // Whole real DLG_Active leaf: LW followed by unsigned zero-less-than-word.
    return read_word_fresh(address(norrath_dialog_active_pointer_word_view))!=0u?1:0;
}
