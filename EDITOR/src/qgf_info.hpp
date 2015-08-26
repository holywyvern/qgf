#define QGF_VERSION 1
#define QGF_VERSION_STRING "1.0"

#ifdef _WIN32
#define QGF_PLUGIN_EXT_A "dll"
#define QGF_PLUGIN_EXT_B ".dll"
#else
#define QGF_PLUGIN_EXT_A "so"
#define QGF_PLUGIN_EXT_B ".so"
#endif

#define QGF_SCRIPT_EXT_A ".rb"
#define QGF_SCRIPT_EXT_B ".rbw"
#define QGF_SCRIPT_EXT_C "rb"
#define QGF_SCRIPT_EXT_D "rbw"

#define QGF_PROJECT_CONFIG_NAME "project.qgfproj"
#define QGF_CONFIG_ROOT_TAG "qgf"
#define QGF_CONFIG_INFO_TAG "info"
#define QGF_CONFIG_INFO_NAME_TAG "name"
#define QGF_CONFIG_INFO_VERSION_TAG "qgf_version"
#define QGF_CONFIG_PLUGINS_TAG "plugins"
#define QGF_CONFIG_PLUGINS_PLUGIN_TAG "plugin"
#define QGF_CONFIG_FOLDERS_TAG "folders"
#define QGF_CONFIG_FOLDERS_FOLDER_TAG "folder"
#define QGF_CONFIG_FOLDERS_FOLDER_ATT_TYPE_TAG "type"
#define QGF_CONFIG_FOLDERS_FOLDER_ATT_NAME_TAG "name"
#define QGF_CONFIG_FOLDERS_FOLDER_AUDIO_TAG "audio"
#define QGF_CONFIG_FOLDERS_FOLDER_IMAGE_TAG "image"
#define QGF_CONFIG_FOLDERS_FOLDER_FONT_TAG "font"
#define QGF_CONFIG_FOLDERS_FOLDER_SCRIPT_TAG "script"
#define QGF_CONFIG_FOLDERS_FOLDER_MUSIC_TAG "music"
#define QGF_CONFIG_FOLDERS_FOLDER_SFX_TAG "sfx"
#define QGF_CONFIG_FOLDERS_FOLDER_VIDEO_TAG "video"

#define QGF_AUDIO_FORMATS "*.mp3;*.ogg;*.mid"
#define QGF_IMAGE_FORMATS "*.bmp;*.jpg;*.jpeg;*.png"
#define QGF_FONT_FORMATS "*.ttf"
#define QGF_VIDEO_FORMATS "*.ogv"
#define QGF_SCRIPT_FORMATS "*.rb;*.rbw"

#define QGF_INVALID_CHAR_SIZE 20
#define QGF_INVALID_FOLDER_CHARS { wxT("."), wxT("&"), wxT("\\"), wxT("/"), wxT("+"), wxT("{"), wxT("}"), wxT("["), wxT("]"), wxT("?"), wxT(","), wxT(";"), wxT("<"), wxT(">"), wxT("*"), wxT(":"), wxT("\""), wxT("|"), wxT("="), wxT("'") }