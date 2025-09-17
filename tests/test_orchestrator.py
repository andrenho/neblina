import unittest
import psutil
import os
import sys
import time

sys.path.insert(0, os.path.dirname(__file__))
import neblinax

class TestOrchestrator(unittest.TestCase):
    def setUp(self):
        self.process = neblinax.start()

    def tearDown(self):
        neblinax.kill(self.process)

    def test_kill_parent(self):
        parent = psutil.Process(self.process.pid)
        children = parent.children()
        neblinax.kill(self.process)
        time.sleep(0.2)
        for child in children:
            self.assertTrue(not psutil.pid_exists(child.pid))

    def test_child(self):
        parent = psutil.Process(self.process.pid)
        children = parent.children()
        n_children = len(children)
        neblinax.kill(children[0])
        time.sleep(0.3)
        children = parent.children()
        self.assertEqual(len(children), n_children)   # children should have been restarted

if __name__ == '__main__':
    unittest.main()
