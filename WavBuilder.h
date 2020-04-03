#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <SDL_mixer.h>
#include <sstream>
#include <cstring>

namespace little_endian_io {
	template<typename Word>
	std::ostream& writeWord(std::ostream& _out, Word value, unsigned size = sizeof(Word)) {
		for (; size; --size, value >>= 8)
			_out.put(static_cast <char> (value & 0xFF));
		return _out;
	}
}

struct WavFile {
	char chunkId[4];
	char chunkSize[4];
	char format[4];

	char subChunk1Id[4];
	char subChunk1Size[4];
	char audioFormat[2];
	char numChannels[2];
	char sampleRate[4];
	char byteRate[4];
	char blockAlign[2];
	char bitsPerSample[2];

	char subChunk2Id[4];
	char subChunk2Size[4];
	char* data;
};

class WavBuilder
{
public:
	WavBuilder();
	~WavBuilder();
	void build(double time, double freq);
	char* getWavInMem(double time, double freq);
private:
	const int SAMPLE_RATE = 22050;
	const int BITS_PER_SAMPLE = 8;
	const int NUM_CHANNELS = 1;
	const double TWO_PI = 6.283185307179586476925286766559;
	const double MAX_AMPLITUDE = 32760;
};

