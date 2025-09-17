import unittest
import requests
import sys
import os

sys.path.insert(0, os.path.dirname(__file__))
import neblinax

start_neblina = True

class TestHttpService(unittest.TestCase):

    def setUp(self):
        if start_neblina:
            self.process = neblinax.start()

    def tearDown(self):
        if start_neblina:
            neblinax.kill(self.process)

    def test_simple_call(self):
        r = requests.get("http://localhost:8080/health")
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers["Content-Type"].upper(), "TEXT/PLAIN")
        self.assertTrue("Content-Length" in r.headers)
        self.assertTrue("Date" in r.headers)
        self.assertEqual(r.text, "Success\r\n")

    def test_redirect(self):
        r = requests.get("http://localhost:8080/google", allow_redirects=False)
        self.assertEqual(r.status_code, 301)
        self.assertEqual(r.headers["Location"], "https://www.google.com")

    def test_etag(self):
        r = requests.get("http://localhost:8080", allow_redirects=False)
        self.assertTrue("ETag" in r.headers)
        r = requests.get("http://localhost:8080", headers={ "If-None-Match": r.headers["ETag"] })
        self.assertEqual(r.status_code, 304)
