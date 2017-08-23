#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "examples/minisdl_audio.h"

const static unsigned char MINIMAL_SOUND_FONT[] = {
#define TEN0 0,0,0,0,0,0,0,0,0,0
	'R','I','F','F',220,1,0,0,'s','f','b','k',
	'L','I','S','T',88,1,0,0,'p','d','t','a',
	'p','h','d','r',76,TEN0,TEN0,TEN0,TEN0,0,0,0,0,TEN0,0,0,0,0,0,0,0,255,0,255,0,1,TEN0,0,0,0,
	'p','b','a','g',8,0,0,0,0,0,0,0,1,0,0,0,'p','m','o','d',10,TEN0,0,0,0,'p','g','e','n',8,0,0,0,41,0,0,0,0,0,0,0,
	'i','n','s','t',44,TEN0,TEN0,0,0,0,0,0,0,0,0,TEN0,0,0,0,0,0,0,0,1,0,
	'i','b','a','g',8,0,0,0,0,0,0,0,2,0,0,0,'i','m','o','d',10,TEN0,0,0,0,
	'i','g','e','n',12,0,0,0,54,0,1,0,53,0,0,0,0,0,0,0,
	's','h','d','r',92,TEN0,TEN0,0,0,0,0,0,0,0,50,0,0,0,0,0,0,0,49,0,0,0,34,86,0,0,60,0,0,0,1,TEN0,TEN0,TEN0,TEN0,0,0,0,0,0,0,0,
	'L','I','S','T',112,0,0,0,'s','d','t','a','s','m','p','l',100,0,0,0,86,0,119,3,31,7,147,10,43,14,169,17,58,21,189,24,73,28,204,31,73,35,249,38,46,42,71,46,250,48,150,53,242,55,126,60,151,63,108,66,126,72,207,
	70,86,83,100,72,74,100,163,39,241,163,59,175,59,179,9,179,134,187,6,186,2,194,5,194,15,200,6,202,96,206,159,209,35,213,213,216,45,220,221,223,76,227,221,230,91,234,242,237,105,241,8,245,118,248,32,252
};

static tsf* ptsf = TSF_NULL;

static void audio_callback(void* data, Uint8* stream, int len) {
	int sampleCount = (len / (2 * sizeof(short)));
	tsf_render_short(ptsf, (short*)stream, sampleCount, 0);
}

int main(int argc, char* argv[]) {
	SDL_AudioSpec outputAudioSpec;
	outputAudioSpec.freq = 44100;
	outputAudioSpec.format = AUDIO_S16;
	outputAudioSpec.channels = 2;
	outputAudioSpec.samples = 4096;
	outputAudioSpec.callback = audio_callback;

	if (argc == 1) {
		fprintf(stdout, "Play what?\n");

		return 1;
	}

	if (SDL_AudioInit(NULL) < 0) {
		fprintf(stderr, "Could not initialize audio hardware or driver.\n");

		return 1;
	}

	ptsf = tsf_load_memory(MINIMAL_SOUND_FONT, sizeof(MINIMAL_SOUND_FONT));
	tsf_set_output(ptsf, TSF_STEREO_INTERLEAVED, 44100, 0);

	if (SDL_OpenAudio(&outputAudioSpec, NULL) < 0) {
		fprintf(stderr, "Could not open the audio hardware or the desired audio output format.\n");

		return 1;
	}

	SDL_PauseAudio(0);

	fprintf(stdout, "Playing \"%s\".\n", argv[1]);

	tsf_play(ptsf, 0, argv[1], 0.8f, SDL_Delay);

	fprintf(stdout, "Done.\n");

	tsf_close(ptsf);

	SDL_CloseAudio();

	SDL_AudioQuit();

	return 0;
}
