"""EE object check for _3map$autoMapBitsSize."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-automap-bits/auto_map_bits_size.cpp"


class WordTests(support.CardTest):
    def test_object(self):
        _data, _sections, symbols, relocations = self.expect_object(
            SOURCE, '_3map$autoMapBitsSize', 4, content=b'\xc8\x16\x00\x00', imports=[])
        if '_3map$autoMapBitsSize' == "allActs":
            names = [item["symbol"] for item in relocations]
            self.assertEqual(names, ["act1", "act2", "act3", "act4", "act5"])


if __name__ == "__main__":
    unittest.main()
