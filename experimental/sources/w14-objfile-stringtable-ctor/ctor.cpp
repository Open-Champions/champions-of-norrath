// Observed: __13VIStringTable at 0x0101bb78, 20 bytes. Stores zero at +12, +0, +4 and +8 of the object and returns nothing.
// Inferred: a table with a row count (+0), a column count (+4), an unknown word (+8) and a pointer to the text block (+12), as
// GetString__13VIStringTable reads them; zero means "not loaded".
// Unknown: the object size beyond +16.
extern "C" {
void __13VIStringTable(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    words[3] = 0;
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}
}
