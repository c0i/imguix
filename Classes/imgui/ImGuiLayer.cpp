#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "CCIMGUI.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#include "imgui_impl_glfw.h"
#else
#include "imgui_implCC_android.h"
#endif

USING_NS_CC;

// on "init" you need to initialize your instance
bool ImGuiLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	// init on windows
#else
	ImGui_ImplCC_Init();
#endif
	//----------------------------------------
	// init imgui cc
    //setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	//----------------------------------------
	// convert touch to mouse

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event*) -> bool {
		Vec2 pos = touch->getLocationInView();
		ImGui_ImplCC_UpdateMouseByTouch(pos, true);
    };
	listener->onTouchMoved = [](Touch* touch, Event*) -> void {
		Vec2 pos = touch->getLocationInView();
		ImGui_ImplCC_UpdateMouseByTouch(pos, true);
	};
	listener->onTouchEnded = [](Touch* touch, Event*) -> void {
		Vec2 pos = touch->getLocationInView();
		ImGui_ImplCC_UpdateMouseByTouch(pos, false);
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    
    return true;
}

void ImGuiLayer::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(ImGuiLayer::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void ImGuiLayer::onDraw()
{
    //glUseProgram(0);

	//----- draw imgui
	ImGuiIO& io = ImGui::GetIO();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    ImGui_ImplGlfw_NewFrame();
#else
	ImGui_ImplCC_NewFrame();
#endif
	CCIMGUI::getInstance()->updateImGUI();

    ImGui::Render();

	//glUseProgram(1);
}
