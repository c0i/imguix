#pragma once

#include "cocos2d.h"
USING_NS_CC;

class ImGuiLayer : public cocos2d::Layer
{
public:
    ~ImGuiLayer();
    bool detached = false;

    static void createAndKeepOnTop();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;

    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

    void onDraw();

    // implement the "static create()" method manually
    CREATE_FUNC(ImGuiLayer);
private:
#if COCOS2D_VERSION < 0x00040000
    CustomCommand _command;
#else
    CallbackCommand _command;
#endif
};
