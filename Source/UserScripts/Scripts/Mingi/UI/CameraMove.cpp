#include "Export.h"
#include "ScriptBehaviour.h"
#include "CameraMove.h"
#include "Transform.h"
#include "MMMTime.h"

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
	m_currentPos = Vector3::Lerp(m_currentPos, targetPos, ChasingSpeed * Time::GetDeltaTime());
	GetTransform()->SetLocalPosition(m_currentPos + Offset);
}
