#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_extend.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

#include <ctype.h>      // toupper, isprint
#include <math.h>       // sqrtf, fabsf, fmodf, powf, cosf, sinf, floorf, ceilf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi
#include <stdio.h>      // vsnprintf, sscanf, printf
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4127) // condition expression is constant
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif

void ImGui::Image(const char* spriteFrameName, const ImVec2& size, const ImVec4& tint_col, const ImVec4& border_col)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	if (cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName) == NULL) return;

	GLuint textID = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName)->getTexture()->getName();
	cocos2d::Rect rectange = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName)->getRect();
	cocos2d::Size orgSize = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName)->getTexture()->getContentSize();

	ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	if (border_col.w > 0.0f)
		bb.Max += ImVec2(2, 2);
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return;

	float focus_x = rectange.origin.x;
	float focus_y = rectange.origin.y;
	ImVec2 uv0 = ImVec2((focus_x) / orgSize.width, (focus_y) / orgSize.height);
	ImVec2 uv1 = ImVec2((focus_x + rectange.size.width) / orgSize.width, (focus_y + rectange.size.height) / orgSize.height);

	if (border_col.w > 0.0f)
	{
		window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
		window->DrawList->AddImage((ImTextureID)textID, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), uv0, uv1, GetColorU32(tint_col));
	}
	else
	{
		window->DrawList->AddImage((ImTextureID)textID, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col));
	}
}

bool ImGui::ImageButton(const ImVec2& size, const char* spriteFrameName, bool selected, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	GLuint textID = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName)->getTexture()->getName();
	cocos2d::Rect rectange = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName)->getRect();
	cocos2d::Size orgSize = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName)->getTexture()->getContentSize();
	// Default to using texture ID as ID. User can still push string/integer prefixes.
	// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
	ImGui::PushID((void *)textID);
	const ImGuiID id = window->GetID("#image");
	ImGui::PopID();

	const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
	const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
	ItemSize(bb);
	if (!ItemAdd(bb, &id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	// Render
	ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	if (selected) col = GetColorU32(ImGuiCol_ButtonActive);
	RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	if (bg_col.w > 0.0f)
		window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

	float focus_x = rectange.origin.x;
	float focus_y = rectange.origin.y;
	ImVec2 uv0 = ImVec2((focus_x) / orgSize.width, (focus_y) / orgSize.height);
	ImVec2 uv1 = ImVec2((focus_x + rectange.size.width) / orgSize.width, (focus_y + rectange.size.height) / orgSize.height);

	window->DrawList->AddImage((ImTextureID)textID, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

	return pressed;
}

bool ImGui::Image(cocos2d::Texture2D* texture, const cocos2d::Rect cropSize, const ImVec2& displaySize, int borderSize)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	const ImVec4& tint_col = ImVec4(1, 1, 1, 1);
	const ImVec4& border_col = ImVec4(0.95f, 0.12f, 0.04f, 1);
	
	GLuint textureID = texture->getName();
	ImRect bb(window->DC.CursorPos, window->DC.CursorPos + displaySize);
	if (border_col.w > 0.0f)
		bb.Max += ImVec2(2, 2);
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return false;

	ImGui::PushID((void *)textureID);
	const ImGuiID id = window->GetID("#image");
	ImGui::PopID();

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	float focus_x = cropSize.origin.x;
	float focus_y = cropSize.origin.y;
	ImVec2 uv0 = ImVec2((focus_x) / texture->getContentSize().width, (focus_y) / texture->getContentSize().height);
	ImVec2 uv1 = ImVec2((focus_x + cropSize.size.width) / texture->getContentSize().width, (focus_y + cropSize.size.height) / texture->getContentSize().height);

	if (border_col.w > 0.0f)
	{
		window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(border_col));
		window->DrawList->AddImage((ImTextureID)textureID, bb.Min + ImVec2(borderSize, borderSize), bb.Max - ImVec2(borderSize, borderSize), uv0, uv1, GetColorU32(tint_col));
	}
	else
	{
		window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(border_col));
		window->DrawList->AddImage((ImTextureID)textureID, bb.Min + ImVec2(borderSize, borderSize), bb.Max - ImVec2(borderSize, borderSize), uv0, uv1, GetColorU32(tint_col));
	}

	return pressed;
}

bool ImGui::Image(cocos2d::Sprite* sprite, const cocos2d::Rect cropSize, const ImVec2& displaySize, int borderSize)
{
	return ImGui::Image(sprite->getTexture(), cropSize, displaySize, borderSize);
}

bool ImGui::InputTextM(const char* label, std::string& text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)	
	bool ret = InputTextEx(label, &text[0], text.capacity(), ImVec2(0, 0), 0, 0, nullptr);
	return ret;
#else
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	//============================================
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size = CalcItemSize(ImVec2(0, 0), CalcItemWidth(), label_size.y + style.FramePadding.y*2.0f);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));
	//============================================
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, &id))
		return false;

	const bool hovered = IsHovered(frame_bb, id);
	if (hovered)
	{
		SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}
	ImGuiTextEditState& edit_state = g.InputTextState;
	const bool user_clicked = hovered && io.MouseClicked[0];
	if (user_clicked)
	{
		if (g.ActiveId != id)
		{
			edit_state.CursorAnimReset();
		}
		SetActiveID(id, window);
		FocusWindow(window);
	}
	else if (io.MouseClicked[0])
	{
		// Release focus when we click outside
		if (g.ActiveId == id)
			SetActiveID(0, NULL);
	}

	if (g.ActiveId == id)
	{
		//window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_TextSelectedBg), style.FrameRounding);
		RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_TextSelectedBg), true, style.FrameRounding);
		ImRect frame_select(frame_bb);
		frame_select.Reduce(ImVec2(2, 2));
		RenderFrame(frame_select.Min, frame_select.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	}else
	{
		RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	}

	//============================================
	int w = snprintf(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%s", text.c_str());
	int tempSize = IM_ARRAYSIZE(g.TempBuffer);
	g.TempBuffer[tempSize - 1] = 0;
	const char* text_end = g.TempBuffer + ((w == -1) ? tempSize : w);

	// Render Text
	const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), g.TempBuffer, text_end, 0.0f, &clip_rect);

	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	return user_clicked;
#endif
}

bool ImGui::InputTextM2(const char* label, std::string& text)
{
	

	return true;
}