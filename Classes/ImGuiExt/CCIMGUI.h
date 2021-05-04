/**
 * https://github.com/namkazt/cocos2d-x-ImGui
 */
#pragma once

#include "cocos2d.h"
#include "imgui/imgui.h"

#define IMGUI_LUA 1

USING_NS_CC;

class CCImValue // as cocos2d::Value
{
public:
    CCImValue();
    explicit CCImValue(bool v):_type(Type::BOOLEAN) { setValue(v); };
    explicit CCImValue(float v):_type(Type::FLOAT) { setValue(v); };
    explicit CCImValue(int v):_type(Type::INTEGER) { setValue(v); };

    // bool
    void setValue(bool v) { _field.boolVal = v; };
    bool asBool() { return _field.boolVal; };

    // float
    void setValue(float v) { _field.floatVal = v; };
    float asFloat() { return _field.floatVal; };

    // int
    void setValue(int v) { _field.intVal = v; };
    int asInt() { return _field.intVal; };

    /** Value type wrapped by Value. */
    enum class Type
    {
        /// no value is wrapped, an empty Value
        NONE = 0,
        /// wrap byte
        BYTE,
        /// wrap integer
        INTEGER,
        /// wrap float
        FLOAT,
        /// wrap double
        DOUBLE,
        /// wrap bool
        BOOLEAN,
        /// wrap string
        STRING,
        /// wrap vector
        VECTOR,
        /// wrap ValueMap
        MAP,
        /// wrap ValueMapIntKey
        INT_KEY_MAP
    };
protected:
    union
    {
        unsigned char byteVal;
        int intVal;
        float floatVal;
        double doubleVal;
        bool boolVal;

        std::string* strVal;
        ValueVector* vectorVal;
        ValueMap* mapVal;
        ValueMapIntKey* intKeyMapVal;
    }_field;

    Type _type;
};

class CCIMGUI
{
public:
	static CCIMGUI* getInstance();
	//-------------------------------------------------------
	GLFWwindow* getWindow() { return _window; };
	void setWindow(GLFWwindow* window) { _window = window; };
	ImVec4 getClearColor() { return _clearColor; };
	void setClearColor(ImColor color) { _clearColor = color; };
	//-------------------------------------------------------
	void init();
	void updateImGUI();
	void addImGUI(std::function<void()> imGUICall, const std::string& name) { _callPiplines[name] = imGUICall; };
	void removeImGUI(const std::string& name);
	//-------------------------------------------------------
	void setValue(bool value, const std::string& uid);
	void setValue(int value, const std::string& uid);
	CCImValue* getValue(const std::string& uid);
	void removeValue(const std::string& uid);
	//-------------------------------------------------------
	void setShowStyleEditor(bool show) { isShowSetupStyle = show; };

    // imgui helper
    void image(const std::string& fn, int w = -1, int h = -1);
    bool imageButton(const std::string& fn, int w = -1, int h = -1);

    ImFont* chineseFont = nullptr;
private:
    bool isShowSetupStyle = false;
    void displaySetupStyle();

private:
    GLFWwindow* _window = nullptr;
    ImVec4 _clearColor = ImColor(114, 144, 154);

    std::unordered_map<std::string, std::function<void()>> _callPiplines;
    std::unordered_map<std::string, CCImValue*> _values;

    std::unordered_map<unsigned int, int> _usedTextureIdMap;
};
