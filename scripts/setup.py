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

def main():
    """
    Sets up the build folder for the user.
    :return:
    """
    print("Setting up Git Repository.")
    os.chdir("../")
    if os.path.isdir("build"):
        print("build folder already exists!")
        usr = input("Does the user wish to overwrite this build folder?")
        if usr.lower() == "y" or usr.lower() == "yes":
            print("Overwriting old build folder.")
        else:
            print("Routing to another build folder...")
    else:
        print("Creating build folder...")
        os.mkdir("build")
        print("build folder created!")

    return 0


if __name__ == '__main__':
    main()