// Observed: zero-initialized dialog and option state in .sbss/.bss of the
// pinned SLUS-20565 image. Sizes come from the symbol spacing (each is four
// bytes before the next symbol) and from the word loads in the authored
// dialog message and draw sources.
//   showOptions 0x00486b24, showMore 0x00486b28, optionIndex 0x00486b34,
//   scrollTarget 0x00486b44, scrollY 0x00486b48, scrollEnd 0x00486b50,
//   nameChars 0x00486b5c, optionCount 0x00486b64, scrollSpan 0x00486b78
//   (a float), optionsShowing 0x00486b8c: all one word.
//   optionLinePos 0x007d0c70: 48 bytes, twelve ints; optTbl starts after it.
// Inferred: optionLinePos holds one row position per dialog option, indexed
// by optionCount values seen in the draw code. Unknown: the largest option
// count the dialog permits (twelve is only the room before optTbl) and what
// optionsShowing means; no authored function reads it yet.
extern "C" {
int showOptions;
int showMore;
int optionIndex;
int scrollTarget;
int scrollY;
int scrollEnd;
int nameChars;
int optionCount;
float scrollSpan;
int optionsShowing;
int optionLinePos[12];
}
static_assert(sizeof(int) == 4 && sizeof(float) == 4, "state words are four bytes");
