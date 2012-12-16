"""
* Smartphone Brain Scanner 2 License Agreement (MIT License)
*
* Copyright (c) 2012 Arkadiusz Stopczynski, Jakob Eg Larsen, Carsten Stahlhut, Michael Kai Petersen, Lars Kai Hansen. 
* Technical University of Denmark, DTU Informatics, Cognitive Systems Section. http://code.google.com/p/smartphonebrainscanner2
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, 
* modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom 
* the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
"""


import gzip
import time
import datetime
import sys
import os
import logging
logger = logging.getLogger("emotiv")
import SocketServer
import struct
from threading import Thread



sensorBits = {
	'F3': [10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7], 
	'FC6': [214, 215, 200, 201, 202, 203, 204, 205, 206, 207, 192, 193, 194, 195], 
	'P7': [84, 85, 86, 87, 72, 73, 74, 75, 76, 77, 78, 79, 64, 65], 
	'T8': [160, 161, 162, 163, 164, 165, 166, 167, 152, 153, 154, 155, 156, 157], 
	'F7': [48, 49, 50, 51, 52, 53, 54, 55, 40, 41, 42, 43, 44, 45], 
	'F8': [178, 179, 180, 181, 182, 183, 168, 169, 170, 171, 172, 173, 174, 175], 
	'T7': [66, 67, 68, 69, 70, 71, 56, 57, 58, 59, 60, 61, 62, 63], 
	'P8': [158, 159, 144, 145, 146, 147, 148, 149, 150, 151, 136, 137, 138, 139], 
	'AF4': [196, 197, 198, 199, 184, 185, 186, 187, 188, 189, 190, 191, 176, 177], 
	'F4': [216, 217, 218, 219, 220, 221, 222, 223, 208, 209, 210, 211, 212, 213], 
	'AF3': [46, 47, 32, 33, 34, 35, 36, 37, 38, 39, 24, 25, 26, 27], 
	'O2': [140, 141, 142, 143, 128, 129, 130, 131, 132, 133, 134, 135, 120, 121], 
	'O1': [102, 103, 88, 89, 90, 91, 92, 93, 94, 95, 80, 81, 82, 83], 
	'FC5': [28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23, 8, 9]
}

cqBits = [104,105,106,107,108,109,110,111,96,97,98,99,100,101]
cqMapping = ["F3","FC5","AF3","F7","T7","P7","O1","O2","P8","T8","F8","AF4","FC6","F4"]






class EmotivPacket(object):
	def __init__(self, data):
		self.counter = ord(data[0])
		self.gyroX = (ord(data[29]) << 4 & 0xfff0) + (ord(data[31])>>4 & 0x0f)
		self.gyroY = (ord(data[30]) << 4 & 0xfff0) + (ord(data[31]) & 0x0f)
		self.battery = -1
		self.cqName = ""
		self.cq = -1
		
		index = self.counter % 64
		if index < 14:
			level = 0
			for i in range(13, -1, -1):
				level <<= 1
				b, o = (cqBits[i] / 8) + 1, cqBits[i] % 8
				level |= (ord(data[b]) >> o) & 1
			self.cqName = cqMapping[index]
			self.cq = level
        
		if self.counter > 127:
			self.battery = self.counter & 127
			self.counter = 128
        
		for name, bits in sensorBits.items():
			level = 0
			for i in range(13, -1, -1):
				level <<= 1
				b, o = (bits[i] / 8) + 1, bits[i] % 8
				level |= (ord(data[b]) >> o) & 1
			setattr(self, name, (level, 0))
	
    

class Emotiv(object):
	seeker=0
    
	def __init__(self, filePath):
        
		self.filePath = filePath
		self.goOn = True
		self.packets = []
        
		self.setup()
    
	
	def setup(self):
		def reader():
			brainLocation=self.filePath
			size = os.path.getsize(brainLocation)
			self.hidrawFile = open(brainLocation,'rb')
			while self.goOn:
				self.hidrawFile.seek(self.seeker)
				data = self.hidrawFile.read(32)
				if (self.seeker > size - 32):
					exit()
				self.seeker=self.seeker+32
				if data != "":
					self.gotData(data)
			
			self.hidrawFile.close()
        
		self.dataReader = Thread(target=reader)
		self.dataReader.start()
		return True
	
	def gotData(self, data):
		assert len(data) == 32
		self.packets.append(EmotivPacket(data))
	
	def dequeue(self):
		while len(self.packets):
			yield self.packets.pop(0)


if (len(sys.argv)>1):
	fileRaw = str(sys.argv[1]).split(".")[0]
else:
	print "ERROR: specify RAW file to decode"
	exit()

print "working on file "+str(fileRaw)

if (os.path.isfile(fileRaw+".raw") ==0 ):
	print "ERROR: RAW file does not exist"
	exit()

rawFileSize = os.path.getsize(fileRaw+".raw")
print "file size is "+str(rawFileSize)

if (rawFileSize%32 != 0):
	print "ERROR: the file size is not divisable by 32"
	exit()

rawNumberOfSamples = int(rawFileSize)/32


print "samples in the file "+str(rawNumberOfSamples)

fileMeta = str(fileRaw)+".meta"
print "META file is "+fileMeta

if (os.path.isfile(fileMeta) ==0 ):
	print "ERROR: META file does not exist"
	exit()


fMetaFile = open(fileMeta)

i=0
metaLines = []
for line in fMetaFile:
	metaLines.append(line)
	i=i+1


startTime = float(metaLines[1].split(":")[1])


try:
	reserved = int(metaLines[0].split(":")[3])
except ValueError:
	reserved = 1



if (isinstance(startTime, (float,int)) == False):
	print "ERROR: start time doesn't seem to be a number"
	exit()


divider = 1
if (startTime > 1000000000000):
	print "time in msec"
	divider = 1000

startTime = startTime/divider 

print "start time %.3f"%startTime

frequency = 128.0
print "frequency " + str(frequency)

timeTick = 1/float(frequency)
print "time tick " + str(timeTick)


readingNo = 1

outputFile = fileRaw + ".txt"
print "output file "+outputFile

headset = Emotiv(fileRaw+".raw")
f = open(outputFile,'w')
perc = 0
f.write("no timestamp gyroX gyroY P7 FC6 T7 P8 O2 O1 FC5 F3 F4 T8 F7 F8 AF4 AF3 counter cq\n")
loss = 0

while True:
	for packet in headset.dequeue():
		timing = startTime + (readingNo-1)* timeTick 

		f.write(str(readingNo) + " " + "%.0f"%(timing*1000) + " " + str(packet.gyroX) + " " + str(packet.gyroY) + " "+str(getattr(packet, "P7")[0]) + " " + str(getattr(packet, "FC6")[0]) + " " + str(getattr(packet, "T7")[0]) + " " + str(getattr(packet, "P8")[0]) + " " + str(getattr(packet, "O2")[0]) + " " + str(getattr(packet, "O1")[0]) + " " + str(getattr(packet, "FC5")[0]) + " " + str(getattr(packet, "F3")[0]) + " " + str(getattr(packet, "F4")[0]) + " " + str(getattr(packet, "T8")[0]) + " " + str(getattr(packet, "F7")[0]) + " " + str(getattr(packet, "F8")[0]) + " " + str(getattr(packet, "AF4")[0]) + " " + str(getattr(packet, "AF3")[0])+ " " + str(packet.counter) + " ")
		if packet.cq != -1:
			f.write(str(packet.cqName)+":"+str(packet.cq))

		f.write("\n")

		if readingNo > 1:
			currentLoss = (packet.counter - lastPacketNumber)%129-1
			loss += currentLoss

		lastPacketNumber = packet.counter			
	
	
	
		if readingNo == rawNumberOfSamples:
			print "finished, loss: %s (%s%%)"%(loss,loss/float(rawNumberOfSamples))
			exit()
		

		if readingNo % 100 == 0:
			perc = (readingNo/float(rawNumberOfSamples)*100.0)
		sys.stdout.write("%d%%\r" % (perc) )
		readingNo = readingNo + 1
	
