#pragma once

#include "APpch.h"
#include "Apostle/Core/Core.h"

enum class APOSTLE_API MouseButtonCodes
{
	AP_MOUSE_BUTTON_1       =  0,
	AP_MOUSE_BUTTON_2       =  1,
	AP_MOUSE_BUTTON_3       =  2,
	AP_MOUSE_BUTTON_4       =  3,
	AP_MOUSE_BUTTON_5       =  4,
	AP_MOUSE_BUTTON_6       =  5,
	AP_MOUSE_BUTTON_7       =  6,
	AP_MOUSE_BUTTON_8       =  7,
	AP_MOUSE_BUTTON_LAST    =  AP_MOUSE_BUTTON_8,
	AP_MOUSE_BUTTON_LEFT    =  AP_MOUSE_BUTTON_1,
	AP_MOUSE_BUTTON_RIGHT   =  AP_MOUSE_BUTTON_2,
	AP_MOUSE_BUTTON_MIDDLE  =  AP_MOUSE_BUTTON_3,
};

// In future, find another way to fix linker error other than declaring as static
static std::map<MouseButtonCodes, std::string> MouseButtonCodesMap = {

	{ MouseButtonCodes::AP_MOUSE_BUTTON_1, "BUTTON 1" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_2, "BUTTON 2" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_3, "BUTTON 3" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_4, "BUTTON 4" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_5, "BUTTON 5" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_6, "BUTTON 6" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_7, "BUTTON 7" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_8, "BUTTON 8" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_LAST, "LAST" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_LEFT, "LEFT CLICK" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_RIGHT, "RIGHT CLICK" },
	{ MouseButtonCodes::AP_MOUSE_BUTTON_MIDDLE, "MIDDLE MOUSE" }

};