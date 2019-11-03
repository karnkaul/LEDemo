#include "Core/Game/OS.h"
#include "Engine/Rendering/LEShaders.h"
#include "Game/GameLoop.h"
#include "Game/Model/GameConfig.h"
#include "Game/Model/GameSettings.h"
#include "Game/Model/World/WorldStateMachine.h"
#include "Game/Framework/Framework.h"
#include "GameInit.h"
#include "Worlds.h"
#include "Components/ControllerComponent.h"

namespace LittleEngine
{
extern Fixed g_maxParticlesScale;

bool GameInit::Init(s32 argc, char** argv)
{
	OS::Env()->SetVars(argc, argv, {GameLoop::Res::COOKED_ASSETS.c_str(), GameLoop::Res::MAIN_MANIFEST_FILE.c_str()});
	GameConfig& config = GameConfig::Instance();
	config.Init();
#if !defined(SHIPPING)
	LOG_D("[GameLoop] Loading config...");
	config.Load(OS::Env()->FullPath(GameLoop::Res::GAME_CONFIG_FILE.c_str()));
#endif

	const auto& settings = GameSettings::Instance();
	f32 maxParticlesScale = 1.0f;
	auto pStr = settings.GetValue("LOW_QUALITY");
	if (pStr && Strings::ToBool(*pStr))
	{
		maxParticlesScale = 0.25f;
	}
	pStr = settings.GetValue("MAX_PARTICLES_SCALE");
	if (pStr)
	{
		maxParticlesScale = Strings::ToF32(*pStr, maxParticlesScale);
	}
	g_maxParticlesScale = Fixed(maxParticlesScale);
	LE::g_MinLogSeverity = settings.LogLevel();
	ControllerComponent::s_orientationEpsilon = config.ControllerOrientationEpsilon();
	if (config.ShouldCreateRenderThread())
	{
		if (OS::Threads::VacantThreadCount() == 0)
		{
			LOG_W("[GameLoop] Insufficient threads to create render thread!\n!ERROR! Async Renderer not available!");
			config.m_bRenderThread = false;
		}
	}
	if (!GameLoop::Init())
	{
		return false;
	}
#if defined(DEBUGGING)
	ControllerComponent::s_orientationWidthHeight = config.ControllerOrientationSize();
	Entity::s_orientationWidthHeight = config.EntityOrientationSize();
	Collider::s_debugShapeWidth = config.ColliderBorderWidth();
#endif
	Locale::Init(settings.LocdataID(), settings.ENLocdataID());
	Time::Reset();
	return true;
}

void GameInit::CreateWorlds()
{
	// Create Worlds and manage their IDs (if required) here
	// WorldStateMachine::CreateWorld<T> will return the WorldID assigned to the created world.
	// WorldStateMachine::CreateWorlds<T...> will create worlds in order of template invocation and return nothing.

	WorldStateMachine::CreateWorlds<BootWorld, TestWorld>();
}

void GameInit::LoadShaders()
{
	// Load SFShaders here

	LEShaders::LoadShader<Shader>("Default", Shader::Type().set(ToIdx(Shader::Flag::Vertex)).set(ToIdx(Shader::Flag::Fragment)));
}
} // namespace LittleEngine
