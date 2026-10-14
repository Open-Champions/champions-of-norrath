"""EE object checks for the w9-frontend-d data sources. Not a partial-build input."""

import struct
import unittest

import support

S = support.SOURCES


def zeros(n):
    return bytes(n)


class DataTests(support.ObjectTest):
    def test_state_words(self):
        data, sections, symbols, relocations = support.compile_ee(S / "w9-frontend-d-state-words/state_words.cpp")
        sizes = {
            "requestToExit": 4, "restrictChars": 4, "saveOk": 4, "startFrame": 4, "textBodyLen": 4,
            "waiting": 4, "waitingForStreamSpool": 4, "startExitFade": 1, "scriptFrame": 4,
            "startInhibit": 4, "startScorePos": 2, "texturesAreLoading": 1, "texturesAreDecompressing": 1,
        }
        for name, size in sizes.items():
            row = support.defined(symbols, name)
            self.assertEqual(row["size"], size, name)
            self.assertEqual(support.object_bytes(data, sections, row), zeros(size), name)
        self.assertEqual(support.undefined(symbols), [])
        self.assertEqual(len(relocations), 0)

    def test_buffers(self):
        data, sections, symbols, relocations = support.compile_ee(S / "w9-frontend-d-buffers/buffers.cpp")
        for name, size in {"rumphole": 96, "savedScene": 56, "slotInfoHeaders": 1120,
                           "tempPlayerStruct": 95936}.items():
            row = support.defined(symbols, name)
            self.assertEqual(row["size"], size, name)
            self.assertEqual(support.object_bytes(data, sections, row), zeros(size), name)
        self.assertEqual(support.undefined(symbols), [])

    def test_launcher_statics(self):
        data, sections, symbols, relocations = support.compile_ee(
            S / "w9-frontend-d-launcher-statics/launcher_statics.cpp")
        for name, size in {"sGameOptions": 1024, "sCdRmode": 8}.items():
            row = support.defined(symbols, name)
            self.assertEqual(row["size"], size, name)
            self.assertEqual(support.object_bytes(data, sections, row), zeros(size), name)
        self.assertEqual(support.undefined(symbols), [])

    def test_tex_strings(self):
        data, sections, symbols, relocations = support.compile_ee(S / "w9-frontend-d-tex-strings/tex_strings.cpp")
        row = support.defined(symbols, "texStrings")
        self.assertEqual(row["size"], 40)
        # Each of the ten words is relocated against the string section.
        words = [r for r in relocations if r["section"].startswith(".rel") and r["offset"] < 40]
        self.assertEqual(len(words), 10)
        self.assertEqual([r["offset"] for r in words], list(range(0, 40, 4)))
        rodata = [s for s in sections if s["name"].startswith(".rodata")]
        blob = b"".join(data[s["offset"]:s["offset"] + s["size"]] for s in rodata)
        for name in (b"b_bm.tex", b"b_bf.tex", b"b_wem.tex", b"b_wef.tex", b"b_hem.tex",
                     b"b_hef.tex", b"b_em.tex", b"b_ef.tex", b"b_dem.tex", b"b_def.tex"):
            self.assertIn(name + b"\0", blob)
        self.assertEqual(support.undefined(symbols), [])


if __name__ == "__main__":
    unittest.main()
