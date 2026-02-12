#include "Export.h"
#include "ScriptBehaviour.h"
#include "GameClearSequencer.h"
#include "Image.h"
#include "Text.h"
#include "RectTransform.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "MMMMathF.h"
#include "../../Mingi/UI/FadeInOutFX.h"
#include "../../Mingi/Manager/SoundManager.h"
#include "MMMSceneManagement.h"

void MMMEngine::GameClearSequencer::Start()
{
}

void MMMEngine::GameClearSequencer::StartGameClear()
{
	m_internalTimer = 0.0f;
	m_animState = 1;
}

void MMMEngine::GameClearSequencer::Update()
{
	switch (m_animState)
	{
	case 1:
		m_internalTimer += Time::GetUnscaledDeltaTime();
		GameClearBGPanel->SetAlpha(GameClearBGAlphaCV.Evaluate(m_internalTimer));

		GameClearImg->SetColor({ 1,1,1,GameClearAlphaCV.Evaluate(m_internalTimer) });
		GameClearImg->GetRectTransform()->SetLocalScale({ 1.0f + GameClearScaleXCV.Evaluate(m_internalTimer) , 1.0f + GameClearScaleYCV.Evaluate(m_internalTimer), 1.0f });
		GameClearImg->GetRectTransform()->SetWorldEulerRotation({ 0,0,GameClearRotZCV.Evaluate(m_internalTimer) });

		if (m_internalTimer > 2.5f)
		{
			FadeInOutFX::Instance->FadeOut();
			m_animState++;
			m_internalTimer = 0.0f;
		}

		break;
	case 2:

		m_internalTimer += Time::GetUnscaledDeltaTime();

		if (m_internalTimer > 1.45f && FadeInOutFX::Instance->GetState() == 0)
		{
			SceneManagement::ChangeScene(NextSceneName);
			SoundManager::Instance->StopBGM();
			FadeInOutFX::Instance->FadeIn();
		}

		break;
	}
}
