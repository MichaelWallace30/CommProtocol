"""
Simple repo update versioning for the CommProtocol Library

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
import os
import sys
import shutil
from urllib.request import urlopen
from urllib.error import HTTPError
import editor

# This will likely need to change
repo = {"version":"v0.0.0", "parent_repo":"https://github.com/MichaelWallace30/CommProtocol", "filename":""}

def DownloadZipFile():
    """
    Download the zip file from Github repo of CommProtocol.
    :return: Nothing.
    """
    url = repo["parent_repo"] + repo["filename"] + ".zip"
    try:
        file = urlopen(url) # Download the zip file from the link and the filename
    except HTTPError as e:
        if e.code == 404:
            print("Error: Could not find file from url: '%s'" % repo["parent_repo"])
        print("Https request failed, error code: " + str(e.code) + ", reason: " + e.read())
        sys.exit(-1)
    print("Commencing download...")
    # After downloading...
    with open(os.path.basename(url), "wb") as local_file:
        # local file gets downloaded into a directory
        local_file.write(file.read())
    print("Finished downloading!")
    ZipInstall(local_file)

def ZipInstall(file):
    """
    :param file: File of which to extract the zip file with.
    :return:
    """
    print("Unpacking zip...")
    socket = 1
    str = "kit cat"
    editor.VersionEdit(str)

def main():
    """
    Main function.
    :return:
    """
    DownloadZipFile()

if __name__ == '__main__':
    main()
