import platform
import subprocess
import sys

OS_NAME = platform.system()

if OS_NAME == "Windows":
    if sys.argv[1] == "debug":
        subprocess.run("../build/bin/Debug/Sandbox.exe")
    elif sys.argv[1] == "release":
        subprocess.run("../build/bin/Release/Sandbox.exe")