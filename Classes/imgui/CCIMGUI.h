#ifndef __CC_IMGUI_H__
#define __CC_IMGUI_H__

#include "cocos2d.h"
#include "imgui.h"

USING_NS_CC;

class CCIMGUI_Value
{
public:
	void* value = nullptr;
	bool getBool() { return (bool)value; };
	int getInt() { return (int)value; };
};

class CCIMGUI
{
private:
	//-------------------------------------------------------
	std::map<std::string, std::function<void()>> _callPiplines;
	std::map<std::string, CCIMGUI_Value*> _values;
	std::vector<std::string> _markToDelete;
	//-------------------------------------------------------
	void displaySetupStyle();
public:
	static CCIMGUI* getInstance();
	//-------------------------------------------------------
	void init();
	void updateImGUI();
	void addImGUI(std::function<void()> imGUICall, std::string name) {_callPiplines[name] = imGUICall;};
	void removeImGUI(std::string name);
	//-------------------------------------------------------
	void setValue(bool value, std::string uid);
	void setValue(int value, std::string uid);
	CCIMGUI_Value* getValue(std::string uid);
	void removeValue(std::string uid);
};

#endif // __IMGUILAYER_H__
