"""EE object check for pauseStackPos."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-pause-stack-pos/pause_stack_pos.cpp"


class WordTests(support.CardTest):
    def test_object(self):
        _data, _sections, symbols, relocations = self.expect_object(
            SOURCE, 'pauseStackPos', 1, content=b'\x00', imports=[])
        if 'pauseStackPos' == "allActs":
            names = [item["symbol"] for item in relocations]
            self.assertEqual(names, ["act1", "act2", "act3", "act4", "act5"])


if __name__ == "__main__":
    unittest.main()
