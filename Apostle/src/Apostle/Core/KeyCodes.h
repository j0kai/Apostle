#pragma once

#include "APpch.h"
#include "Apostle/Core/Core.h"

enum class APOSTLE_API KeyCodes
{
	// From glfw3.h
	AP_KEY_SPACE = 32,
	AP_KEY_APOSTROPHE = 39,  /* ' */
	AP_KEY_COMMA = 44,  /* , */
	AP_KEY_MINUS = 45,  /* - */
	AP_KEY_PERIOD = 46,  /* . */
	AP_KEY_SLASH = 47,  /* / */
	AP_KEY_0 = 48,
	AP_KEY_1 = 49,
	AP_KEY_2 = 50,
	AP_KEY_3 = 51,
	AP_KEY_4 = 52,
	AP_KEY_5 = 53,
	AP_KEY_6 = 54,
	AP_KEY_7 = 55,
	AP_KEY_8 = 56,
	AP_KEY_9 = 57,
	AP_KEY_SEMICOLON = 59,  /* ; */
	AP_KEY_EQUALS = 61,  /* = */
	AP_KEY_A = 65,
	AP_KEY_B = 66,
	AP_KEY_C = 67,
	AP_KEY_D = 68,
	AP_KEY_E = 69,
	AP_KEY_F = 70,
	AP_KEY_G = 71,
	AP_KEY_H = 72,
	AP_KEY_I = 73,
	AP_KEY_J = 74,
	AP_KEY_K = 75,
	AP_KEY_L = 76,
	AP_KEY_M = 77,
	AP_KEY_N = 78,
	AP_KEY_O = 79,
	AP_KEY_P = 80,
	AP_KEY_Q = 81,
	AP_KEY_R = 82,
	AP_KEY_S = 83,
	AP_KEY_T = 84,
	AP_KEY_U = 85,
	AP_KEY_V = 86,
	AP_KEY_W = 87,
	AP_KEY_X = 88,
	AP_KEY_Y = 89,
	AP_KEY_Z = 90,
	AP_KEY_LEFT_BRACKET = 91,  /* [ */
	AP_KEY_HASH = 92,  /* # - Backslash on US keyboards */
	AP_KEY_RIGHT_BRACKET = 93,  /* ] */
	AP_KEY_GRAVE_ACCENT = 96,  /* ` */

	AP_KEY_WORLD_1 = 161, /* non-US #1 */
	AP_KEY_BACKSLASH = 162, /* \ */

	/* Function keys */
	AP_KEY_ESCAPE = 256,
	AP_KEY_ENTER = 257,
	AP_KEY_TAB = 258,
	AP_KEY_BACKSPACE = 259,
	AP_KEY_INSERT = 260,
	AP_KEY_DELETE = 261,
	AP_KEY_RIGHT = 262,
	AP_KEY_LEFT = 263,
	AP_KEY_DOWN = 264,
	AP_KEY_UP = 265,
	AP_KEY_PAGE_UP = 266,
	AP_KEY_PAGE_DOWN = 267,
	AP_KEY_HOME = 268,
	AP_KEY_END = 269,
	AP_KEY_CAPS_LOCK = 280,
	AP_KEY_SCROLL_LOCK = 281,
	AP_KEY_NUM_LOCK = 282,
	AP_KEY_PRINT_SCREEN = 283,
	AP_KEY_PAUSE = 284,
	AP_KEY_F1 = 290,
	AP_KEY_F2 = 291,
	AP_KEY_F3 = 292,
	AP_KEY_F4 = 293,
	AP_KEY_F5 = 294,
	AP_KEY_F6 = 295,
	AP_KEY_F7 = 296,
	AP_KEY_F8 = 297,
	AP_KEY_F9 = 298,
	AP_KEY_F10 = 299,
	AP_KEY_F11 = 300,
	AP_KEY_F12 = 301,
	AP_KEY_F13 = 302,
	AP_KEY_F14 = 303,
	AP_KEY_F15 = 304,
	AP_KEY_F16 = 305,
	AP_KEY_F17 = 306,
	AP_KEY_F18 = 307,
	AP_KEY_F19 = 308,
	AP_KEY_F20 = 309,
	AP_KEY_F21 = 310,
	AP_KEY_F22 = 311,
	AP_KEY_F23 = 312,
	AP_KEY_F24 = 313,
	AP_KEY_F25 = 314,
	AP_KEY_KP_0 = 320,
	AP_KEY_KP_1 = 321,
	AP_KEY_KP_2 = 322,
	AP_KEY_KP_3 = 323,
	AP_KEY_KP_4 = 324,
	AP_KEY_KP_5 = 325,
	AP_KEY_KP_6 = 326,
	AP_KEY_KP_7 = 327,
	AP_KEY_KP_8 = 328,
	AP_KEY_KP_9 = 329,
	AP_KEY_KP_DECIMAL = 330,
	AP_KEY_KP_DIVIDE = 331,
	AP_KEY_KP_MULTIPLY = 332,
	AP_KEY_KP_SUBTRACT = 333,
	AP_KEY_KP_ADD = 334,
	AP_KEY_KP_ENTER = 335,
	AP_KEY_KP_EQUAL = 336,
	AP_KEY_LEFT_SHIFT = 340,
	AP_KEY_LEFT_CONTROL = 341,
	AP_KEY_LEFT_ALT = 342,
	AP_KEY_LEFT_SUPER = 343, // Windows, CMD, Super (dependent on platform)
	AP_KEY_RIGHT_SHIFT = 344,
	AP_KEY_RIGHT_CONTROL = 345,
	AP_KEY_RIGHT_ALT = 346,
	AP_KEY_RIGHT_SUPER = 347,
	AP_KEY_MENU = 348,
	AP_KEY_LAST = AP_KEY_MENU

};

// In future, find another way to fix linker error other than declaring as static
static std::map<KeyCodes, std::string> KeyCodesMap = {

	{ KeyCodes::AP_KEY_SPACE, "SPACE" },
	{ KeyCodes::AP_KEY_APOSTROPHE, "APOSTROPHE (')" },
	{ KeyCodes::AP_KEY_COMMA, "COMMA (,)" },
	{ KeyCodes::AP_KEY_MINUS, "MINUS (-)" },
	{ KeyCodes::AP_KEY_PERIOD, "PERIOD (.)" },
	{ KeyCodes::AP_KEY_SLASH, "SLASH" },
	{ KeyCodes::AP_KEY_0, "0" },
	{ KeyCodes::AP_KEY_1, "1" },
	{ KeyCodes::AP_KEY_2, "2" },
	{ KeyCodes::AP_KEY_3, "3" },
	{ KeyCodes::AP_KEY_4, "4" },
	{ KeyCodes::AP_KEY_5, "5" },
	{ KeyCodes::AP_KEY_6, "6" },
	{ KeyCodes::AP_KEY_7, "7" },
	{ KeyCodes::AP_KEY_8, "8" },
	{ KeyCodes::AP_KEY_9, "9" },
	{ KeyCodes::AP_KEY_SEMICOLON, "SEMI-COLON (;)" },
	{ KeyCodes::AP_KEY_EQUALS, "EQUALS (=)" },
	{ KeyCodes::AP_KEY_A, "A" },
	{ KeyCodes::AP_KEY_B, "B" },
	{ KeyCodes::AP_KEY_C, "C" },
	{ KeyCodes::AP_KEY_D, "D" },
	{ KeyCodes::AP_KEY_E, "E" },
	{ KeyCodes::AP_KEY_F, "F" },
	{ KeyCodes::AP_KEY_G, "G" },
	{ KeyCodes::AP_KEY_H, "H" },
	{ KeyCodes::AP_KEY_I, "I" },
	{ KeyCodes::AP_KEY_J, "J" },
	{ KeyCodes::AP_KEY_K, "K" },
	{ KeyCodes::AP_KEY_L, "L" },
	{ KeyCodes::AP_KEY_M, "M" },
	{ KeyCodes::AP_KEY_N, "N" },
	{ KeyCodes::AP_KEY_O, "O" },
	{ KeyCodes::AP_KEY_P, "P" },
	{ KeyCodes::AP_KEY_Q, "Q" },
	{ KeyCodes::AP_KEY_R, "R" },
	{ KeyCodes::AP_KEY_S, "S" },
	{ KeyCodes::AP_KEY_T, "T" },
	{ KeyCodes::AP_KEY_U, "U" },
	{ KeyCodes::AP_KEY_V, "V" },
	{ KeyCodes::AP_KEY_W, "W" },
	{ KeyCodes::AP_KEY_X, "X" },
	{ KeyCodes::AP_KEY_Y, "Y" },
	{ KeyCodes::AP_KEY_Z, "Z" },
	{ KeyCodes::AP_KEY_LEFT_BRACKET, "LEFT BRACKET ([)" },
	{ KeyCodes::AP_KEY_HASH, "HASH (#)" },
	{ KeyCodes::AP_KEY_RIGHT_BRACKET, "RIGHT BRACKET (])" },
	{ KeyCodes::AP_KEY_GRAVE_ACCENT,"`"},
	{ KeyCodes::AP_KEY_WORLD_1, "WORLD ONE" },
	{ KeyCodes::AP_KEY_BACKSLASH , "BACKSLASH (\\)" },
	{ KeyCodes::AP_KEY_ESCAPE, "ESCAPE" },
	{ KeyCodes::AP_KEY_ENTER, "ENTER" },
	{ KeyCodes::AP_KEY_TAB, "TAB" },
	{ KeyCodes::AP_KEY_BACKSPACE, "BACKSPACE" },
	{ KeyCodes::AP_KEY_INSERT, "INSERT" },
	{ KeyCodes::AP_KEY_DELETE, "DEL" },
	{ KeyCodes::AP_KEY_RIGHT, "ARROW RIGHT" },
	{ KeyCodes::AP_KEY_LEFT, "ARROW LEFT" },
	{ KeyCodes::AP_KEY_DOWN, "ARROW DOWN" },
	{ KeyCodes::AP_KEY_UP, "ARROW UP" },
	{ KeyCodes::AP_KEY_PAGE_UP, "PAGE UP" },
	{ KeyCodes::AP_KEY_PAGE_DOWN, "PAGE DOWN" },
	{ KeyCodes::AP_KEY_HOME, "HOME" },
	{ KeyCodes::AP_KEY_END, "END" },
	{ KeyCodes::AP_KEY_CAPS_LOCK, "CAPS LOCK" },
	{ KeyCodes::AP_KEY_SCROLL_LOCK, "SCROLL LOCK" },
	{ KeyCodes::AP_KEY_NUM_LOCK, "NUM LOCK" },
	{ KeyCodes::AP_KEY_PRINT_SCREEN, "PRINT_SCREEN" },
	{ KeyCodes::AP_KEY_PAUSE, "PAUSE" },
	{ KeyCodes::AP_KEY_F1, "F1" },
	{ KeyCodes::AP_KEY_F2, "F2" },
	{ KeyCodes::AP_KEY_F3, "F3" },
	{ KeyCodes::AP_KEY_F4, "F4" },
	{ KeyCodes::AP_KEY_F5, "F5" },
	{ KeyCodes::AP_KEY_F6, "F6" },
	{ KeyCodes::AP_KEY_F7, "F7" },
	{ KeyCodes::AP_KEY_F8, "F8" },
	{ KeyCodes::AP_KEY_F9, "F9" },
	{ KeyCodes::AP_KEY_F10, "F10" },
	{ KeyCodes::AP_KEY_F11, "F11" },
	{ KeyCodes::AP_KEY_F12, "F12" },
	{ KeyCodes::AP_KEY_F13, "F13" },
	{ KeyCodes::AP_KEY_F14, "F14" },
	{ KeyCodes::AP_KEY_F15, "F15" },
	{ KeyCodes::AP_KEY_F16, "F16" },
	{ KeyCodes::AP_KEY_F17, "F17" },
	{ KeyCodes::AP_KEY_F18, "F18" },
	{ KeyCodes::AP_KEY_F19, "F19" },
	{ KeyCodes::AP_KEY_F20, "F20" },
	{ KeyCodes::AP_KEY_F21, "F21" },
	{ KeyCodes::AP_KEY_F22, "F22" },
	{ KeyCodes::AP_KEY_F23, "F23" },
	{ KeyCodes::AP_KEY_F24, "F24" },
	{ KeyCodes::AP_KEY_F25, "F25" },
	{ KeyCodes::AP_KEY_KP_0, "KP 0" },
	{ KeyCodes::AP_KEY_KP_1, "KP 1" },
	{ KeyCodes::AP_KEY_KP_2, "KP 2" },
	{ KeyCodes::AP_KEY_KP_3, "KP 3" },
	{ KeyCodes::AP_KEY_KP_4, "KP 4" },
	{ KeyCodes::AP_KEY_KP_5, "KP 5" },
	{ KeyCodes::AP_KEY_KP_6, "KP 6" },
	{ KeyCodes::AP_KEY_KP_7, "KP 7" },
	{ KeyCodes::AP_KEY_KP_8, "KP 8" },
	{ KeyCodes::AP_KEY_KP_9, "KP 9" },
	{ KeyCodes::AP_KEY_KP_DECIMAL , "KP DECIMAL" },
	{ KeyCodes::AP_KEY_KP_DIVIDE, "KP DIVIDE" },
	{ KeyCodes::AP_KEY_KP_MULTIPLY, "KP MULTIPLY" },
	{ KeyCodes::AP_KEY_KP_SUBTRACT, "KP SUBTRACT" },
	{ KeyCodes::AP_KEY_KP_ADD, "KP ADD" },
	{ KeyCodes::AP_KEY_KP_ENTER, "KP ENTER" },
	{ KeyCodes::AP_KEY_KP_EQUAL, "KP EQUAL" },
	{ KeyCodes::AP_KEY_LEFT_SHIFT, "LEFT SHIFT" },
	{ KeyCodes::AP_KEY_LEFT_CONTROL , "LEFT CTRL" },
	{ KeyCodes::AP_KEY_LEFT_ALT, "LEFT ALT" },
	{ KeyCodes::AP_KEY_LEFT_SUPER, "LEFT SUPER" },
	{ KeyCodes::AP_KEY_RIGHT_SHIFT, "RIGHT SHIFT" },
	{ KeyCodes::AP_KEY_RIGHT_CONTROL, "RIGHT CTRL" },
	{ KeyCodes::AP_KEY_RIGHT_ALT, "RIGHT ALT" },
	{ KeyCodes::AP_KEY_RIGHT_SUPER, "RIGHT SUPER" },
	{ KeyCodes::AP_KEY_MENU, "MENU" }
};