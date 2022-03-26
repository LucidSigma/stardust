import os
import platform
import subprocess

if __name__ == '__main__':
    if not os.path.exists("./run_all_tests/target/release"):
        subprocess.run("cargo build --release", cwd = "./run_all_tests")

    OS_NAME = platform.system()
    
    if OS_NAME == "Windows":
        subprocess.run("./run_all_tests/target/release/run_all_tests.exe --type=debug", cwd = "..")
        subprocess.run("./run_all_tests/target/release/run_all_tests.exe --type=release", cwd = "..")
