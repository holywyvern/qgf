#include "qgf/tools/audio_system.h"
#include "qgf/tools/file_finder.h"
#include "ruby.h"

#ifdef _WIN32
static audiere::AudioDevicePtr qgf_device;
#endif

int
qgf_tools_audio_system_open() {
	qgf_device = audiere::OpenDevice();
	return qgf_device ? 1 : 0;
}

int
qgf_tools_audio_system_close() {
	qgf_device = NULL;
	return 1;
}

qgf_stream*
qgf_tools_audio_system_openStream(char* file, int music) {
	std::string fn = std::string(file);
	char* filename = (music) ? qgf_tools_file_finder_find_music((char*)fn.c_str()) : qgf_tools_file_finder_find_sound((char*)fn.c_str());
	if (!filename) {
		return 0;
	}
	qgf_stream* stream = new qgf_stream();
	stream->pan = 0;
	stream->pitch = 100.0f;
	stream->pos = 0.0f;
	stream->volume = 100.0f;
#ifdef _WIN32
	if (!qgf_device) {
		qgf_device = audiere::OpenDevice();
	}
	audiere::OutputStreamPtr p = audiere::OpenSound(qgf_device, std::string(filename).c_str(), music ? true : false);
	if (!p) {
		free(stream);
		return NULL;
	}
	p->setRepeat(music ? true : false);
	stream->sound = p;
#endif
	free(filename);
	return stream;
}

void 
qgf_tools_audio_system_endStream(qgf_stream* sound) {
	if (sound->sound) sound->sound->stop();
	sound->sound = NULL;
}

void 
qgf_tools_audio_system_playStream(qgf_stream* sound) {
#ifdef _WIN32
	if (sound->sound) {
		sound->sound->setVolume(sound->volume / 100.0f);
		sound->sound->setPan(sound->pan / 100.0f);
		sound->sound->setPitchShift(sound->pitch / 100.0f);
		sound->sound->setPosition(sound->pos);
		sound->sound->play();
		
	}
#endif
}

void 
qgf_tools_audio_system_stopStream(qgf_stream* sound) {
#ifdef _WIN32
	if (sound->sound) {
		sound->sound->stop();
		sound->pos = sound->sound->getPosition();
		
	}
#endif
}

void 
qgf_tools_audio_system_updateStream(qgf_stream* sound) {
#ifdef _WIN32
	if (sound->sound) {
		sound->sound->setVolume(sound->volume / 100.0f);
		sound->sound->setPan(sound->pan / 100.0f);
		sound->sound->setPitchShift(sound->pitch / 100.0f);
		if (!sound->sound->isPlaying()) sound->sound->setPosition(sound->pos);
	}
#endif
}

void
qgf_tools_audio_system_updateInformation(qgf_stream* sound) {
#ifdef _WIN32
	if (sound->sound) {
		sound->pos = sound->sound->getPosition();
		sound->pan = sound->sound->getPan() * 100.0f;
		sound->pitch = sound->sound->getPitchShift() * 100.0f;
		sound->volume = sound->sound->getVolume() * 100.0f;
	}
#endif
}

int
qgf_tools_audio_system_isPlaying(qgf_stream* sound) {
#ifdef _WIN32
	return sound->sound->isPlaying();
#endif
}