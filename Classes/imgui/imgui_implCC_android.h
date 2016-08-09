// ImGui with cocos2d-x 3.8 GLES 2.0
#include "cocos2d.h" // opengl method

IMGUI_API bool        ImGui_ImplCC_Init();
IMGUI_API void        ImGui_ImplCC_Shutdown();
IMGUI_API void        ImGui_ImplCC_NewFrame();
IMGUI_API bool        ImGui_ImplCC_ProcessEvent();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplCC_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplCC_CreateDeviceObjects();

IMGUI_API void        ImGui_ImplCC_UpdateMouseByTouch(cocos2d::Vec2 pos, bool status);
