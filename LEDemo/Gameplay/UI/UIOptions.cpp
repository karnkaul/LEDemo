#include "Core/Game/ArchiveReader.h"
#include "SFMLAPI/System/Assets.h"
#include "Engine/Audio/LEAudio.h"
#include "Engine/GFX.h"
#include "Engine/Context/LEContext.h"
#include "Engine/Physics/Collider.h"
#include "Engine/Rendering/LERenderer.h"
#include "Engine/Repository/LERepository.h"
#include "Game/Model/GameManager.h"
#include "Game/Model/GameSettings.h"
#include "Game/Model/World/Entity.h"
#include "Game/Model/World/WorldStateMachine.h"
#include "Game/Model/UI/UIElement.h"
#include "Game/Model/UI/UIManager.h"
#include "Game/Framework/Utility/Debug/RenderStatsRenderer.h"
#include "Game/Framework/Utility/Debug/DebugProfiler.h"
#include "Game/Framework/UI/Contexts/UIButtonDrawer.h"
#include "Game/Framework/UI/Widgets/UIButton.h"
#include "Game/Framework/UI/Widgets/UISelection.h"
#include "Game/Framework/UI/Widgets/UIToggle.h"
#include "Game/GameLoop.h"
#include "UIOptions.h"
#include "Gameplay/Components/ControllerComponent.h"

namespace LittleEngine
{
UIOptions::UIOptions() = default;
UIOptions::~UIOptions() = default;

void UIOptions::OnCreated()
{
	Deserialise(g_pRepository->Load<TextAsset>("Texts/Layouts/UIOptions.uil")->Text());

	auto pHelpButton = Find<UIButton>("help_button");
	if (pHelpButton)
	{
		m_tokens.push_back(pHelpButton->AddCallback([]() { LOG_D("Not Implemented"); }));
	}

	auto pQuitButton = Find<UIButton>("quit_button");
	if (pQuitButton)
	{
		m_tokens.push_back(pQuitButton->AddCallback([&]() { g_pGameManager->Quit(); }));
	}

	auto pResolutionSelection = Find<UISelection>("resolution_selection");
	if (pResolutionSelection)
	{
		const auto& resolutions = g_pGFX->ValidViewportSizes();
		std::vector<u32> resolutionHeights;
		std::for_each(resolutions.begin(), resolutions.end(),
					  [&resolutionHeights](const auto& kvp) { resolutionHeights.push_back(kvp.second.height); });
		pResolutionSelection->SetOptions(Strings::ToString(resolutionHeights, "", "p"));
		m_tokens.push_back(pResolutionSelection->RegisterOnChanged([&, resolutionHeights](std::pair<size_t, std::string> kvp) {
			if (g_pGameManager->Context()->TrySetViewportSize(resolutionHeights[kvp.first]))
			{
				GameSettings::Instance().SetViewportHeight(resolutionHeights[kvp.first]);
			}
		}));
	}

	auto pBorderlessToggle = Find<UIToggle>("borderless_toggle");
	if (pBorderlessToggle)
	{
		pBorderlessToggle->SetOn(GameSettings::Instance().IsBorderless());
		m_tokens.push_back(pBorderlessToggle->AddCallback([&](bool bBorderless) {
			GameSettings::Instance().SetBorderless(bBorderless);
			g_pGameManager->Context()->SetWindowStyle(bBorderless ? ViewportStyle::Bordlerless : ViewportStyle::Default);
		}));
	}

	auto pCloseButton = Find<UIButton>("close_button");
	if (pCloseButton)
	{
		m_tokens.push_back(pCloseButton->AddCallback([&]() { Destruct(); }));
	}

	auto pReloadSelection = Find<UISelection>("reload_selection");
	if (pReloadSelection)
	{
		pReloadSelection->SetText("LOC_RELOAD");
		m_tokens.push_back(pReloadSelection->RegisterOnChanged([](std::pair<size_t, std::string> selected) {
			auto type = GameLoop::ReloadType::World;
			if (selected.first < ToIdx(GameLoop::ReloadType::_COUNT))
			{
				type = static_cast<GameLoop::ReloadType>(selected.first);
			}
			GameLoop::Reload(type);
		}));
	}

	auto pDebugButton = Find<UIButton>("debug_button");
	if (pDebugButton)
	{
#if defined(DEBUGGING)
		m_tokens.push_back(pDebugButton->AddCallback([&]() {
			auto pDrawer = g_pGameManager->UI()->PushContext<UIButtonDrawer>("DebugButtonDrawer");
			if (pDrawer)
			{
				pDrawer->m_bAutoDestroyOnCancel = true;

				UIStyle drawerStyle;
				drawerStyle.size = {600, 700};
				drawerStyle.fill = Colour(200, 200, 200, 240);
				drawerStyle.border = 4;
				drawerStyle.outline = Colour::Magenta;
				pDrawer->SetBaseStyle(std::move(drawerStyle), {-Fixed::OneHalf, Fixed::Zero});

#if ENABLED(PROFILER)
				m_tokens.push_back(pDrawer->AddButton("Profiler", []() { Debug::Profiler::Toggle(); }));
#endif
#if ENABLED(RENDER_STATS)
				m_tokens.push_back(pDrawer->AddButton("RenderStats", []() {
					Debug::RenderStatsRenderer::s_bConsoleRenderStatsEnabled = !Debug::RenderStatsRenderer::s_bConsoleRenderStatsEnabled;
				}));
#endif
				m_tokens.push_back(pDrawer->AddButton("Audio", []() { LEAudio::s_bDebugHUD = !LEAudio::s_bDebugHUD; }));
				m_tokens.push_back(pDrawer->AddButton("Colliders", []() { Collider::s_bShowDebugShape = !Collider::s_bShowDebugShape; }));
				m_tokens.push_back(
					pDrawer->AddButton("Orientation Vectors", []() { Entity::s_bShowOrientation = !Entity::s_bShowOrientation; }));

				m_tokens.push_back(pDrawer->AddButton("Controller Orientation", []() {
					ControllerComponent::s_bShowJoystickOrientation = !ControllerComponent::s_bShowJoystickOrientation;
				}));

				pDrawer->ResetSelection();
			}
		}));
#else
		pDebugButton->SetInteractable(false);
#endif
	}
}

void UIOptions::OnActivated()
{
	auto pWorldSelection = Find<UISelection>("world_selection");
	if (pWorldSelection)
	{
		auto stateIDs = g_pGameManager->AllWorldIDs();
		Core::Remove(stateIDs, g_pGameManager->ActiveWorldID());
		pWorldSelection->SetOptions(Strings::ToString(stateIDs, "World "));
		pWorldSelection->SetText("LOC_LOAD_WORLD");
		m_worldSelectToken = pWorldSelection->RegisterOnChanged(
			[&, stateIDs](std::pair<size_t, std::string> selected) { g_pGameManager->LoadWorld(stateIDs[selected.first]); });
	}

	auto pReloadSelection = Find<UISelection>("reload_selection");
	if (pReloadSelection)
	{
		pReloadSelection->SetText("LOC_RELOAD");
	}

	auto pResolutionSelection = Find<UISelection>("resolution_selection");
	if (pResolutionSelection)
	{
		pResolutionSelection->SetText("LOC_RESOLUTION");
	}

	auto pBorderlessToggle = Find<UIToggle>("borderless_toggle");
	if (pBorderlessToggle)
	{
		pBorderlessToggle->SetOn(GameSettings::Instance().IsBorderless());
	}
}
} // namespace LittleEngine
