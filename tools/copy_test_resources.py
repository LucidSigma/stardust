import os
import platform
import subprocess

if __name__ == '__main__':
    if not os.path.exists("./copy_test_resources/target/release"):
        subprocess.run("cargo build --release", cwd = "./copy_test_resources")

    OS_NAME = platform.system()
    
    if OS_NAME == "Windows":
        subprocess.run("./copy_test_resources/target/release/copy_test_resources.exe", cwd = "..")
