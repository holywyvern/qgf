#include "qgf/audio/audio_file.h"
#include "qgf/ruby.h"

ruby_value 
rb_gc_cAudioFile_alloc(ruby_value klass) {
	TAudioFile* file = ALLOC(TAudioFile);
	file->name = Qnil;
	file->stream = 0;
	return Data_Wrap_Struct(klass, rb_gc_cAudioFile_mark, rb_gc_cAudioFile_free, file);
}

void
rb_gc_cAudioFile_mark(TAudioFile* stream) {
	rb_gc_mark(stream->name);
}

void
rb_gc_cAudioFile_free(TAudioFile* stream) {
	if (stream->stream) qgf_tools_audio_system_endStream(stream->stream);
	free(stream->stream);
	stream->stream = 0;
}

ruby_value 
rb_cAudioFile_initialize(ruby_value self, ruby_value filename, ruby_value loop) {
	qgf_stream* s = qgf_tools_audio_system_openStream(StringValuePtr(filename), RTEST(loop));
	if (!s) {
		RAISE_FILE_NOT_FOUND(filename);
	}
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	file->stream = s;
	file->name = filename;
	if (RTEST(loop)) rb_cv_set(rb_cAudioFile, "@@lmsk", self);
	return self;
}

ruby_value 
rb_cAudioFile_playingQ(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	return qgf_tools_audio_system_isPlaying(file->stream);
}

ruby_value 
rb_cAudioFile_play(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_playStream(file->stream);
	return Qnil;
}

ruby_value 
rb_cAudioFile_stop(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_stopStream(file->stream);
	qgf_tools_audio_system_updateInformation(file->stream);
	file->stream->pos = 0;
	return Qnil;
}

ruby_value 
rb_cAudioFile_pause(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_stopStream(file->stream);
	qgf_tools_audio_system_updateInformation(file->stream);
	return Qnil;
}

ruby_value 
rb_cAudioFile_SETvolume(ruby_value self, ruby_value value) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	file->stream->volume = (float)NUM2DBL(value);
	qgf_tools_audio_system_updateStream(file->stream);
	return value;
}

ruby_value 
rb_cAudioFile_GETvolume(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	return DBL2NUM(file->stream->volume);
}

ruby_value 
rb_cAudioFile_SETpitch(ruby_value self, ruby_value value) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	file->stream->pitch = (float)NUM2DBL(value);
	qgf_tools_audio_system_updateStream(file->stream);
	return value;
}

ruby_value 
rb_cAudioFile_GETpitch(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	return DBL2NUM(file->stream->pitch);
}

ruby_value 
rb_cAudioFile_SETpos(ruby_value self, ruby_value value) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	file->stream->pos = (float)NUM2DBL(value);
	qgf_tools_audio_system_updateStream(file->stream);
	return value;
}

ruby_value 
rb_cAudioFile_GETpos(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	return DBL2NUM(file->stream->pos);
}

ruby_value 
rb_cAudioFile_SETpan(ruby_value self, ruby_value value) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	file->stream->pan = (float)NUM2DBL(value);
	qgf_tools_audio_system_updateStream(file->stream);
	return value;
}

ruby_value 
rb_cAudioFile_GETpan(ruby_value self) {
	TAudioFile* file;
	Data_Get_Struct(self, TAudioFile, file);
	qgf_tools_audio_system_updateInformation(file->stream);
	return DBL2NUM(file->stream->pan);
}


ruby_value 
rb_cAudioFileCLASS_last_music(ruby_value self) {
	return rb_cv_get(rb_cAudioFile, "@@lmsk");
}

void
Init_QGF_Audio_AudioFile() {
	qgf_tools_audio_system_open();
	rb_cAudioFile = rb_define_class("AudioFile", rb_cObject);
	rb_define_alloc_func(rb_cAudioFile, rb_gc_cAudioFile_alloc);

	//rb_cv_set(rb_cAudioFile, "@@lmsk", Qnil);

	rb_define_method(rb_cAudioFile, "initialize", RUBY_METHOD_FUNC(rb_cAudioFile_initialize), 2);

	rb_define_method(rb_cAudioFile, "playing?", RUBY_METHOD_FUNC(rb_cAudioFile_playingQ), 0);

	rb_define_method(rb_cAudioFile, "play", RUBY_METHOD_FUNC(rb_cAudioFile_play), 0);
	rb_define_method(rb_cAudioFile, "stop", RUBY_METHOD_FUNC(rb_cAudioFile_stop), 0);
	rb_define_method(rb_cAudioFile, "pause", RUBY_METHOD_FUNC(rb_cAudioFile_pause), 0);

	rb_define_method(rb_cAudioFile, "volume", RUBY_METHOD_FUNC(rb_cAudioFile_GETvolume), 0);
	rb_define_method(rb_cAudioFile, "volume=", RUBY_METHOD_FUNC(rb_cAudioFile_SETvolume), 1);
	rb_define_method(rb_cAudioFile, "pitch", RUBY_METHOD_FUNC(rb_cAudioFile_GETpitch), 0);
	rb_define_method(rb_cAudioFile, "pitch=", RUBY_METHOD_FUNC(rb_cAudioFile_SETpitch), 1);
	rb_define_method(rb_cAudioFile, "pos", RUBY_METHOD_FUNC(rb_cAudioFile_GETpos), 0);
	rb_define_method(rb_cAudioFile, "pos=", RUBY_METHOD_FUNC(rb_cAudioFile_SETpos), 1);
	rb_define_method(rb_cAudioFile, "pan", RUBY_METHOD_FUNC(rb_cAudioFile_GETpan), 0);
	rb_define_method(rb_cAudioFile, "pan=", RUBY_METHOD_FUNC(rb_cAudioFile_SETpan), 1);

	rb_define_singleton_method(rb_cAudioFile, "last_music", RUBY_METHOD_FUNC(rb_cAudioFileCLASS_last_music), 0);

}