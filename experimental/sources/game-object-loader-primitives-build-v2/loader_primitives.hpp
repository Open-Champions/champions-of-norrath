#pragma once
#include "dependencies.hpp"
extern "C" {
// Genuine loader-global/literal address views. No payload or class is emitted.
extern unsigned char norrath_loader_special_flag_view[];
extern unsigned char norrath_loader_active_buffer_view[];
extern unsigned char norrath_loader_active_size_view[];
extern unsigned char norrath_loader_active_marker_view[];
extern unsigned char norrath_loader_active_name_view[];
extern unsigned char norrath_loader_current_name_view[];
extern const unsigned char norrath_loader_name_a_view[];
extern const unsigned char norrath_loader_name_b_view[];
extern const unsigned char norrath_loader_context_a_view[];
extern const unsigned char norrath_loader_context_b_view[];
extern const unsigned char norrath_loader_missing_format_view[];
// The original failure instruction165d04 is SWzero0. This genuine hardware
// address view must bind to absolute0; it is not an ELF-owned object or payload.
// No runtime owner is granted by this declaration. A named external volatile
// word keeps the actual store/continuation visible without literal-null UB.
extern volatile unsigned norrath_loader_failure_word_view;
// Real external service functions. Their complete original bodies are pinned;
// no directory/CD/heap/printer success value is supplied by this packet.
int norrath_cd_directory_lookup(char* name, int* sector, int* bytes, int mode);
void* norrath_mem_alloc(int bytes, char* label);
void norrath_cd_load(char* name, void* destination, int bytes, int offset);
void norrath_cd_process();
int norrath_sce_printf(const char* format, ...);
}
