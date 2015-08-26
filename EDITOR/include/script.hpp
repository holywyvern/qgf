#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include <string>

class Script {
private:
	std::string name;
	std::string folder;
	std::string text;
protected:
public:
	void SetName(const char* name) { name = std::string(name); }
	void SetFolder(const char* folder) { folder = std::string(folder); }
	void SetText(const char* text) { text = std::string(text); }
	const char* GetName() { return name.c_str(); }
	const char* GetFolder() { return folder.c_str(); }
	const char* GetText() { return text.c_str(); }
	
}

#endif /* __SCRIPT_H__ */