"""EE object check for actLevelCount."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-act-level-count/act_level_count.cpp"


class WordTests(support.CardTest):
    def test_object(self):
        _data, _sections, symbols, relocations = self.expect_object(
            SOURCE, 'actLevelCount', 20, content=b'\x16\x00\x00\x00\n\x00\x00\x00\x12\x00\x00\x00\t\x00\x00\x00\x05\x00\x00\x00', imports=[])
        if 'actLevelCount' == "allActs":
            names = [item["symbol"] for item in relocations]
            self.assertEqual(names, ["act1", "act2", "act3", "act4", "act5"])


if __name__ == "__main__":
    unittest.main()
