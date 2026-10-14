#include "allocator_support.hpp"
using namespace norrath_allocator_support_detail;

extern "C" void norrath_player_malloc_lock(unsigned char*) {
    const Wide thread=norrath_player_thread_id_bits();
    const Word owner=address(norrath_player_malloc_lock_owner_view),count=address(norrath_player_malloc_lock_count_view);
    if(extend_signed_word(read_word(owner))!=thread) {
        (void)norrath_player_wait_semaphore(signed_word(read_word(address(norrath_player_malloc_semaphore_view))));
        write_word(owner,Word(thread));
    }
    write_word(count,read_word(count)+1u);
}

extern "C" void norrath_player_malloc_unlock(unsigned char*) {
    const Word count=address(norrath_player_malloc_lock_count_view);
    const Word next=read_word(count)-1u;
    write_word(count,next);
    if(next==0u) {
        const int semaphore=signed_word(read_word(address(norrath_player_malloc_semaphore_view)));
        write_word(address(norrath_player_malloc_lock_owner_view),0xffffffffu);
        (void)norrath_player_signal_semaphore(semaphore);
    }
}

extern "C" Wide norrath_player_adjust_break_reentrant(unsigned char* reent,int increment) {
    const Word error=address(norrath_player_syscall_errno_word);
    write_word(error,0u);
    const Wide result=norrath_player_adjust_break(increment);
    if(result==0xffffffffffffffffull) {
        const Word code=read_word(error);
        if(code!=0u)write_word(address(reent),code);
    }
    return result;
}

extern "C" void norrath_player_malloc_extend_top(unsigned char* reent,Word requested) {
    const Word bins=address(norrath_player_malloc_bins_view);
    const Word break_base=address(norrath_player_malloc_break_base_view),current=address(norrath_player_malloc_current_info_view);
    const Word old_top=read_word(bins+8u);
    Word old_size=read_word(old_top+4u)&0xfffffffcu;
    const Wide padded=Wide(requested)+read_wide(address(norrath_player_malloc_top_pad_view))+16ull;
    const Word base=read_word(break_base),old_end=old_top+old_size;
    Word extension=Word(padded);
    if(base!=0xffffffffu)extension=Word((Wide(extension)+4095ull)&0xfffffffffffff000ull);
    const Wide first=norrath_player_adjust_break_reentrant(reent,signed_word(extension));
    if(first==0xffffffffffffffffull)return;
    if(first<extend_signed_word(old_end)&&old_top!=bins)return;
    const Word arena=read_word(current)+extension;
    write_word(current,arena);
    if(first==extend_signed_word(old_end)) {
        const Word top=read_word(bins+8u);
        write_word(top+4u,(extension+old_size)|1u);
    } else {
        if(read_word(break_base)==0xffffffffu)write_word(break_base,Word(first));
        else write_word(current,arena+Word(first)-old_end);
        const Word misalignment=(Word(first)+8u)&15u;
        Word correction=misalignment==0u?0u:16u-misalignment;
        const Word aligned=Word(first)+correction;
        correction+=4096u-((aligned+extension)&4095u);
        const Wide second=norrath_player_adjust_break_reentrant(reent,signed_word(correction));
        if(second==0xffffffffffffffffull)return;
        const Word top_header=(Word(second)-aligned+correction)|1u;
        write_word(current,read_word(current)+correction);
        write_word(bins+8u,aligned);write_word(aligned+4u,top_header);
        if(old_top!=bins) {
            if(old_size<16u) {write_word(aligned+4u,1u);return;}
            const Word old_header=read_word(old_top+4u);
            old_size=(old_size-12u)&0xfffffff0u;
            const Word fence=old_top+old_size;
            write_word(old_top+4u,(old_header&1u)|old_size);
            write_word(fence+8u,5u);write_word(fence+4u,5u);
            if(old_size>=16u)norrath_player_free_reentrant(reent,bytes(old_top+8u));
        }
    }
    const Wide total=extend_signed_word(read_word(current));
    const Word maximum_break=address(norrath_player_malloc_max_break_view);
    if(read_wide(maximum_break)<total)write_wide(maximum_break,total);
    const Word maximum_total=address(norrath_player_malloc_max_total_view);
    if(read_wide(maximum_total)<total)write_wide(maximum_total,total);
}

extern "C" int norrath_player_malloc_trim_reentrant(unsigned char* reent,Word pad) {
    norrath_player_malloc_lock(reent);
    const Word bins=address(norrath_player_malloc_bins_view);
    const Word old_top=read_word(bins+8u),old_size=read_word(old_top+4u)&0xfffffffcu;
    const Wide amount=(((Wide(old_size)-Wide(pad)+4079ull)>>12)-1ull)<<12;
    if(signed_wide(amount)<4096) {norrath_player_malloc_unlock(reent);return 0;}
    const Wide end=norrath_player_adjust_break_reentrant(reent,0);
    const Word top=read_word(bins+8u);
    if(end!=extend_signed_word(top+old_size)) {norrath_player_malloc_unlock(reent);return 0;}
    const Wide result=norrath_player_adjust_break_reentrant(reent,signed_word(0u-Word(amount)));
    const Word current=address(norrath_player_malloc_current_info_view);
    if(result!=0xffffffffffffffffull) {
        const Word fresh_top=read_word(bins+8u);
        write_word(fresh_top+4u,Word(Wide(old_size)-amount)|1u);
        write_word(current,read_word(current)-Word(amount));
        norrath_player_malloc_unlock(reent);return 1;
    }
    const Wide recovered_end=norrath_player_adjust_break_reentrant(reent,0);
    const Word fresh_top=read_word(bins+8u),recovered=Word(recovered_end)-fresh_top;
    if(signed_word(recovered)>=16) {
        const Word base=read_word(address(norrath_player_malloc_break_base_view));
        write_word(current,Word(recovered_end)-base);
        write_word(fresh_top+4u,recovered|1u);
    }
    norrath_player_malloc_unlock(reent);return 0;
}
