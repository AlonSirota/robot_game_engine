#pragma once
#include "UICommon.hpp"

#include "UIElement.hpp"
#include "TextLable.hpp"
#include "TextBox.hpp"
#include "SolidColor.hpp"

std::list<UIElement*> AddDebugInfo(State *);

string ControlModeToString(ControlMode);
string PointOfViewToString(PointOfView);