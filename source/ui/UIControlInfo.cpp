#include "UIControlInfo.hpp"

std::list<UIElement*> AddControlInfo(State *state){
    std::list<UIElement*> result;

    SolidColor *RobotBodyElement = new SolidColor(state, 50, 50, 100, 200, 0, 0, 0, 0.8);
    RobotBodyElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColor *me = (SolidColor *)meOrg;
        if(state->controlMode == Robot){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    RobotBodyElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = Robot;
    };
    result.push_back(RobotBodyElement);

    SolidColorCircle *RobotHeadElement = new SolidColorCircle(state, 100, 300, 50, 0, 0, 0, 0.8);
    RobotHeadElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColorCircle *me = (SolidColorCircle *)meOrg;
        if(state->controlMode == RobotHead){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    RobotHeadElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = RobotHead;
    };
    result.push_back(RobotHeadElement);

    SolidColor *RobotUpperArmElement = new SolidColor(state, 150, 150, 100, 50, 0, 0, 0, 0.8);
    RobotUpperArmElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColor *me = (SolidColor *)meOrg;
        if(state->controlMode == UpperArm){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    RobotUpperArmElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = UpperArm;
    };
    result.push_back(RobotUpperArmElement);

    SolidColor *RobotLowerArmElement = new SolidColor(state, 250, 150, 50, 100, 0, 0, 0, 0.8);
    RobotLowerArmElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColor *me = (SolidColor *)meOrg;
        if(state->controlMode == LowerArm){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    RobotLowerArmElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = LowerArm;
    };
    result.push_back(RobotLowerArmElement);

    SolidColorCircle *RobotHandElement = new SolidColorCircle(state, 275, 300, 50, 0, 0, 0, 0.8);
    RobotHandElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColorCircle *me = (SolidColorCircle *)meOrg;
        if(state->controlMode == Hand){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    RobotHandElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = Hand;
    };
    result.push_back(RobotHandElement);

    SolidColorCircle *CameraCircleElement = new SolidColorCircle(state, 400, 125, 30, 0, 0, 0, 0.8);
    CameraCircleElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColorCircle *me = (SolidColorCircle *)meOrg;
        if(state->controlMode == Camera){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    CameraCircleElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = Camera;
    };
    result.push_back(CameraCircleElement);

    SolidColor *CameraBodyElement = new SolidColor(state, 300, 80, 200, 100, 0, 0, 0, 0.8);
    CameraBodyElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColor *me = (SolidColor *)meOrg;
        if(state->controlMode == Camera){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.5;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.5;
        }
    };
    CameraBodyElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = Camera;
    };
    result.push_back(CameraBodyElement);


    SolidColorTriangle *LampTriangleElement = new SolidColorTriangle(state, 550, 150, 150, 0, 0, 0, 0.8);
    LampTriangleElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColorTriangle *me = (SolidColorTriangle *)meOrg;
        if(state->controlMode == Lamp){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.8;
        }
    };
    LampTriangleElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = Lamp;
    };
    result.push_back(LampTriangleElement);

    SolidColor *LampBodyElement = new SolidColor(state, 600, 80, 50, 100, 0, 0, 0, 0.8);
    LampBodyElement->update = [](UIElement* meOrg, State* state) -> void {
        SolidColor *me = (SolidColor *)meOrg;
        if(state->controlMode == Lamp){
            me->color[0] = 0;
            me->color[1] = 1;
            me->color[2] = 0;
            me->color[3] = 0.5;
        }else{
            me->color[0] = 0;
            me->color[1] = 0;
            me->color[2] = 0;
            me->color[3] = 0.5;
        }
    };
    LampBodyElement->OnClick = [](State *state, double x, double y) -> void {
        state->controlMode = Lamp;
    };
    result.push_back(LampBodyElement);


    return result;
}