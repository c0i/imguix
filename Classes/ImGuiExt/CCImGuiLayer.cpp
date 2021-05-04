#include "CCImGuiLayer.h"
#include "imgui/imgui.h"
//#include "imgui_impl_cocos2dx.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "CCIMGUI.h"

USING_NS_CC;

static GLFWwindow* getGLFWWindow()
{
    GLViewImpl* glview = (GLViewImpl*)Director::getInstance()->getOpenGLView();
    GLFWwindow* window = glview->getWindow();
    return window;
}

ImGuiLayer::~ImGuiLayer()
{
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::createAndKeepOnTop()
{
    // delay call, once.
    auto director = Director::getInstance();
    director->getScheduler()->schedule([=](float dt)
    {
        std::string layerName = "ImGUILayer";
        auto order = INT_MAX;
        auto layer = ImGuiLayer::create();
        auto runningScene = Director::getInstance()->getRunningScene();
        if (runningScene && !runningScene->getChildByName(layerName))
        {
           runningScene->addChild(layer, INT_MAX, layerName);
        }
        
        auto e = Director::getInstance()->getEventDispatcher();
        layer->detached = false;
        e->addCustomEventListener(Director::EVENT_BEFORE_SET_NEXT_SCENE, [&, layerName](EventCustom*){
            layer = dynamic_cast<ImGuiLayer*>(Director::getInstance()->getRunningScene()->getChildByName(layerName));
            if (layer) {
                layer->retain();
                layer->removeFromParent();
                layer->detached = true;
            }
        });
        e->addCustomEventListener(Director::EVENT_AFTER_SET_NEXT_SCENE, [&, layer, layerName](EventCustom*){
            if (layer && layer->detached) {
                Director::getInstance()->getRunningScene()->addChild(layer, order, layerName);
                layer->release();
                layer->detached = false;
            }
        });
    }, director, 0, 0, 0, false, "checkIMGUI");
}

// on "init" you need to initialize your instance
bool ImGuiLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(getGLFWWindow(), true);
    ImGui_ImplOpenGL2_Init();

#if COCOS2D_VERSION < 0x00040000
    // init imgui
    setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
#endif

    auto fontPath = FileUtils::getInstance()->fullPathForFilename("font/cfk.ttf");
    io.Fonts->AddFontDefault();
    CCIMGUI::getInstance()->chineseFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 20.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon()); // io.Fonts->GetGlyphRangesChineseFull()

	// events
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event*) -> bool {
        bool inImGuiWidgets = ImGui::IsAnyWindowHovered();
        
        return inImGuiWidgets;
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void ImGuiLayer::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(ImGuiLayer::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

// main loop from https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl2/main.cpp#L79
void ImGuiLayer::onDraw()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    // Draw All custom ImGui Widgets
    CCIMGUI::getInstance()->updateImGUI();

    // Rendering
    ImGui::Render();

    auto window = getGLFWWindow();

#if COCOS2D_VERSION < 0x00040000
    // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
    // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
    GLint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    glUseProgram(0);
#endif

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

#if COCOS2D_VERSION < 0x00040000
    glUseProgram(last_program);
#endif

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
