#include "wielding.hpp"
using namespace norrath_wielding_detail;

extern "C" char* norrath_factory_strstr(const char* input,const char* pattern) {
    Address candidate=address(input);
    const Address needle=address(pattern);
    // Original reads haystack first, then needle even for an empty haystack.
    int first=read_signed_byte_fresh(candidate);
    int head=read_signed_byte_fresh(needle);
    if(first==0)return head==0?text(candidate):nullptr;
    for(;;) {
        if(head==0)return text(candidate);
        if(head==read_signed_byte_fresh(candidate)) {
            unsigned offset=1u;
            for(;;) {
                const int next=read_signed_byte_fresh(needle+offset);
                if(next==0)return text(candidate);
                if(next!=read_signed_byte_fresh(candidate+offset))break;
                ++offset;
            }
        }
        ++candidate;
        if(read_signed_byte_fresh(candidate)==0)return nullptr;
        // Native reloads the first needle byte for each new candidate.
        head=read_signed_byte_fresh(needle);
    }
}
