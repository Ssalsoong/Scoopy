#include "Export.h"
#include "ScriptBehaviour.h"
#include "GameOverSequencer.h"
#include "Image.h"
#include "Text.h"
#include "RectTransform.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "MMMMathF.h"
#include "Camera.h"
#include "../../Mingi/UI/CameraMove.h"
#include "../../Mingi/UI/SwitchSceneFX.h"
#include "../../Mingi/Manager/SoundManager.h"
#include "MMMSceneManagement.h"

using namespace DirectX::SimpleMath;

void MMMEngine::GameOverSequencer::Start()
{

	if (GameOverText.IsValid())
	{
		m_gameoverRect = GameOverText->GetRectTransform();
		m_firstGameOverPos = m_gameoverRect->GetAnchoredPosition();
		m_firstGameOverRot = m_gameoverRect->GetWorldEulerRotation();
	}

	if (ReplayText.IsValid())
	{
		m_replayRect = ReplayText->GetRectTransform();
		m_firstReplayPos = m_replayRect->GetAnchoredPosition();
	}

	if (ToTitleText.IsValid())
	{
		m_totitleRect = ToTitleText->GetRectTransform();
		m_firstToTitlePos = m_totitleRect->GetAnchoredPosition();
	}

	std::vector<float> state1MaxTimes = {
		CameraRotXCV.GetKeyframes().back().time,
		CameraRotYCV.GetKeyframes().back().time,
		CameraRotZCV.GetKeyframes().back().time,
		CameraPosYCV.GetKeyframes().back().time,
		CameraPosZCV.GetKeyframes().back().time,
	};

	std::vector<float> state3MaxTimes = {
		GameOverPosXCV.GetKeyframes().back().time,
		GameOverPosYCV.GetKeyframes().back().time,
		GameOverRotZCV.GetKeyframes().back().time
	};

	m_state1_maximumTime = 0.0f;
	m_state3_maximumTime = 0.0f;

	for (const auto& t : state1MaxTimes)
	{
		if (m_state1_maximumTime < t)
			m_state1_maximumTime = t;
	}

	for (const auto& t : state3MaxTimes)
	{
		if (m_state3_maximumTime < t)
			m_state3_maximumTime = t;
	}
}

void MMMEngine::GameOverSequencer::Update()
{
	//debug
	if (Input::GetKeyDown(KeyCode::G))
	{
		//연출 시작
		StartGameOver();
	}


	if (m_animState == 0 || !m_mainCam.IsValid())
		return;

	m_internalTimer += Time::GetDeltaTime();

	switch (m_animState)
	{
	case 1:
		m_mainCam->GetTransform()->SetWorldPosition(m_firstCamPos + Vector3{ 0, CameraPosYCV.Evaluate(m_internalTimer),CameraPosZCV.Evaluate(m_internalTimer)});
		m_mainCam->GetTransform()->SetWorldEulerRotation(m_firstCamRot + Vector3{ CameraRotXCV.Evaluate(m_internalTimer), CameraRotYCV.Evaluate(m_internalTimer), CameraRotZCV.Evaluate(m_internalTimer) });
		
		if (m_internalTimer > m_state1_maximumTime)
		{
			m_animState++;
			m_internalTimer = 0.0f;
		}

		break;
	case 2:
		GameOverBGPanel->SetAlpha(PanelCV.Evaluate(m_internalTimer));
		if (PanelCV.GetKeyframes().back().time < m_internalTimer)
		{
			m_animState++;
			m_internalTimer = 0.0f;
		}
		break;
	case 3:
		m_gameoverRect->SetAnchoredPosition(m_firstGameOverPos + Vector2{ GameOverPosXCV.Evaluate(m_internalTimer),GameOverPosYCV.Evaluate(m_internalTimer) });
		m_gameoverRect->SetWorldEulerRotation(m_firstGameOverRot + Vector3{0,0,GameOverRotZCV.Evaluate(m_internalTimer)});
		if (m_internalTimer > m_state3_maximumTime)
		{
			m_animState++;
			m_internalTimer = 0.0f;
		}
		break;
	case 4:
		m_replayRect->SetAnchoredPosition(m_firstReplayPos + Vector2{  0, ButtonYPosCV.Evaluate(m_internalTimer) });
		m_totitleRect->SetAnchoredPosition(m_firstToTitlePos + Vector2{ 0,ButtonYPosCV.Evaluate(m_internalTimer - ButtonOffsetTime) });
		ReplayText->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer) / 0.355f, 1.0f)) * 0.5f });
		ToTitleText->SetColor({ 1.0f,1.0f,1.0f, std::max(0.0f, std::min((m_internalTimer - ButtonOffsetTime) / 0.355f, 1.0f)) * 0.5f });
		if (m_internalTimer - ButtonOffsetTime > (ButtonYPosCV.GetKeyframes().back().time - ButtonOffsetTime))
		{
			m_animState++;
			m_isControlAble = true;
			m_internalTimer = 0.0f;
		}
		break;
	case 6:
		m_internalTimer += Time::GetUnscaledDeltaTime();

		if (SwitchSceneFX::Instance.IsValid() &&
			(SwitchSceneFX::Instance->GetState() != 0 ||
				m_internalTimer < m_sceneChangeTime))
		{
			return;
		}

		Time::SetTimeScale(1.0f);
		if (m_currentSelected == 0)
		{
			SceneManagement::ChangeScene(GameSceneName);
		}
		else
		{
			SceneManagement::ChangeScene(TitleSceneName);
		}

		if (SwitchSceneFX::Instance.IsValid())
			SwitchSceneFX::Instance->FXEnd();

		if (SoundManager::Instance.IsValid())
			SoundManager::Instance->StopBGM();
		return;
	}

	if (m_isControlAble)
	{
		bool leftKey = Input::GetKeyDown(KeyCode::LeftArrow);
		bool rightKey = Input::GetKeyDown(KeyCode::RightArrow);

		if (leftKey || rightKey)
		{
			m_internalTimer = 0.0f;
			m_currentSelected = 1 - m_currentSelected;
		}


		bool selectKey = Input::GetKeyDown(KeyCode::Enter) || Input::GetKeyDown(KeyCode::Space);

		if (selectKey)
		{
			if (SwitchSceneFX::Instance.IsValid())
				SwitchSceneFX::Instance->FXStart();
			m_isControlAble = false;
			m_internalTimer = 0.0f;
			m_animState++;
		}

		float targetResume = (m_currentSelected == 0) ? 1.0f : 0.5f;
		float targetToTitle = (m_currentSelected == 1) ? 1.0f : 0.5f;

		float dt = Time::GetDeltaTime();
		float t = std::min(1.0f, m_focusFadeSpeed * dt);

		m_replayFocusAlpha = m_replayFocusAlpha + (targetResume - m_replayFocusAlpha) * t;
		m_totitleFocusAlpha = m_totitleFocusAlpha + (targetToTitle - m_totitleFocusAlpha) * t;

		ReplayText->SetColor({ 1.0f,1.0f,1.0f, m_replayFocusAlpha });
		ToTitleText->SetColor({ 1.0f,1.0f,1.0f, m_totitleFocusAlpha });

		m_replayRect->SetLocalScale({ (m_currentSelected == 0) ? ButtonScaleCV.Evaluate(m_internalTimer) : 1.0f ,(m_currentSelected == 0) ? ButtonScaleCV.Evaluate(m_internalTimer) : 1.0f ,1.0f });
		m_totitleRect->SetLocalScale({ (m_currentSelected == 1) ? ButtonScaleCV.Evaluate(m_internalTimer) : 1.0f ,(m_currentSelected == 1) ? ButtonScaleCV.Evaluate(m_internalTimer) : 1.0f ,1.0f });
	}
}

void MMMEngine::GameOverSequencer::StartGameOver()
{
	m_mainCam = Camera::GetMainCamera();

	if (auto mainCamMove = m_mainCam->GetComponent<CameraMove>(); mainCamMove.IsValid())
	{
		mainCamMove->SetEnabled(false);
	}

	if (!m_startGameOverSequence)
	{
		m_firstCamPos = m_mainCam->GetTransform()->GetWorldPosition();
		m_firstCamRot = m_mainCam->GetTransform()->GetWorldEulerRotation();
	}
	else
	{
		m_mainCam->GetTransform()->SetWorldPosition(m_firstCamPos);
		m_mainCam->GetTransform()->SetWorldEulerRotation(m_firstCamRot);

		m_gameoverRect->SetAnchoredPosition(m_firstGameOverPos);
		m_gameoverRect->SetWorldEulerRotation(m_firstGameOverRot);

		std::vector<float> state1MaxTimes = {
		CameraRotXCV.GetKeyframes().back().time,
		CameraRotYCV.GetKeyframes().back().time,
		CameraRotZCV.GetKeyframes().back().time,
		CameraPosYCV.GetKeyframes().back().time,
		CameraPosZCV.GetKeyframes().back().time,
		};

		std::vector<float> state3MaxTimes = {
			GameOverPosXCV.GetKeyframes().back().time,
			GameOverPosYCV.GetKeyframes().back().time,
			GameOverRotZCV.GetKeyframes().back().time
		};

		m_state1_maximumTime = 0.0f;
		m_state3_maximumTime = 0.0f;

		for (const auto& t : state1MaxTimes)
		{
			if (m_state1_maximumTime < t)
				m_state1_maximumTime = t;
		}

		for (const auto& t : state3MaxTimes)
		{
			if (m_state3_maximumTime < t)
				m_state3_maximumTime = t;
		}
	}

	m_internalTimer = 0.0f;
	m_animState = 1;
	m_startGameOverSequence = true;
	m_isControlAble = false;

	m_replayFocusAlpha = 0.5f;
	m_totitleFocusAlpha = 0.5f;

	ReplayText->SetColor({ 1,1,1,0 });
	ToTitleText->SetColor({ 1,1,1,0 });


	m_replayRect->SetLocalScale({1,1,1});
	m_totitleRect->SetLocalScale({ 1,1,1 });

	GameOverBGPanel->SetAlpha(0.0f);
}

bool MMMEngine::GameOverSequencer::IsSequenceEnable()
{
	return m_startGameOverSequence;
}
