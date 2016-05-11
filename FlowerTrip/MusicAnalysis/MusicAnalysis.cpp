// To get by errors for C-style file I/O
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES

// Have to do this because I can't seem to be able to set an array's length 
// with a variable, even if I specify it as const. This is used to evaluate 
// how many samples you should take at a time for each window, and this is 
// also the size of the FFT arrays
#define WINSIZE 8192

#include "kiss_fftr.h"
#include "Wav.hpp"
#include <fstream>
#include <iostream>
#include <vector>

typedef std::vector<std::vector<float>> ScaleData;

void LoadWavFile(const char* fname, Wav& song);
void CalculateFreqPower(int freqBandStart, int freqBandEnd, int divisions, float& freqPower);
void CalculateFrequencyBands(int freqBandStart, int freqBandEnd, int divisions, const Wav& song, std::vector<int>& freqBandIndices);
void Hann(short in, int index, int size, float& value);
void TakeSnapshots(int snapshotRate, const std::vector<int>& freqBandIndices, const Wav& song, ScaleData& scaleData);
void SaveSnapshots(int snapshotRate, const ScaleData& scaleData, const std::string& destinationPath);

void main() {
	std::string songPath = R"(X:\Music\Expander\Aura\auraLeftChannel.wav)";
	Wav song;
	LoadWavFile(songPath.c_str(), song);

	// Power to measure frequencies by
	float freqPower = powf(2.0f, 0.35f);
	// Target frequencies we want to shoot for
	int freqBandStart = 50;
	int freqBandEnd = 18000;
	// Divisions is the number of frequency separations we want for the spectrum
	int divisions = 16;
	// Actual indices corresponding to the WINSIZE
	std::vector<int> freqBandIndices;
	CalculateFrequencyBands(freqBandStart, freqBandEnd, divisions, song, freqBandIndices);

	// How fast do you want to take snapshots, in milliseconds
	int snapshotRate = 100;
	// Data structure we'll store all the changes in frequency
	ScaleData scaleData(freqBandIndices.size());
	TakeSnapshots(snapshotRate, freqBandIndices, song, scaleData);

	std::string destinationPath("ScaleData.txt");
	SaveSnapshots(snapshotRate, scaleData, destinationPath);

	std::cout << "Music analysis complete" << std::endl;
}

// Grabs info from a WAV file and puts it into a class
// Uses C-style because of the guide I referenced off of: http://rogerchansdigitalworld.blogspot.com/2010/05/how-to-read-wav-format-file-in-c.html
// WAV format: http://soundfile.sapp.org/doc/WaveFormat/wav-sound-format.gif
void LoadWavFile(const char* fname, Wav& wav) {
	FILE* fp = fopen(fname, "rb");

	fread(wav.chunkId, sizeof(char), 4, fp);
	// Without the '\0', printing to console will try and print out later characters
	wav.chunkId[4] = '\0';
	fread(&wav.chunkSize, sizeof(unsigned long), 1, fp);
	fread(wav.format, sizeof(char), 4, fp);
	wav.format[4] = '\0';

	fread(wav.subchunk1Id, sizeof(char), 4, fp);
	wav.subchunk1Id[4] = '\0';
	fread(&wav.subchunk1Size, sizeof(unsigned long), 1, fp);
	fread(&wav.audioFormat, sizeof(short), 1, fp);
	fread(&wav.numChannels, sizeof(short), 1, fp);
	fread(&wav.sampleRate, sizeof(unsigned long), 1, fp);
	fread(&wav.byteRate, sizeof(unsigned long), 1, fp);
	fread(&wav.blockAlign, sizeof(short), 1, fp);
	fread(&wav.bitsPerSample, sizeof(short), 1, fp);

	fread(wav.subchunk2Id, sizeof(char), 4, fp);
	wav.subchunk2Id[4] = '\0';
	// subchunk2Size returns the amount of WAV data in bytes
	fread(&wav.subchunk2Size, sizeof(unsigned long), 1, fp);
	// size is the actual number of shorts
	wav.size = wav.subchunk2Size / sizeof(short);
	wav.data = (short*)malloc(wav.subchunk2Size);
	fread(wav.data, sizeof(short), wav.size, fp);

	fclose(fp);
}

// The calculation behind finding freqPower; required some natural log math
// s * f^d = e where
// s = freqBandStart
// e = freqBandEnd
// d = divisions
// f = factor to find
// ln(s * f^d) = ln(e)
// ln(s) + ln(f^d) = ln(e)
// ln(s) + d * ln(f) = ln(e)
// d * ln(f) = ln(e) - ln(s)
// ln(f) = (ln(e) - ln(s)) / d
// f = e ^ ((ln(e) - ln(s)) / d)
void CalculateFreqPower(int freqBandStart, int freqBandEnd, int divisions, float& freqPower) {
	// Splitting up so I can visualize better
	freqPower = logf(freqBandEnd);
	freqPower = freqPower - logf(freqBandStart);
	freqPower = freqPower / divisions;
	freqPower = powf(expf(1), freqPower);
}

void CalculateFrequencyBands(int freqBandStart, int freqBandEnd, int divisions, const Wav& song, std::vector<int>& freqBandIndices) {
	float freqPower;
	CalculateFreqPower(freqBandStart, freqBandEnd, divisions, freqPower);

	// Find the corresponding indices reflecting the freqs after FFT.
	// FFT divides up the output into linear bins of frequencies. Each
	// value inside freqBandIndices represents the starting bin you should
	// look in. If you are not the last freqBand, see the next index to
	// determine the end range of bins you should search in.
	// The nth FFT bin is in n * sampleRate / WINSIZE frequency
	float freqConstant = (float)song.sampleRate / WINSIZE;
	for (float freq = freqBandStart; freq <= freqBandEnd; freq *= freqPower) {
		// freq = freqBandIndex * freqConstant
		int freqBandIndex = freq / freqConstant;
		freqBandIndices.push_back(freqBandIndex);
	}
}

// Hann function used for TakeSnapshots
void Hann(short in, int index, int size, float& value) {
	// Doing this split up shenanigans to show steps
	value = 2.0f * M_PI * index;
	value = value / (size - 1.0f);
	value = cos(value);
	value = 1 - value;
	value = value * 0.5f;
	value = value * in;
}

// Does the big bad job of running through samples,
// computing FFT and scaling bars
void TakeSnapshots(int snapshotRate, const std::vector<int>& freqBandIndices, const Wav& song, ScaleData& scaleData) {
	float snapshotsPerSec = 1000.0f / snapshotRate;
	// A little on the distinction between snapshots and progress:
	// Snapshot rate describes how often we're taking snapshots, and
	// progress rate describes how many respective samples we need to
	// progress through to match the snapshot rate
	// Note also that sample rate is given in Hz, samples per second
	float progressRate = song.sampleRate / snapshotsPerSec;

	// Find start and end points we take samples from
	// Need to account for window sizes, so we start and end with a bit of buffer space
	for (float p = progressRate; p < song.size - progressRate; p += progressRate) {
		// Displays progress at so and so seconds
		std::cout << "Processing sample at: " << (int) (p / song.sampleRate) << " seconds" << std::endl;

		// Grab WINSIZE amount of samples and apply Hann function to them
		float* input = (float*)malloc(WINSIZE * sizeof(float));
		for (int j = 0; j < WINSIZE; ++j) {
			// Taking data half below and after makes sure that our window
			// is centered on where we want to take our snapshot
			float data = song.data[j + (int)p - (WINSIZE / 2)];
			float hann;
			Hann(data, j, WINSIZE, hann);
			// Make sure to divide!! FFT takes input from -1 to 1 but WAV
			// gives short values between -32000~ to 32000~
			input[j] = hann / 32768.0f;
		}

		// Setup FFT and apply FFT to input
		// Since we are using real values only, I use the real version of kiss_fft
		kiss_fft_cpx out[WINSIZE / 2 + 1];
		kiss_fftr_cfg cfg = kiss_fftr_alloc(WINSIZE, 0, NULL, NULL);
		kiss_fftr(cfg, input, out);
		free(cfg);

		// Condense FFT output into frequency band information
		for (int j = 0; j < freqBandIndices.size(); ++j) {
			// Find bin indices
			int startBin = freqBandIndices[j];
			int endBin;
			if (j == freqBandIndices.size() - 1) {
				endBin = WINSIZE / 2 + 1;
			}
			else {
				endBin = freqBandIndices[j + 1];
			}

			// Track the max value out of the respective bins
			// You don't have to use max, but a resource I referenced used it: https://github.com/zardoru/osutk/blob/master/tools/musnalisys.py
			// Special thanks to Mr. Zardoru btw, helped point me in the right direction
			// to fix some frequency issues
			float maxMagSquared = 0.0f;
			for (int k = startBin; k <= endBin; ++k) {
				float magSquared = out[k].r * out[k].r + out[k].i * out[k].i;
				if (magSquared > maxMagSquared) {
					maxMagSquared = magSquared;
				}
			}

			// Scale the magnitude to a more reasonable log/dB scale
			float loggedMax = 10.0f * log10f(maxMagSquared);
			// log10 can return from negative infinity to 0, so this clamps negative values
			if (loggedMax < 0.0f) {
				loggedMax = 0.0f;
			}

			scaleData[j].push_back(loggedMax);
		}
	}
}

void SaveSnapshots(int snapshotRate, const ScaleData& scaleData, const std::string& destinationPath) {
	std::ofstream output(destinationPath);
	output << snapshotRate << std::endl;
	
	int bandCount = scaleData.size();
	output << bandCount << std::endl;

	int sampleCount = scaleData[0].size();
	output << sampleCount << std::endl;

	for (int i = 0; i < scaleData.size(); ++i) {
		for (int j = 0; j < scaleData[i].size(); ++j) {
			output << scaleData[i][j] << std::endl;
		}
	}

	output.close();
}