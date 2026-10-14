#include "sound_command_initialization.hpp"
using namespace norrath_sound_command_initialization_detail;
extern "C" void norrath_sound_add_command_handler(int command,
                                                 NorrathSoundCommandHandler handler,
                                                 void* parameter) {
    // The existing signed32 command ABI covers canonical entry values only;
    // original native BGEZ tests full64. No new guard or command/count cap.
    const bool system_command=command<0;
    const unsigned number=system_command?(unsigned(command)&0x7fffffffu):unsigned(command);
    const Address offset=number*12u; // actual low32 product; HI/LO incidental
    const Address state=address(norrath_sound_command_state_view);
    const Address table_field=state+(system_command?12u:20u);
    const Address captured_table=read_aligned_word(table_field);
    const Address record=captured_table+offset;
    write_aligned_word(record+4u,address(parameter));
    write_aligned_word(record,command_handler_word(handler));
    // Capture actual live full64 GP AFTER both stores, then reload the table
    // pointer fresh. Aliases may change it; do not reuse the captured table.
    const Raw64 gp=current_gp_bits();
    const Address fresh_table=read_aligned_word(table_field);
    write_aligned_word(fresh_table+offset+8u,unsigned(gp));
    // Native return V0 and call-volatile temporaries are unpromised by void.
}
