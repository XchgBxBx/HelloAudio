#include <stdio.h>
#include "portaudio.h"

typedef struct
{
	float gain;
}
paTestData;


static int paTestCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
{
	paTestData *data = (paTestData*)userData;
	float *out = (float*)outputBuffer;
	unsigned int i;
	//(void)inputBuffer;
	
	float wave = 0.0f;

	for(i = 0; i < framesPerBuffer; i++)
	{
		if((i % 50) > 25)
			wave = 0.3f;
		else
			wave = -0.3f;

		wave *= data->gain;

		*out++ = wave;
		*out++ = wave;

//		*out++ = data->leftPhase;
//		*out++ = data->rightPhase;
//
//		data->leftPhase += 0.003f;
//
//		if(data->leftPhase >= 0.5f)
//			data->leftPhase -= 1.0f;
//
//
//		data->rightPhase += 0.003f;
//
//		if(data->rightPhase >= 0.5f)
//			data->rightPhase -= 1.0;
	}
	return 0;
}


int main(int argc, char **argv)
{
	PaError err;
	err = Pa_Initialize();
	if(err != paNoError)
		goto error;

	PaStream *stream;
	paTestData data;

	err = Pa_OpenDefaultStream(
			&stream,
			0,		// input channels
			2,		// output channels
			paFloat32,	// data resolution
			44100,		// sample rate
			256,		// frames per buffer
			paTestCallback,	// callback
			&data);		// our data
	if(err != paNoError)
		goto error;

	err = Pa_StartStream(stream);
	if(err != paNoError)
		goto error;

	data.gain = 0.5f;
	Pa_Sleep(5000);
	data.gain = 0.2f;
	Pa_Sleep(5000);

	err = Pa_StopStream(stream);
	if(err != paNoError)
		goto error;

	err = Pa_CloseStream(stream);
	if(err != paNoError)
		goto error;


	err = Pa_Terminate();
	if(err != paNoError)
		goto error;


	return 0;
error:
	printf("PA ERROR: %s\n", Pa_GetErrorText(err));
	return 0;
}
