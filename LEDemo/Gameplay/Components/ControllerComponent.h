#pragma once
#include "Game/Model/World/Component.h"

namespace LittleEngine
{
class ControllerComponent : public AComponent
{
public:
	enum class Scheme
	{
		Joystick = 0,
		KBM,
	};

#if defined(DEBUGGING)
public:
	static bool s_bShowJoystickOrientation;
	static std::array<Colour, 2> s_ornColours;
	static Vector2 s_orientationWidthHeight;

private:
	class SFRect* m_pRect = nullptr;
#endif
public:
	static Fixed s_orientationEpsilon;

protected:
	static constexpr f64 s_XY_DEADZONE = 0.01;
	static constexpr f64 s_ORN_DEADZONE = 0.5;

public:
	Fixed m_angularSpeed = Fixed::One;
	Fixed m_linearSpeed = Fixed::One;

protected:
	Vector2 m_displacement;
	Vector2 m_targetOrn = Vector2::Up;
	Vector2 m_prevMousePos;
	Scheme m_active = Scheme::Joystick;
	Token m_ptrToken;
	class RenderComponent* m_pRenderComponent = nullptr;

public:
	ControllerComponent();
	~ControllerComponent() override;

protected:
	void OnCreated() override;

public:
	void Tick(Time dt) override;

public:
	TimingType Timing() const override;
	void SetEnabled(bool bEnabled) override;

public:
	void Reset();

private:
	bool OnInput(const LEInput::Frame& frame);
};
} // namespace LittleEngine
