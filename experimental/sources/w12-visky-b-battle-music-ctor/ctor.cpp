// Observed: __13VIBattleMusic at 0x0106de00 is 8 bytes. With the object as this it stores zero in
// the word at this+0 and returns nothing.
// Inferred: that word is the object's only state at construction; the options window embeds the
// object at offset 1200 and the destructor calls Free on it.
// Unknown: the size of the object and the meaning of the first word.
extern "C" {
void __13VIBattleMusic(void* self) {
    *static_cast<unsigned*>(self) = 0;
}
}
