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

using namespace DirectX::SimpleMath;

void MMMEngine::GameOverSequencer::Start()
{
	std::vector<float> state1MaxTimes = {
		CameraRotXCV.GetKeyframes().back().time,
		CameraRotYCV.GetKeyframes().back().time,
		CameraRotZCV.GetKeyframes().back().time,
		CameraPosYCV.GetKeyframes().back().time,
		CameraPosZCV.GetKeyframes().back().time,
	};

	m_state1_maximumTime = 0.0f;

	for (const auto& t : state1MaxTimes)
	{
		if (m_state1_maximumTime < t)
			m_state1_maximumTime = t;
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
		break;
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
	}

	m_internalTimer = 0.0f;
	m_animState = 1;
	m_startGameOverSequence = true;

	GameOverBGPanel->SetAlpha(0.0f);
}

bool MMMEngine::GameOverSequencer::IsSequenceEnable()
{
	return m_startGameOverSequence;
}
