#pragma once
#include "Core/CoreTypes.h"

namespace LittleEngine
{
namespace GameInit
{
bool Init(s32 argc, char** argv);
void CreateWorlds();
void LoadShaders();
} // namespace GameInit
} // namespace LittleEngine
