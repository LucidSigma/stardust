import os
import platform
import subprocess

if __name__ == '__main__':
    if not os.path.exists("./copy_dlls/target/release"):
        subprocess.run("cargo build --release", cwd = "./copy_dlls")

    OS_NAME = platform.system()
    
    if OS_NAME == "Windows":
        subprocess.run("./copy_dlls/target/release/copy_dlls.exe --sdl2=2.0.20 --angle=2.1.0.5bd4596", cwd = "..")
