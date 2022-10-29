#pragma once

namespace Alice
{

class Input
{
public:
    static bool IsKeyPressed(int keycode)
    {
        return s_instance->IsKeyPressedImpl(keycode);
    }

protected:
    virtual bool IsKeyPressedImpl(int keycode) = 0;

private:
    static Input* s_instance;
};

} // namespace Alice
