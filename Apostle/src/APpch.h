#pragma once

#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

/*----- Data Structures -----*/
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <unordered_set>

/*----- Windows API -----*/
#ifdef AP_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

/*---- Logger (REMOVE FROM RELEASE BUILD) ----*/
#include "Apostle/Core/Log.h"

/*---- Profiler (REMOVE FROM RELEASE BUILD) ----*/
#include "Apostle/Debug/Instrumentor.h"