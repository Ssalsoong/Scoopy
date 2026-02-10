#include "Export.h"
#include "ScriptBehaviour.h"
#include "RedLine.h"

#include "MMMMathF.h"
#include "Transform.h"
#include "LineRenderer.h"

void MMMEngine::RedLine::Start()
{
	if (ReddoLine.IsValid())
		m_baseColor = ReddoLine->GetColor();
}

void MMMEngine::RedLine::Update()
{
	if (PlayerTr.IsValid())
	{
		auto pos = PlayerTr->GetWorldPosition();
		auto dist = pos - GetTransform()->GetWorldPosition();
		auto distX = dist.x;
		auto distZ = dist.z;
		bool isDanger = false;
		if (CheckDistance > 0)
		{
			if (CheckXPos && distX < CheckDistance)
			{
				isDanger = true;
			}
			if (CheckZPos && distZ < CheckDistance)
			{
				isDanger = true;
			}
		}
		else
		{
			if (CheckXPos && distX >= CheckDistance)
			{
				isDanger = true;
			}
			if (CheckZPos && distZ >= CheckDistance)
			{
				isDanger = true;
			}
		}

		m_internalTimer = std::min(1.0f, m_internalTimer);
		if (isDanger)
		{
			m_internalTimer += Time::GetDeltaTime();
		}
		else
		{
			m_internalTimer -= Time::GetDeltaTime();
		}

		m_internalTimer = std::max(0.0f, m_internalTimer);


		ReddoLine->SetColor({ m_baseColor.R(),m_baseColor.G(),m_baseColor.B(), m_internalTimer });
	}
}