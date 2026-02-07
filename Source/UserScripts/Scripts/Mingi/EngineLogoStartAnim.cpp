#include "Export.h"
#include "ScriptBehaviour.h"
#include "EngineLogoStartAnim.h"
#include "../Mingi/UI/FadeInOutFX.h"
#include "MMMTime.h"
#include "Image.h"
#include "SceneManager.h"

void MMMEngine::EngineLogoStartAnim::Start()
{
}

void MMMEngine::EngineLogoStartAnim::Update()
{
	// 20프레임 대기
	if (m_state < 20)
	{
		m_state++;
		return;
	}
	m_internalTime += Time::GetDeltaTime();

	switch (m_state)
	{
	case 20:
		if (FadeInOutFX::Instance.IsValid())
			FadeInOutFX::Instance->FadeIn();
		m_state++;
		break;
	case 21:
		if (m_internalTime > 2.0f)
		{
			FadeInOutFX::Instance->FadeOut();
			m_state++;
			m_internalTime = 0.0f;
		}
		break;
	case 22:
		if (m_internalTime > 0.82f && FadeInOutFX::Instance->GetState() == 0)
		{
			m_state++;
			m_internalTime = 0.0f;
			SceneManager::Get().ChangeScene(1);
		}
		break;
	}
}
