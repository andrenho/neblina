import subprocess
import time


def start():
    p = subprocess.Popen("./neblina", cwd="../../neblina")
    time.sleep(0.5)
    return p


def kill(process):
    process.kill()
    process.wait()
