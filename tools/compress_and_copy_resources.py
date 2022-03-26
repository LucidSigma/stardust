import os
import platform
import subprocess

if __name__ == '__main__':
    if not os.path.exists("./compress_and_copy_resources/target/release"):
        subprocess.run("cargo build --release", cwd = "./compress_and_copy_resources")

    OS_NAME = platform.system()
    
    if OS_NAME == "Windows":
        subprocess.run("./compress_and_copy_resources/target/release/compress_and_copy_resources.exe", cwd = "..")
