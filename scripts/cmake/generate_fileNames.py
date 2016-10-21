import os
import shutil

fileLocation = "libxbee3-3.0.11_win32/modes"
 
ACCEPTABLE_FILE_TYPES = ('.cpp', '.h', '.txt')    

fastqS = [f for f in os.listdir(fileLocation) if f.endswith('.h')]
fastqSM = [f for f in os.listdir(fileLocation) if f.endswith('.cpp')]
fastqSMM = [f for f in os.listdir(fileLocation) if f.endswith('.c')]


f = open('CMakeListsTest.txt', 'w+')


for fileToWrite in fastqS:
	f.write(fileLocation + fileToWrite + '\n')

f.write('\n\n\n')

for fileToWrite in fastqSM:
	f.write(fileLocation + fileToWrite + '\n')
    
f.write('\n\n\n')

for fileToWrite in fastqSMM:
	f.write(fileLocation + fileToWrite + '\n')








