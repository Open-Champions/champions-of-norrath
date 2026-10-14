#pragma once

// doEntityDispatch__FP8MTObjDefiP13delayedEntityPib at 0x002695e0, size 0x59c.
// a0 record, a1 region index, a2 delayed-record base, a3 count word, t0 filter.
// This unit names the image's own symbols. megatile-do-entity-dispatch defines the same body under norrath_ alias names.
#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif
extern "C" {
extern unsigned char mt_tiledata[];         // 10-byte region rows
extern unsigned char pmlistptrs[];          // pointer list
extern unsigned char pmlistptrsused[];      // word: entries used
extern unsigned char kittencount[];         // word
extern unsigned char userFilterFunction[];  // word: function address or zero
extern const unsigned char _ctype_[];

int strncmp_(const char* left, const char* right, unsigned count) LIBRARY_SYMBOL(strncmp);
int strcasecmp_(const char* left, const char* right) LIBRARY_SYMBOL(strcasecmp);
char* strstr_(const char* text, const char* needle) LIBRARY_SYMBOL(strstr);
int sprintf_(char* output, const char* format, ...) LIBRARY_SYMBOL(sprintf);
char* addpmlist__FPc(char* text);
char** addpmlistptrscap__Fv();
char** specialparse__FPPciiG6Point3Pc(char** arguments, int selector, int mode, const unsigned char* point, char* name);
void monsterSpawn__FPPcG6Point3sb(char** arguments, const void* point, short type_word, bool filter);
unsigned char* objectCreateByName__FPcG6Point3sPPc(char* name, const unsigned char* point, int type_word, char** arguments);

void doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(unsigned char* record, int index, unsigned char* delayed, int* count, bool filter);
}

#if defined(NORRATH_MEGATILE_HOST)
extern "C" void* norrath_megatile_host_pointer(unsigned id);
extern "C" void norrath_megatile_host_note_word(const unsigned char* cell, unsigned value);
extern "C" void norrath_megatile_host_note_store(const unsigned char* cell, const void* value);
extern "C" int norrath_megatile_host_filter(char* name, unsigned code_word);
#endif
