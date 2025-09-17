import unittest
import requests
import sys
import os

sys.path.insert(0, os.path.dirname(__file__))
import neblinax

class SimpleWebRequest(unittest.TestCase):

    def setUp(self):
        self.process = neblinax.start()

    def tearDown(self):
        pass
        neblinax.kill(self.process)

    def test_something(self):
        self.assertEqual(True, True)  # add assertion here