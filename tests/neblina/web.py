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
        neblinax.kill(self.process)

    def test_something(self):
        r = requests.get("http://localhost:8080/health")
        print(r.headers)
        print(r.text)