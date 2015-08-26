#include "qgf/types.h"
#include "qgf/tools/audio_system.h"

ruby_value 
rb_gc_cAudioFile_alloc(ruby_value klass);
void
rb_gc_cAudioFile_mark(TAudioFile* stream);
void
rb_gc_cAudioFile_free(TAudioFile* stream);

ruby_value 
rb_cAudioFile_initialize(ruby_value self, ruby_value filename, ruby_value loop);

ruby_value 
rb_cAudioFile_playingQ(ruby_value self);

ruby_value 
rb_cAudioFile_play(ruby_value self);
ruby_value 
rb_cAudioFile_stop(ruby_value self);
ruby_value 
rb_cAudioFile_pause(ruby_value self);

ruby_value 
rb_cAudioFile_SETvolume(ruby_value self, ruby_value value);
ruby_value 
rb_cAudioFile_GETvolume(ruby_value self);
ruby_value 
rb_cAudioFile_SETpitch(ruby_value self, ruby_value value);
ruby_value 
rb_cAudioFile_GETpitch(ruby_value self);
ruby_value 
rb_cAudioFile_SETpos(ruby_value self, ruby_value value);
ruby_value 
rb_cAudioFile_GETpos(ruby_value self);
ruby_value 
rb_cAudioFile_SETpan(ruby_value self, ruby_value value);
ruby_value 
rb_cAudioFile_GETpan(ruby_value self);

ruby_value 
rb_cAudioFileCLASS_last_music(ruby_value self);

static ruby_value rb_cAudioFile;

void
Init_QGF_Audio_AudioFile();