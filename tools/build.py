import os
import platform
import subprocess

if __name__ == '__main__':
    if not os.path.exists("./build_project/target/release"):
        subprocess.run("cargo build --release", cwd = "./build_project")

    OS_NAME = platform.system()
    
    if OS_NAME == "Windows":
        subprocess.run("./build_project/target/release/build_project.exe", cwd = "..")
