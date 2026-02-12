#include "Export.h"
#include "ScriptBehaviour.h"
#include "SwitchSceneFX.h"
#include "MMMTime.h"
#include "Image.h"
#include "RectTransform.h"
#include "Canvas.h"

using namespace MMMEngine;

ObjPtr<SwitchSceneFX> SwitchSceneFX::Instance = nullptr;


void MMMEngine::SwitchSceneFX::Awake()
{
	if (!Instance.IsValid())
	{
		Instance = SelfPtr(this);
		DontDestroyOnLoad(SelfPtr(this));

		if (FXImage.IsValid())
			m_rect = FXImage->GetRectTransform();
	}
	else
	{
		GetGameObject()->Destroy(GetGameObject());
	}
}

void MMMEngine::SwitchSceneFX::OnDestroy()
{
	if (Instance == SelfPtr(this))
		Instance = nullptr;
}

void MMMEngine::SwitchSceneFX::Update()
{
	// 조건 미충족 시 리턴
	if (m_animState == 0 || 
		!m_rect.IsValid() ||
		FXCurve.IsEmpty())
		return;

	float usdt = Time::GetUnscaledDeltaTime();
	usdt = std::min(0.016f, usdt);

	m_timer += usdt;

	switch (m_animState)
	{
		// 가리기 시작
	case 1:
		//anchor min = 0.0f 고정
		//anchor max = 0 -> 1 로
		m_rect->SetAnchorMin({ 0.0f , 0.0f});
		m_rect->SetAnchorMax({ FXCurve.Evaluate(m_timer) , 1.0f});

		if (m_timer > FXCurve.GetKeyframes().back().time)
		{
			m_timer = 0.0f;
			m_animState = 0;
		}

		break;

		// 화면 보이게
	case 2:

		m_rect->SetAnchorMin({ FXCurve.Evaluate(m_timer) , 0.0f });
		m_rect->SetAnchorMax({ 1.0f , 1.0f });

		if (m_timer > FXCurve.GetKeyframes().back().time)
		{
			m_timer = 0.0f;
			m_animState = 0;
		}

		break;
	}
}

int MMMEngine::SwitchSceneFX::GetState()
{
	return m_animState;
}

void MMMEngine::SwitchSceneFX::FXStart()
{
	m_animState = 1;
}

void MMMEngine::SwitchSceneFX::FXEnd()
{
	m_animState = 2;
}
