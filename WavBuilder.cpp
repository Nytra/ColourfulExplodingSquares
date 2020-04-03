#include "WavBuilder.h"

WavBuilder::WavBuilder()
{
}

WavBuilder::~WavBuilder()
{
}

void WavBuilder::build(double time, double freq)
{
	std::ofstream f("sound.wav", std::ios::binary);

    // Write the file headers
    f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
    little_endian_io::writeWord(f, 16, 4);  // no extension data
    little_endian_io::writeWord(f, 1, 2);  // PCM - integer samples
    little_endian_io::writeWord(f, NUM_CHANNELS, 2);  // two channels (stereo file)
    little_endian_io::writeWord(f, SAMPLE_RATE, 4);  // samples per second (Hz)
    little_endian_io::writeWord(f, SAMPLE_RATE * BITS_PER_SAMPLE * NUM_CHANNELS, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
    little_endian_io::writeWord(f, BITS_PER_SAMPLE * 2 / 8, 2);  // data block size (size of two integer samples, one for each channel, in bytes)
    little_endian_io::writeWord(f, BITS_PER_SAMPLE, 2);  // number of bits per sample (use a multiple of 8)

    // Write the data chunk header
    size_t data_chunk_pos = f.tellp();
    f << "data----";  // (chunk size to be filled in later)

    // Write the audio samples

    //double hz = 44100;    // samples per second
    //double frequency = 75;  // middle C
    //double seconds = 5;      // time

    //double initValue = sin((two_pi * 0 * frequency) / hz);
    //double initFrequency = frequency;
    double value;
    //bool goingUp = true;
    //double oscillationAmt = 10;
    //double freqChange = 1000; // rate of oscillation
    //double timePassed = 0;
    //double rateOfReturn = 0.5;

    //int N = hz * seconds;  // total number of samples
    for (int n = 0; n < SAMPLE_RATE * time; n++)
    {
        //timePassed += 1;// / hz;
        //double amplitude = (double)n / N * max_amplitude;
        double amplitude = MAX_AMPLITUDE;
        //double value = sin((two_pi * n * frequency) / hz);
        //value = frequency * 3;
        value = sin((TWO_PI * n * freq) / SAMPLE_RATE);
        int sample = amplitude * value;
        little_endian_io::writeWord(f, sample, 2);
        little_endian_io::writeWord(f, sample, 2);
        //if (timePassed / hz >= 1) {
            //frequency += freqChange;
            //timePassed = 0;
        //}
        //frequency -= 0.01;//abs(initFrequency - freqChange) / rateOfReturn * hz;
        //if (frequency < initFrequency) {
            //frequency = initFrequency;
        //}
        //little_endian_io::writeWord(f, (int)((max_amplitude - amplitude) * value), 2);
        /*if (frequency <= initFrequency + oscillationAmt && goingUp) {
            frequency += freqChange;
        }
        else if (frequency >= initFrequency - oscillationAmt){
            goingUp = false;
            frequency -= freqChange;
        }
        else {
            goingUp = true;
        }*/
    }

    // (We'll need the final file size to fix the chunk sizes above)
    size_t file_length = f.tellp();

    // Fix the data chunk header to contain the data size
    f.seekp(data_chunk_pos + 4);
    little_endian_io::writeWord(f, file_length - data_chunk_pos + 8);

    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    f.seekp(0 + 4);
    little_endian_io::writeWord(f, file_length - 8, 4);

    printf("created new audio file\nfilename sound.wav\nsize %d kilobytes\n", file_length / 1000);
}

char* WavBuilder::getWavInMem(double time, double freq)
{
    //Mix_Chunk newChunk;
    //newChunk.allocated = 1;
    //newChunk.volume = 128;

    WavFile wav;

    //wav.chunkId = "RIFF";
    strcpy_s(wav.chunkId, sizeof(char), "RIFF");
    strcpy_s(wav.format, sizeof(char), "WAVE");
    strcpy_s(wav.subChunk1Id, sizeof(char), "fmt ");
    strcpy_s(wav.subChunk1Size, sizeof(char), std::to_string((char)16).c_str());
    strcpy_s(wav.audioFormat, sizeof(char), std::to_string((char)1).c_str());

    char* buf = nullptr;
    std::stringstream ss;
    ss << "RIFF----WAVEfmt ";
    little_endian_io::writeWord(ss, 16, 4);  // no extension data
    little_endian_io::writeWord(ss, 1, 2);  // PCM - integer samples
    little_endian_io::writeWord(ss, NUM_CHANNELS, 2);  // two channels (stereo file)
    little_endian_io::writeWord(ss, SAMPLE_RATE, 4);  // samples per second (Hz)
    little_endian_io::writeWord(ss, SAMPLE_RATE * BITS_PER_SAMPLE * NUM_CHANNELS, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
    little_endian_io::writeWord(ss, BITS_PER_SAMPLE * 2 / 8, 2);  // data block size (size of two integer samples, one for each channel, in bytes)
    little_endian_io::writeWord(ss, BITS_PER_SAMPLE, 2);  // number of bits per sample (use a multiple of 8)
    //// Write the data chunk header
    size_t data_chunk_pos = ss.tellp();
    ss << "data----";  // (chunk size to be filled in later)

    double value, amplitude;
    int sample;
    //char* c = nullptr;
    //int len = SAMPLE_RATE * time;
    
    for (int n = 0; n < SAMPLE_RATE * time; n++) {
        amplitude = MAX_AMPLITUDE;
        value = sin((TWO_PI * n * freq) / SAMPLE_RATE);
        sample = amplitude * value;
        //printf("writing %d\n", sample);
        //ss.str("");
        little_endian_io::writeWord(ss, sample, 1);
        //printf("%s\n", ss.str().c_str());
        //little_endian_io::writeWord(ss, sample, 2);
        //ss.read(&buf[n], sizeof(char));
        //printf("%s\n", ss.str().c_str());
        
        //ss.write((char*)sample, sizeof(sample));
        //ss.write((char*)sample, sizeof(sample));
        //buf[i] = 
    }

    //for (int i = 0; i < SAMPLE_RATE * time; i++) {
        //printf("reading %d\n", buf[i]);
    //}
    // (We'll need the final file size to fix the chunk sizes above)
    size_t file_length = ss.tellp();
    // Fix the data chunk header to contain the data size
    ss.seekp(data_chunk_pos + 4);
    little_endian_io::writeWord(ss, file_length - data_chunk_pos + 8);
    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    ss.seekp(0 + 4);
    little_endian_io::writeWord(ss, file_length - 8, 4);

    buf = new char[strlen(ss.str().c_str()) + 1];
    //for (int i = 0; i < file_length; i++) {
        //buf[i] = ss.str().c_str()[i];
    //}
    strcpy_s(buf, sizeof(*buf) * strlen(ss.str().c_str()) + 1, ss.str().c_str());
    //buf = new char[file_length];

    //strcpy_s(buf, sizeof(char), ss.str().c_str());
    //ss.read(buf, sizeof(char));
    //for (int i = 0; i < file_length; i++) {
        //buf[i] = ss.get();
    //}
    
    //buf = _buf;
    printf("created in-memory audio file\nsize %d kilobytes\n", file_length / 1000);
    //ss.read(reinterpret_cast<char*>(&v), sizeof(unsigned int));
    //newChunk.abuf = ss;
    //newChunk.alen = file_length;
    //v = new Uint8[ss.tellp()];
    //c = new char[file_length];
    //ss.read(c, sizeof(c));
    //for (Uint32 i = 0; i < newChunk.alen; i++) {
        //printf("read %s\n", std::to_string(c[i]));
    //}
    //printf("printing\n");
    //ss.read(reinterpret_cast<char*>(buf), sizeof(*buf));
    //for (int i = 0; i < file_length; i++) {
        //printf("%c\n", c[i]);
    //}
    //printf("%s\n", c);

    
    
    //v = (Uint8*)c;
    //v = (Uint8)atoi(c);
    //newChunk.abuf = (Uint8*)c;
    //*buf = (Uint8)*c;

    return buf;
}