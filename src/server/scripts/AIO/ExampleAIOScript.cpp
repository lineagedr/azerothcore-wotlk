#include "ScriptMgr.h"
#include "World.h"

class ExampleAIOScript : public AIOScript
{
public:
    ExampleAIOScript()
        : AIOScript("AIOExample")
    {
        using namespace std::placeholders;
        AddHandler("Print", std::bind(&ExampleAIOScript::HandlePrint, this, _1, _2));
    }

    void HandlePrint(Player* sender, const LuaVal& args)
    {
        const LuaVal& ButtonName  = args[4];
        const LuaVal& InputValue  = args[5];
        const LuaVal& SliderValue = args[6];

        printf("ButtonFunctionName: %s\nInputValue: %s\nSliderValue: %f\n",
            ButtonName.str().c_str(),
            InputValue.str().c_str(),
            SliderValue.num());
    }
};

void AddSC_ExampleAIO()
{
    new ExampleAIOScript;
}
