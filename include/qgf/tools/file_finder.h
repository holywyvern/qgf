#ifndef QGF_TOOLS_FILE_FINDER_H
#define QGF_TOOLS_FILE_FINDER_H 1


char*
qgf_tools_file_finder_find_image(char* filename);

char*
qgf_tools_file_finder_find_music(char* filename);

char*
qgf_tools_file_finder_find_sound(char* filename);

char*
qgf_tools_file_finder_find_video(char* filename);

char*
qgf_tools_file_finder_find_data(char* filename);

char*
qgf_tools_file_finder_find_font(char* filename, int bold, int italic);

#endif
