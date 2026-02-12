#include "Export.h"
#include "ScriptBehaviour.h"
#include "PauseUI.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Text.h"
#include "MMMTime.h"
#include "MMMInput.h"
#include "MMMSceneManagement.h"
#include "../../Mingi/UI/SwitchSceneFX.h"
#include "../../Mingi/Manager/SoundManager.h"

using namespace DirectX::SimpleMath;

void MMMEngine::PauseUI::Start()
{
	if (P_rect.IsValid())
	{
		m_P_text = P_rect->GetComponent<Text>();
		m_P_pos = P_rect->GetAnchoredPosition();
	}
	if (a_rect.IsValid())
	{
		m_a_text = a_rect->GetComponent<Text>();
		m_a_pos = a_rect->GetAnchoredPosition();
	}
	if (u_rect.IsValid())
	{
		m_u_text = u_rect->GetComponent<Text>();
		m_u_pos = u_rect->GetAnchoredPosition();
	}
	if (s_rect.IsValid())
	{
		m_s_text = s_rect->GetComponent<Text>();
		m_s_pos = s_rect->GetAnchoredPosition();
	}
	if (e_rect.IsValid())
	{
		m_e_text = e_rect->GetComponent<Text>();
		m_e_pos = e_rect->GetAnchoredPosition();
	}
	if (Resume_rect.IsValid())
	{
		m_Resume_text = Resume_rect->GetComponent<Text>();
		m_Resume_pos = Resume_rect->GetAnchoredPosition();
	}
	if (ToTitle_rect.IsValid())
	{
		m_ToTitle_text = ToTitle_rect->GetComponent<Text>();
		m_ToTitle_pos = ToTitle_rect->GetAnchoredPosition();
	}
}

void MMMEngine::PauseUI::Update()
{
	if (m_skipPauseUIUpdate)
	{
		m_internalTimer += Time::GetUnscaledDeltaTime();

		if (SwitchSceneFX::Instance.IsValid() && 
			(SwitchSceneFX::Instance->GetState() != 0 || 
			m_internalTimer < SceneChangeTime))
		{
			return;
		}

		Time::SetTimeScale(1.0f);
		SceneManagement::ChangeScene(TitleSceneName);

		if (SwitchSceneFX::Instance.IsValid())
			SwitchSceneFX::Instance->FXEnd();

		if (SoundManager::Instance.IsValid())
			SoundManager::Instance->StopBGM();
		return;
	}


	// todo : 게임매니저에서 게임이 끝난상태에서는 호출하지 않게하기 Return
	bool lastPause = m_isPause;
	if (Input::GetKeyDown(KeyCode::Escape))
	{
		m_isPause = !m_isPause;
	}

	bool currentFrameHasControl = false;
	if (m_isControlAble)
	{
		m_buttonSelectTimer += Time::GetUnscaledDeltaTime();
		bool upKey = Input::GetKeyDown(KeyCode::UpArrow);
		bool downKey = Input::GetKeyDown(KeyCode::DownArrow);

		currentFrameHasControl = upKey || downKey;

		if (upKey || downKey)
			m_currentSelected = 1 - m_currentSelected;


		bool selectKey = Input::GetKeyDown(KeyCode::Enter) || Input::GetKeyDown(KeyCode::Space);

		if (selectKey)
		{
			if (m_currentSelected == 0)
			{
				m_isPause = false;
			}
			if (m_currentSelected == 1)
			{
				//타이틀로 
				//FX 요청
				if (SwitchSceneFX::Instance.IsValid())
					SwitchSceneFX::Instance->FXStart();
				m_skipPauseUIUpdate = true;
				m_internalTimer = 0.0f;
				return;
			}
		}
	}

	if (lastPause != m_isPause)
	{
		//애니메이션 체인지
		m_internalTimer = 0.0f;
		PanelGO->SetActive(m_isPause);
		m_isControlAble = false;
		m_resumeFocusAlpha = 0.5f;
		m_toTitleFocusAlpha = 0.5f;
		m_currentSelected = 0;
		Resume_rect->SetLocalScale({ 1.0f , 1.0f ,1.0f });
		ToTitle_rect->SetLocalScale({ 1.0f , 1.0f ,1.0f });
		m_buttonSelectTimer = 0.0f;

		Time::SetTimeScale(m_isPause ? 0.0f : 1.0f);
	}

	

	if (currentFrameHasControl)
	{
		m_buttonSelectTimer = Time::GetUnscaledDeltaTime();
	}

	if (m_isPause)
	{
		m_internalTimer += Time::GetUnscaledDeltaTime();

		m_P_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min(m_internalTimer / FadeDuration, 1.0f)) });
		m_a_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - (AnimOffestTime)) / FadeDuration, 1.0f)) });
		m_u_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - (AnimOffestTime * 2)) / FadeDuration, 1.0f)) });
		m_s_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - (AnimOffestTime * 3)) / FadeDuration, 1.0f)) });
		m_e_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - (AnimOffestTime * 4)) / FadeDuration, 1.0f)) });


		if (!m_isControlAble)
		{
			m_Resume_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - (AnimOffestTime * buttonAnimOffset)) / FadeDuration, 1.0f)) * 0.5f });
			m_ToTitle_text->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - (AnimOffestTime * (buttonAnimOffset + 1))) / FadeDuration, 1.0f)) * 0.5f });

		}
		else
		{
			float targetResume = (m_currentSelected == 0) ? 1.0f : 0.5f;
			float targetToTitle = (m_currentSelected == 1) ? 1.0f : 0.5f;

			float dt = Time::GetUnscaledDeltaTime();
			float t = std::min(1.0f, FocusFadeSpeed * dt);

			m_resumeFocusAlpha = m_resumeFocusAlpha + (targetResume - m_resumeFocusAlpha) * t;
			m_toTitleFocusAlpha = m_toTitleFocusAlpha + (targetToTitle - m_toTitleFocusAlpha) * t;

			m_Resume_text->SetColor({ 1.0f,1.0f,1.0f, m_resumeFocusAlpha });
			m_ToTitle_text->SetColor({ 1.0f,1.0f,1.0f, m_toTitleFocusAlpha });


			Resume_rect->SetLocalScale({ (m_currentSelected == 0) ? ButtonScaleCurve.Evaluate(m_buttonSelectTimer) : 1.0f ,(m_currentSelected == 0) ? ButtonScaleCurve.Evaluate(m_buttonSelectTimer) : 1.0f ,1.0f});
			ToTitle_rect->SetLocalScale({ (m_currentSelected == 1) ? ButtonScaleCurve.Evaluate(m_buttonSelectTimer) : 1.0f ,(m_currentSelected == 1) ? ButtonScaleCurve.Evaluate(m_buttonSelectTimer) : 1.0f ,1.0f});
		}
		
		P_rect->SetAnchoredPosition(m_P_pos + Vector2{0.0f, PosYCurve.Evaluate(m_internalTimer)});
		a_rect->SetAnchoredPosition(m_a_pos + Vector2{0.0f, PosYCurve.Evaluate(m_internalTimer - (AnimOffestTime))});
		u_rect->SetAnchoredPosition(m_u_pos + Vector2{0.0f, PosYCurve.Evaluate(m_internalTimer - (AnimOffestTime * 2))});
		s_rect->SetAnchoredPosition(m_s_pos + Vector2{0.0f, PosYCurve.Evaluate(m_internalTimer - (AnimOffestTime * 3))});
		e_rect->SetAnchoredPosition(m_e_pos + Vector2{0.0f, PosYCurve.Evaluate(m_internalTimer - (AnimOffestTime * 4))});

		Resume_rect->SetAnchoredPosition(m_Resume_pos + Vector2{ 0.0f, ButtonYCurve.Evaluate(m_internalTimer - (AnimOffestTime * buttonAnimOffset)) });
		ToTitle_rect->SetAnchoredPosition(m_ToTitle_pos + Vector2{ 0.0f, ButtonYCurve.Evaluate(m_internalTimer - (AnimOffestTime * (buttonAnimOffset+1))) });

		if (!m_isControlAble && m_internalTimer - (AnimOffestTime * (buttonAnimOffset + 1)) > (ButtonYCurve.GetKeyframes().back().time - ButtonSelectTimeOffset))
		{
			m_isControlAble = true;
		}

	}
}

void MMMEngine::PauseUI::OnDestroy()
{
	if (m_isPause)
	{
		Time::SetTimeScale(1.0f);
	}
}

void MMMEngine::PauseUI::ForcePauseOff()
{
	m_internalTimer = 0.0f;
	PanelGO->SetActive(false);
	m_isControlAble = false;
	m_resumeFocusAlpha = 0.5f;
	m_toTitleFocusAlpha = 0.5f;
	m_currentSelected = 0;
	Resume_rect->SetLocalScale({ 1.0f , 1.0f ,1.0f });
	ToTitle_rect->SetLocalScale({ 1.0f , 1.0f ,1.0f });
	m_buttonSelectTimer = 0.0f;

	Time::SetTimeScale(1.0f);
}

bool MMMEngine::PauseUI::IsPause()
{
	return m_isPause;
}
