import subprocess
import time


def start():
    p = subprocess.Popen(["./neblina", "-D", "../tests/init"], cwd="../neblina")
    time.sleep(0.2)
    assert p.poll() is None  # process is still running
    return p


def kill(process):
    process.kill()
    process.wait()
