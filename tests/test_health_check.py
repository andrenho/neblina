import unittest
import requests
import sys
import os

sys.path.insert(0, os.path.dirname(__file__))
import neblinax


class HealthCheck(unittest.TestCase):

    def setUp(self):
        self.process = neblinax.start()

    def tearDown(self):
        neblinax.kill(self.process)

    def test_simple_call(self):
        r = requests.get("http://localhost:8080/health")
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers["Content-Type"].upper(), "TEXT/PLAIN")
        self.assertTrue("Content-Length" in r.headers)
        self.assertTrue("Date" in r.headers)
        self.assertEqual(r.text, "Success\r\n")
