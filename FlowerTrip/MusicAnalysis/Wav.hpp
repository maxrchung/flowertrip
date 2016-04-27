#ifndef WAV_HPP
#define WAV_HPP

#include <string>

class Wav {
public:
	char chunkId[5];
	unsigned long chunkSize;
	char format[5];
	char subchunk1Id[5];
	unsigned long subchunk1Size;
	short audioFormat;
	short numChannels;
	unsigned long sampleRate;
	unsigned long byteRate;
	short blockAlign;
	short bitsPerSample;
	char subchunk2Id[5];
	unsigned long subchunk2Size;
	unsigned long size;
	short* data;

	// Prints out all the Wav info minus the data
	std::string header();
};


#endif//WAV_HPP