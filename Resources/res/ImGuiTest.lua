print('hellllo from lua')

local function createDefaultWindow()
    ImGui.Separator()
    ImGui.Text('Hello from Lua')
end

local f = 4.9
local color = {100/255, 200/255, 300/255}
local function createToolWindow()
    ImGui.Begin('Lua Window')
    if ImGui.Button('Lua Button') then
        print('click Lua Button', os.time())
        cxxfun()
    end
    ImGui.End()
end

local _drawList = {}
function ImGuiDraw(func)
    _drawList[func] = func
end

function ImGuiRenderer()

    for k,v in pairs(_drawList) do
        if v then v() end
    end
end


ImGuiDraw(createDefaultWindow)
ImGuiDraw(createToolWindow)
ImGuiDraw(function ( )
    ImGui.Begin('Lua Window 222')
    ImGui.Text("Hello, world!");
    f = ImGui.SliderFloat("float", f, 0.0, 1.0)
    color = ImGui.ColorEdit3("clear color", color);
    if (ImGui.Button("Test Window")) then
        print('click Test Window')
    end

    if (ImGui.Button("Another Window")) then
        print('click another window')
    end

    ImGui.End()

end)
