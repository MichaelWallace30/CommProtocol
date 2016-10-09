import os
import shutil

 
ACCEPTABLE_FILE_TYPES = ('.cpp', '.h', '.txt')    
    



fastqS = [f for f in os.listdir("cryptopp564/") if f.endswith('.h')]
fastqSM = [f for f in os.listdir("cryptopp564/") if f.endswith('.cpp')]


f = open('CMakeListsTest.txt', 'w+')



for fileToWrite in fastqS:
	f.write("${CRYPTOPP_HEADER_DIR}" + fileToWrite + '\n')

f.write(")\n\n\n\n\n")

for fileToWrite in fastqSM:
	f.write("${CRYPTOPP_SOURCE_DIR}" + fileToWrite + '\n')









