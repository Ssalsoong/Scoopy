#include "Export.h"
#include "ScriptBehaviour.h"
#include "CameraMove.h"
#include "Transform.h"
#include "MMMTime.h"

#include <cmath>

void MMMEngine::CameraMove::Start()
{
	if (!Target.IsValid())
	{
		if (auto pl = GameObject::Find("Player"); pl.IsValid())
			Target = pl->GetTransform();
	}

	m_currentPos = Target->GetTransform()->GetWorldPosition();
}

void MMMEngine::CameraMove::Update()
{
	if (!Target.IsValid())
		return;

	auto targetPos = Target->GetWorldPosition();

	targetPos.x = std::clamp(targetPos.x, XClamp.x, XClamp.y);
	targetPos.z = std::clamp(targetPos.z, ZClamp.x, ZClamp.y);

	m_currentPos = Vector3::Lerp(m_currentPos, targetPos, ChasingSpeed * Time::GetDeltaTime());
	GetTransform()->SetLocalPosition(m_currentPos + Offset);
}
