#include "network_send.hpp"
using namespace norrath_network_send_detail;

extern "C" void norrath_net_send_message(int handler,unsigned char* payload,int length) {
    const unsigned mode=read_byte_fresh(address(norrath_network_mode_byte_view));
    if(mode==0u) {
        const Address field=address(norrath_network_message_callbacks_view)+unsigned(handler)*4u;
        const Address entry=read_word_fresh(field);
        using MessageFunction=void (*)(unsigned char*,int);
        reinterpret_cast<MessageFunction>(static_cast<__UINTPTR_TYPE__>(entry))(payload,length);
        return;
    }
    const int player_number=norrath_net_get_local_player_number(0);
    const Address cursor_word=partner_field(address(norrath_network_send_cursor_words_view),player_number);
    const Address ring=partner_field(address(norrath_network_send_ring_bytes_view),player_number);
    const Address first_header=header_destination(ring,read_word_fresh(cursor_word));
    write_byte_ordered(first_header,unsigned(handler));
    const unsigned second_cursor=read_word_fresh(cursor_word)+1u;
    const Address second_header=header_destination(ring,second_cursor);
    // Both actual header stores happen even when length is zero or negative.
    write_byte_ordered(second_header,unsigned(length));
    int index=0;
    while(index<length) {
        const unsigned cursor=read_word_fresh(cursor_word);
        const unsigned value=read_byte_fresh(address(payload)+unsigned(index));
        const unsigned destination=(cursor+unsigned(index)+2u)&8191u;
        write_byte_ordered(ring+destination,value);
        index=signed_word(unsigned(index)+1u);
    }
    const unsigned final_cursor=(read_word_fresh(cursor_word)+unsigned(length)+2u)&8191u;
    write_word_ordered(cursor_word,final_cursor);
}
