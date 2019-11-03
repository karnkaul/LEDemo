#pragma once
#include "Game/Framework/UI/Widgets/UIButton.h"
#include "Game/Framework/UI/Containers/UIContainer.h"

namespace LittleEngine
{
class UIOptions final : public UIContainer
{
private:
	Token m_worldSelectToken;
	std::vector<Token> m_tokens;

public:
	UIOptions();
	~UIOptions() override;

protected:
	void OnCreated() override;
	void OnActivated() override;
};
} // namespace LittleEngine
