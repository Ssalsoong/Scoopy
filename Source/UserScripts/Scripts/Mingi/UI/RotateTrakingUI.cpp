#include "Export.h"
#include "ScriptBehaviour.h"
#include "RotateTrakingUI.h"
#include "Transform.h"
#include "../../Mingi/UI/WorldSpaceUI.h"
#include "Camera.h"
#include "MMMMathF.h"

void MMMEngine::RotateTrakingUI::Start()
{
	auto graphic = WorldSpaceUI->GetComponent<Graphic>();
	if (graphic.IsValid())
	{
		m_canvas = graphic->GetCanvas();
	}
}

void MMMEngine::RotateTrakingUI::Update()
{
	if (!WorldSpaceUI.IsValid())
		return;

	auto target = WorldSpaceUI->TargetTransform;

	if (target.IsValid() && !target->IsDestroyed())
	{
		auto mainCam = Camera::GetMainCamera();
		
		if (!m_canvas.IsValid())
			return;

		if (!mainCam.IsValid() || mainCam->IsDestroyed())
			return;

		Vector3 targetWorldPos = target->GetWorldPosition();
		Vector2 targetScreenPos = mainCam->WorldToScreenPoint(targetWorldPos);

		auto rectTr = GetTransform().Cast<RectTransform>();
		if (!rectTr.IsValid())
			return;

		Vector2 canvasSize = m_canvas->GetCanvasSize();
		Vector4 rect = rectTr->GetRectInCanvas(canvasSize);
		Vector2 pivot = rectTr->GetPivot();

		// UI 자신 위치 (캔버스 좌표)
		Vector2 uiCanvasPos = { rect.x + rect.z * pivot.x, rect.y + rect.w * pivot.y };

		// 타겟 위치 (스크린 -> 캔버스)
		Vector2 targetCanvasPos = mainCam->WorldToScreenPoint(targetWorldPos);
		Vector2 scale = m_canvas->GetScaleToScene();
		Vector2 offset = m_canvas->GetSceneOffset();
		targetCanvasPos.x = (targetCanvasPos.x - offset.x) / (scale.x != 0 ? scale.x : 1.0f);
		targetCanvasPos.y = (targetCanvasPos.y - offset.y) / (scale.y != 0 ? scale.y : 1.0f);

		Vector2 dir = targetCanvasPos - uiCanvasPos;
		if (dir.LengthSquared() > 1e-6f)
		{
			dir.Normalize();
			float angleDeg = DirectX::XMConvertToDegrees(std::atan2(dir.y, dir.x));
			angleDeg -= 90;
			GetTransform()->SetWorldEulerRotation({ 0.0f, 0.0f, angleDeg });
		}
	}
}
