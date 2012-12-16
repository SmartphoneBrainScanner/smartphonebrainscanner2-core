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


import matplotlib.pyplot as plt
import numpy
import sys
from scipy import stats


try:
	file_name = str(sys.argv[1])
except:
	print "specify txt file"
	exit()


audio_sampling_rate = 22050.0 * 2
eeg_sampling_rate = 128.0

delta_correction = 0

file_raw = file_name.split(".")[0]

file_txt = file_raw + ".txt"
file_pcm = file_raw + ".pcm"
file_meta = file_raw + ".meta"


f = open(file_meta)


audio_start = 0
audio_stop = 0

for line in f:
	if line.split(":")[0] == "audio_start": 
		audio_start = int(line.split(":")[2])
	if line.split(":")[0] == "audio_stop":
		audio_stop = int(line.split(":")[2])

audio_start -= delta_correction

data = numpy.memmap(file_pcm, dtype='h', mode='r')

audio_duration = len(data)/audio_sampling_rate
print "audio samples: ",len(data), "duration: ", audio_duration


audio_stop = int(audio_start + audio_duration * eeg_sampling_rate)

eeg_duration = (audio_stop - audio_start)/eeg_sampling_rate
print audio_start, audio_stop, "eeg with audio signal duration: ", eeg_duration


f = open (file_txt)
channel = []

for i in range(0,14):
	channel.append([])



for line in f:
	try:
		for i in range(0,14):
			v = float(line.split(" ")[i+4])
			channel[i].append(v)
	except:
		pass


u = []


for c in channel:
	c = c[audio_start:audio_stop]
	u.append(c)

channel = u





#16 bit signed, little-endian, 1 channel, 44100 Hz






plt.figure(1)
plt.subplot(211).set_xlim(0,len(data))
plt.plot(numpy.arange(0,len(data),1),data)



for c in channel:
	plt.subplot(212).set_xlim(0,len(c))
	plt.plot(numpy.arange(0,len(c),1),c)

plt.show()
