

#ifndef QGF_TOOLS_AUDIO_SYSTEM_H
#define QGF_TOOLS_AUDIO_SYSTEM_H 1

#ifdef _WIN32
#include "audiere.h"
#endif

typedef struct {
	float volume;
	float pitch;
	float pan;
	float pos;
#ifdef _WIN32
	audiere::OutputStreamPtr sound;
#endif
} qgf_stream;


int
qgf_tools_audio_system_open();

int
qgf_tools_audio_system_close();

qgf_stream*
qgf_tools_audio_system_openStream(char* file, int music);

void 
qgf_tools_audio_system_endStream(qgf_stream* sound);

void 
qgf_tools_audio_system_playStream(qgf_stream* sound);

void 
qgf_tools_audio_system_stopStream(qgf_stream* sound);

void 
qgf_tools_audio_system_updateStream(qgf_stream* sound);

void
qgf_tools_audio_system_updateInformation(qgf_stream* sound);

int
qgf_tools_audio_system_isPlaying(qgf_stream* sound);

#endif