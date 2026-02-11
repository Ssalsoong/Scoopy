#include "Export.h"
#include "ScriptBehaviour.h"
#include "SwitchSceneFX.h"
#include "MMMTime.h"
#include "Image.h"
#include "Canvas.h"

using namespace MMMEngine;

ObjPtr<SwitchSceneFX> SwitchSceneFX::Instance = nullptr;


void MMMEngine::SwitchSceneFX::Awake()
{
	if (!Instance.IsValid())
	{
		Instance = SelfPtr(this);
		DontDestroyOnLoad(SelfPtr(this));

		
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
	// idle 시 리턴
	if (m_animState == 0)
		return;

	m_timer += Time::GetUnscaledDeltaTime();

	switch (m_animState)
	{
		// 가리기 시작
	case 1:
		
		break;

		// 화면 보이게
	case 2:
		break;
	}
}

int MMMEngine::SwitchSceneFX::GetState()
{
	return m_animState;
}

void MMMEngine::SwitchSceneFX::FXStart()
{
}

void MMMEngine::SwitchSceneFX::FXEnd()
{
}
