import platform
from invoke import task

profile = "questions"

def invoke_run(c, task):
    if platform.system() == 'Linux':
        c.run(task, pty=True)
    else:
        c.run(task)

@task
def conanprofile(c):
    invoke_run(c, f"conan profile new {profile} --detect")
    if platform.system() == 'Linux':
        invoke_run(c, f"conan profile update settings.compiler.libcxx=libstdc++11 {profile}")

@task
def installdeps(c):
    invoke_run(c, f"conan install . -if build/deps/conan --build missing --profile {profile}")

@task
def cmake(c):
    if platform.system() == 'Linux':
        invoke_run(c, f"cmake -H. -Bbuild/release -DCMAKE_MODULE_PATH=$(pwd)/build/deps/conan -DCMAKE_BUILD_TYPE=RelWithDebInfo")
    elif platform.system() == 'Windows':
        invoke_run(c, f"cmake -H. -Bbuild/release -DCMAKE_MODULE_PATH=$PWD/build/deps/conan -DCMAKE_BUILD_TYPE=RelWithDebInfo")

@task
def cmakedebug(c):
    if platform.system() == 'Linux':
        invoke_run(c, f"cmake -H. -Bbuild/debug -DCMAKE_MODULE_PATH=$(pwd)/build/deps/conan -DCMAKE_BUILD_TYPE=Debug")
    elif platform.system() == 'Windows':
        invoke_run(c, f"cmake -H. -Bbuild/debug -DCMAKE_MODULE_PATH=$PWD/build/deps/conan -DCMAKE_BUILD_TYPE=Debug")

@task
def build(c):
    invoke_run(c, f"cmake --build build/release --config Release")

@task
def builddebug(c):
    invoke_run(c, f"cmake --build build/debug --config Debug")

@task
def clean(c):
    invoke_run(c, f"cmake --build build/release --target clean")

@task
def run(c):
    if platform.system() == 'Linux':
        invoke_run(c, f"./build/release/bin/viewer")
    else:
        raise NotImplementedError("running in windows is not working")
