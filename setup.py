"""
    Setup script for CommProtocol.

    Copyright (C) 2016  Mario Garcia, Michael Wallace, Alex Craig.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (At your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
# Still figuring out the tools to work with.
import os
import time
import sys
import scripts.editor as editor

def InstallCMake():
    """
    Install CMake on this machine.
    :return:
    """
    return

def CheckCMake():
    """
    Check if Cmake is on this machine.
    :return:
    """
    return

def CheckOS(cmake_cmd):
    """
    Check the operating system that this script is setting up on.
    :return:
    """
    editor.VersionEdit("cat")
    platform = sys.platform
    if platform.startswith('linux'):
        print("Running on Linux")
        # nothing to do...
    elif platform.startswith('win32'):
        print("Running on Windows")
        usr = input("*\n*\n*\nDo you wish to build the C# module as well? (Y/n): ")
        if usr.lower() == "y" or usr.lower() == "yes":
            print("Building with the C# module...")
            cmake_cmd = cmake_cmd + " -Dcsharp=ON"
        else:
            print("Skipping C# build")
            cmake_cmd = cmake_cmd + " -Dcsharp=OFF"
    return cmake_cmd

def main():
    """
    Sets up the build folder for the user.
    :return:
    """
    build_dir = "build"
    cmake_cmd = "cmake"
    print("Setting up Git Repository.")
    path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(path)
    if os.path.isdir(build_dir):
        print("build folder already exists!")
        print("Overwriting old build folder.")
    else:
        print("Creating build folder...")
        os.mkdir(build_dir)
    usr = input("*\n*\n*\nDo you wish to add in tests? (Y/n): ")
    if usr.lower() == "y" or usr.lower() == "yes":
        print("Adding test cases...")
        cmake_cmd = cmake_cmd + " -Dtest=ON"
    else:
        cmake_cmd = cmake_cmd + " -Dtest=OFF"
    cmake_cmd = CheckOS(cmake_cmd)
    start = time.monotonic()
    os.chdir(build_dir)
    print("Executing cmake...")
    os.system(cmake_cmd + " -Dversion_release=ON -Dshared=ON"  + " ../")
    print("CMake finished.")
    end = time.monotonic()
    print("Build successfully finished. Took " + str((end - start)) + " sec")
    return 0


if __name__ == '__main__':
    main()