/* Observed setSRSInfo__Fv at 0x001ca528, 672 bytes. It fills the 328-byte
 * netGameLaunchInfo (0x00333130), whose byte 0 is read as a flag byte, byte
 * 295 is written, and three strings are written at offsets 7, 135 and 199.
 *
 * - Offset 7 first receives the text "Kelethin" through sprintf.
 * - Two local ints start at 1.
 * - Flag bit 1 set: the string at playerSelectInfo (0x00438910) is copied to
 *   offset 135, the string indexed by the signed byte at playerSelectInfo + 32
 *   in the ten-pointer table at 0x004293e8 is copied to offset 199, and byte
 *   295 is set to 1. No player copy is made.
 * - Otherwise a 23984-byte local player image is filled with MC_ByteCopy. With
 *   flag bit 2 the source is MC_GetTempPlayerStorage(0) and the second local
 *   int is loaded from the word at 0x004852d8; otherwise the source is
 *   MC_GetRamPlayerPtr(0), MC_GetNetGameInfo(&first, &second, scratch) runs,
 *   and offset 7 gets sprintf("%s", scratch). Then, in both: the local image's
 *   string at offset 0 is copied to offset 135, the table string indexed by
 *   the int at image offset 84 to offset 199, and image byte 89 to byte 295.
 * - Second local int 2 or more: "(Courageous)" for exactly 2 and "(Champion)"
 *   for anything higher is built in a 128-byte local with sprintf("(%s)"),
 *   the offset-7 string is appended, and the result replaces the offset-7
 *   string.
 * - An offset-7 string of 23 or more characters is cut at offset 29 and "..."
 *   is appended. An offset-135 string of 10 or more characters is cut at
 *   offset 144 and "..." is appended.
 *
 * The original also initialises parts of the local player image before the
 * copy (a three-by-sixty-four table of entries with 16-bit and byte fields
 * set to -1 or 0). Every path either does not read the image or overwrites it
 * completely with the copy first, so those stores have no effect and are not
 * reproduced.
 *
 * The word at 0x004852d8 lies in .sdata at the start of a string whose text is
 * "Color"; reading it as an int is what the original does. What the word is
 * meant to hold is unknown. The ten-pointer table is a local of another
 * function (raceStrings.1135); the strings' meaning is inferred from the name
 * only. Both are imported by view names.
 */
extern "C" {
extern unsigned char netGameLaunchInfo[];
extern char playerSelectInfo[];
extern const char* norrath_sce_d_race_strings_view_4293e8[];
extern int norrath_sce_d_unknown_sdata_word_view_4852d8;

void* MC_GetTempPlayerStorage__Fi(int slot);
void* MC_GetRamPlayerPtr__Fi(int slot);
void MC_ByteCopy__FPvT0i(void* destination, void* source, int count);
void MC_GetNetGameInfo__FPiT0Pc(int* first, int* second, char* text);
int sprintf(char* destination, const char* format, ...);
char* strcpy(char* destination, const char* source);
char* strcat(char* destination, const char* source);
unsigned strlen(const char* text);

void setSRSInfo__Fv() {
    char scratch[256];
    int values[2];
    unsigned char* const info = netGameLaunchInfo;
    char* const name = reinterpret_cast<char*>(info) + 7;
    char* const owner = reinterpret_cast<char*>(info) + 135;
    char* const race = reinterpret_cast<char*>(info) + 199;
    values[0] = 1;
    values[1] = 1;
    alignas(16) unsigned char player[23984];
    char text[128];

    sprintf(name, "Kelethin");
    if (info[0] & 2) {
        strcpy(owner, playerSelectInfo);
        strcpy(race, norrath_sce_d_race_strings_view_4293e8[static_cast<signed char>(playerSelectInfo[32])]);
        info[295] = 1;
    } else {
        if (info[0] & 4) {
            MC_ByteCopy__FPvT0i(player, MC_GetTempPlayerStorage__Fi(0), 23984);
            values[1] = norrath_sce_d_unknown_sdata_word_view_4852d8;
        } else {
            MC_ByteCopy__FPvT0i(player, MC_GetRamPlayerPtr__Fi(0), 23984);
            MC_GetNetGameInfo__FPiT0Pc(&values[0], &values[1], scratch);
            sprintf(name, "%s", scratch);
        }
        strcpy(owner, reinterpret_cast<char*>(player));
        strcpy(race, norrath_sce_d_race_strings_view_4293e8[*reinterpret_cast<int*>(player + 84)]);
        info[295] = player[89];
    }
    if (values[1] >= 2) {
        sprintf(text, "(%s)", values[1] == 2 ? "Courageous" : "Champion");
        strcat(text, name);
        strcpy(name, text);
    }
    if (strlen(name) >= 23) {
        info[29] = 0;
        strcat(name, "...");
    }
    if (strlen(owner) >= 10) {
        info[144] = 0;
        strcat(owner, "...");
    }
}
}
