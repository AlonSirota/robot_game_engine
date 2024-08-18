#include "UIDebugInfo.hpp"

std::list<UIElement *> AddDebugInfo(State *state)
{
    std::list<UIElement *> result;

    TextBox *controlModeLable = new TextBox(state, 10, 1050, "", 1, 1, 1, 0, 0, 0);
    controlModeLable->updateString = [](State *state) -> string
    {if(!state->displayDebugInfo){return "";} return "Control Mode = " + ControlModeToString(state->controlMode); };
    result.push_back(controlModeLable);

    TextLable *MovingForwardLable = new TextLable(state, 10, 1020, "", 1, 1, 1);
    MovingForwardLable->updateString = [](State *state) -> string
    {
        if (!state->displayDebugInfo)
        {
            return "";
        }
        return "isMovingForward = " +
               string((state->controlCommands.isMovingForward) ? "True" : "False");
    };
    result.push_back(MovingForwardLable);

    TextLable *RotatingLeftLable = new TextLable(state, 10, 990, "", 1, 1, 1);
    RotatingLeftLable->updateString = [](State *state) -> string
    {
        if (!state->displayDebugInfo)
        {
            return "";
        }
        return "isRotatingLeft = " +
               string((state->controlCommands.isRotatingLeft) ? "True" : "False");
    };
    result.push_back(RotatingLeftLable);

    TextLable *RotatingRightLable = new TextLable(state, 10, 960, "", 1, 1, 1);
    RotatingRightLable->updateString = [](State *state) -> string
    {
        if (!state->displayDebugInfo)
        {
            return "";
        }
        return "isRotatingRight = " +
               string((state->controlCommands.isRotatingRight) ? "True" : "False");
    };
    result.push_back(RotatingRightLable);

    TextLable *RobotPositionLable = new TextLable(state, 10, 930, "", 1, 1, 1);
    RobotPositionLable->updateString = [](State *state) -> string
    {
        if (!state->displayDebugInfo)
        {
            return "";
        }
        return "RobotPosition = x=" +
               std::to_string(state->robot.transform.position.x) +
               " y=" + std::to_string(state->robot.transform.position.y) +
               " z=" + std::to_string(state->robot.transform.position.z);
    };
    result.push_back(RobotPositionLable);

    TextLable *CameraPositionLable = new TextLable(state, 10, 900, "", 1, 1, 1);
    CameraPositionLable->updateString = [](State *state) -> string
    {
        if (!state->displayDebugInfo)
        {
            return "";
        }
        return "CameraPosition = x=" + std::to_string(state->camera.position.x) +
               " y=" + std::to_string(state->camera.position.y) +
               " z=" + std::to_string(state->camera.position.z);
    };
    result.push_back(CameraPositionLable);
    return result;
}

// Robot, RobotHead, UpperArm, LowerArm, Hand, Camera
string ControlModeToString(ControlMode mode)
{
    switch (mode)
    {
    case Robot:
        return "Robot";
    case RobotHead:
        return "RobotHead";
    case UpperArm:
        return "UpperArm";
    case LowerArm:
        return "LowerArm";
    case Hand:
        return "Hand";
    case Camera:
        return "Camera";
    default:
        exit(1);
    }
    return "";
}