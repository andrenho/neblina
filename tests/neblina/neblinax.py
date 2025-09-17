import subprocess
import time


def start():
    p = subprocess.Popen(["./neblina", "-D", "../init"], cwd="../../neblina")
    time.sleep(0.5)
    assert p.poll() is None  # process is still running
    return p


def kill(process):
    process.kill()
    process.wait()
