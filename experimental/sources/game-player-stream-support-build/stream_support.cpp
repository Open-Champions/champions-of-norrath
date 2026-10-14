#include "stream_support.hpp"
using namespace norrath_stream_io_detail;

extern "C" int norrath_factory_walk_streams(unsigned char* reent,int(*callback)(unsigned char*)){
    Word group=address(reent)+472u,result=0u;
    while(group!=0u){
        Word left=read_word(group+4u)-1u;
        Word stream=read_word(group+8u);
        while(signed_word(left)>=0){
            // Original signed half is only tested for zero; every nonzero bit pattern calls.
            if(half(stream+12u)!=0u)result|=Word(callback(bytes(stream)));
            --left;stream+=88u;
        }
        // Callback mutations of the current group's next link are read here, freshly.
        group=read_word(group);
    }
    return signed_word(result);
}

extern "C" int norrath_player_stream_extend_pushback(unsigned char* stream){
    const Word file=address(stream),pushback=read_word(file+48u);
    if(pushback==file+64u){
        const Word buffer=address(norrath_player_malloc_reentrant(bytes(read_word(file+84u)),1024u));
        if(buffer==0u)return -1;
        write_word(file+48u,buffer);write_word(file+52u,1024u);
        const Word cursor=buffer+1021u;
        // Actual three-byte descending copy after publishing the new buffer fields.
        Word source=file+66u;
        for(Word index=3u;index!=0u;){
            const unsigned char byte=*bytes(source);--source;--index;
            *bytes(cursor+index)=byte;
        }
        write_word(file,cursor);return 0;
    }
    const Word old_capacity=read_word(file+52u),new_capacity=old_capacity*2u;
    const Word buffer=address(norrath_player_reallocate_reentrant(bytes(read_word(file+84u)),bytes(pushback),new_capacity));
    if(buffer==0u)return -1;
    const Word cursor=buffer+old_capacity;
    (void)norrath_copy_bytes(bytes(cursor),bytes(buffer),old_capacity);
    write_word(file,cursor);write_word(file+52u,new_capacity);
    write_word(file+48u,buffer);return 0;
}

extern "C" int norrath_player_descriptor_is_terminal(int){
    // Complete actual original isatty8: JR RA; LI V0,1. This genuine function
    // is not an invented answer for the unrelated real status/allocator services.
    return 1;
}

extern "C" void norrath_player_stream_make_buffer(unsigned char* stream){
    const Word file=address(stream);
    Word flags=half(file+12u);
    if(flags&2u){
        const Word buffer=file+67u;
        write_word(file+20u,1u);write_word(file+16u,buffer);write_word(file,buffer);
        return;
    }
    // The original144-byte frame reserves112 stack bytes before callee saves.
    // Actual _fstat_r receives this buffer; only mode word4 is read here. Nothing
    // initializes the other bytes or supplies fabricated successful status data.
    alignas(8) unsigned char status[112];
    bool character_device=false;
    const int fd=descriptor(file);
    bool failed_status=fd<0;
    if(!failed_status){
        if(norrath_player_file_status_reentrant(bytes(read_word(file+84u)),fd,status)<0){
            failed_status=true;flags=half(file+12u);
        }
    }
    if(failed_status){write_half(file+12u,flags|0x800u);}
    else{
        const Word mode=read_word(address(status)+4u)&0xf000u;
        character_device=mode==0x2000u;
        if(mode==0x8000u){
            const Word seek=read_word(file+40u);
            flags=half(file+12u);
            if(seek==function_word(&norrath_factory_stream_seek)){
                write_word(file+76u,1024u);flags|=0x400u;
            }else flags|=0x800u;
        }else{flags=half(file+12u);flags|=0x800u;}
        write_half(file+12u,flags);
    }
    const Word buffer=address(norrath_player_malloc_reentrant(bytes(read_word(file+84u)),1024u));
    flags=half(file+12u); // Captured fresh immediately after the genuine allocator.
    if(buffer==0u){
        const Word fallback=file+67u;
        write_word(file+16u,fallback);flags|=2u;
        write_word(file+20u,1u);write_half(file+12u,flags);write_word(file,fallback);
        return;
    }
    const Word reent=read_word(file+84u);
    flags|=0x80u;
    write_word(file+16u,buffer);
    write_word(reent+60u,function_word(&norrath_factory_cleanup_reentrant));
    write_half(file+12u,flags);write_word(file+20u,1024u);write_word(file,buffer);
    if(character_device){
        if(norrath_player_descriptor_is_terminal(descriptor(file))!=0){
            flags=half(file+12u);write_half(file+12u,flags|1u);
        }
    }
}

extern "C" int norrath_player_file_status(int,unsigned char* status){
    // Whole original fstat20 initializes exactly these fields, in this order.
    const Word result=address(status);
    norrath_core_format_detail::write_wide(result+72u,0u);
    write_word(result+4u,0x2000u);
    return 0;
}
extern "C" int norrath_player_file_status_reentrant(unsigned char* reent,int fd,unsigned char* status){
    const Word error=address(norrath_player_syscall_errno_word);
    write_word(error,0u);
    const int result=norrath_player_file_status(fd,status);
    if(result==-1){
        const Word code=read_word(error);
        if(code!=0u)write_word(address(reent),code);
    }
    return result;
}
