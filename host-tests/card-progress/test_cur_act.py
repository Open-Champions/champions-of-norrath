"""EE object check for curAct."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-cur-act/cur_act.cpp"


class WordTests(support.CardTest):
    def test_object(self):
        _data, _sections, symbols, relocations = self.expect_object(
            SOURCE, 'curAct', 4, content=b'\x00\x00\x00\x00', imports=[])
        if 'curAct' == "allActs":
            names = [item["symbol"] for item in relocations]
            self.assertEqual(names, ["act1", "act2", "act3", "act4", "act5"])


if __name__ == "__main__":
    unittest.main()
