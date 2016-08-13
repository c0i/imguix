#include <imgui.h>
#include "cocos2d.h"

namespace ImGui
{
	//==============================================================
	// Image relat
	IMGUI_API void          Image(const char* spriteFrameName, const ImVec2& size, const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	IMGUI_API bool          ImageButton(const ImVec2& size, const char* spriteFrameName, bool selected = false, int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	IMGUI_API bool          Image(cocos2d::Texture2D* texture,const cocos2d::Rect cropSize, const ImVec2& displaySize, int borderSize = 1);
	IMGUI_API bool          Image(cocos2d::Sprite* sprite,const cocos2d::Rect cropSize, const ImVec2& displaySize, int borderSize = 1);
	//===============================================================
	// TextInput mobile
	IMGUI_API bool			InputTextM(const char* label, std::string& text);
	IMGUI_API bool			InputTextM2(const char* label, std::string& text);
}