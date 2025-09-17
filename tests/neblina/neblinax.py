import subprocess

def start():
    return subprocess.Popen("./neblina", cwd="../neblina")

def kill(process):
    process.kill()
    process.wait()