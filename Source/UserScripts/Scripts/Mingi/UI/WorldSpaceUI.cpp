#include "Export.h"
#include "ScriptBehaviour.h"
#include "WorldSpaceUI.h"
#include "WorldSpaceUISorter.h"
#include "Camera.h"
#include "Canvas.h"

//WorldSpaceUIManager가 targetPos를 받아서 z축으로 UIRenderOrder를 설정해주도록 변경할 예정

bool MMMEngine::WorldSpaceUI::IsTargetInScreen()
{
	return m_isTargetInScreen;
}

void MMMEngine::WorldSpaceUI::Awake()
{
	//WorldSpaceUI 매니저에 자신을 등록시키기
	//매니저가 없는 경우 경고 로그 출력

	if (WorldSpaceUISorter::Instance.IsValid()) {
		WorldSpaceUISorter::Instance->RegisterUI(SelfPtr(this));
	}
	else {
		// 경고 로그 (엔진 로그 함수 사용)
	}
}

void MMMEngine::WorldSpaceUI::Start()
{
	m_rectGraphic = GetGameObject()->GetComponent<Graphic>();
}

void MMMEngine::WorldSpaceUI::LateUpdate()
{
	if(!SelfRect.IsValid() || !TargetTransform.IsValid() || !m_rectGraphic.IsValid())
		return;

	//if (!SelfRect->GetGameObject().IsValid() || SelfRect->GetGameObject()->IsDestroyed())
	//	return;

	//if (!TargetTransform->GetGameObject().IsValid() || TargetTransform->GetGameObject()->IsDestroyed())
	//	return;

	auto mainCam = Camera::GetMainCamera();

	if(!mainCam.IsValid() || mainCam->IsDestroyed())
		return;

	Vector3 targetWorldPos = TargetTransform->GetWorldPosition();
	Vector2 targetScreenPos = mainCam->WorldToScreenPoint(targetWorldPos);
	Vector3 camPos = mainCam->GetGameObject()->GetTransform()->GetWorldPosition();

	auto canvas = m_rectGraphic->GetCanvas();

	if (canvas.IsValid())
	{
		m_lastDistance = Vector3::Distance(targetWorldPos, camPos);

		if (ScaleWithDistance && m_lastDistance > 0.001f)
		{
			// 공식: (기준 거리 / 현재 거리)
			float scaleFactor = ReferenceDistance / m_lastDistance;

			// 최소/최대 제한 (Clamping)
			if (scaleFactor < MinScale) scaleFactor = MinScale;
			if (scaleFactor > MaxScale) scaleFactor = MaxScale;

			SelfRect->SetLocalScale(Vector3(scaleFactor, scaleFactor, 1.0f));
		}

		// ScaleWithScreenSize 대응
		Vector2 scale = canvas->GetScaleToScene();
		Vector2 offset = canvas->GetSceneOffset();

		// 스크린 좌표 -> 캔버스 좌표
		targetScreenPos.x = (targetScreenPos.x - offset.x) / (scale.x != 0 ? scale.x : 1.0f);
		targetScreenPos.y = (targetScreenPos.y - offset.y) / (scale.y != 0 ? scale.y : 1.0f);



		Vector2 canSize = canvas->GetCanvasSize();
		m_isTargetInScreen = targetScreenPos.x > EdgeXOffset && targetScreenPos.x < canSize.x - EdgeXOffset && targetScreenPos.y > EdgeYOffset && targetScreenPos.y < canSize.y - EdgeYOffset;

		if (HideWhenOutsideScreen)
		{
			if (m_isTargetInScreen)
			{
				// 화면 내에 존재함
				m_rectGraphic->SetEnabled(false);
				if (m_rectGraphic->GetTransform()->GetChildCount() > 0)
					m_rectGraphic->GetTransform()->GetChild(0)->GetGameObject()->SetActive(false);
			}
			else
			{
				// 화면 밖에 존재함
				m_rectGraphic->SetEnabled(true);
				if (m_rectGraphic->GetTransform()->GetChildCount() > 0)
					m_rectGraphic->GetTransform()->GetChild(0)->GetGameObject()->SetActive(true);
			}
		}

		if (ClampToScreenEdge)
		{
			targetScreenPos.x = canSize.x - EdgeXOffset < targetScreenPos.x ? canSize.x - EdgeXOffset : (EdgeXOffset > targetScreenPos.x ? EdgeXOffset : targetScreenPos.x);
			targetScreenPos.y = canSize.y - EdgeYOffset < targetScreenPos.y ? canSize.y - EdgeYOffset : (EdgeYOffset > targetScreenPos.y ? EdgeYOffset : targetScreenPos.y);
		}

		Vector2 anchorCenter, anchorSpan;
		SelfRect->GetAnchorData(canvas->GetCanvasSize(), anchorCenter, anchorSpan);
		SelfRect->SetAnchoredPosition(targetScreenPos - anchorCenter);
	}
}


void MMMEngine::WorldSpaceUI::OnDestroy()
{
	//WorldSpaceUI 매니저에서 자신을 해제시키기
	//SceneChange의 경우 매니저가 파괴되었을 수도 있으니 꼭 Valid + IsDestroyed 체크할 것
	if (WorldSpaceUISorter::Instance.IsValid() && !WorldSpaceUISorter::Instance->IsDestroyed()) {
		WorldSpaceUISorter::Instance->UnregisterUI(SelfPtr(this));
	}
}
