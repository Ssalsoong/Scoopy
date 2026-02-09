#include "Export.h"
#include "ScriptBehaviour.h"
#include "RedLine.h"

#include "MMMMathF.h"
#include "Transform.h"
#include "LineRenderer.h"

void MMMEngine::RedLine::Start()
{
}

void MMMEngine::RedLine::Update()
{
	if (PlayerTr.IsValid())
	{
		auto pos = PlayerTr->GetWorldPosition();
		auto dist = pos - GetTransform()->GetWorldPosition();
		auto distX = dist.x;
		auto distY = dist.y;
		bool isDanger = false;
		if (CheckDistance > 0)
		{
			if (CheckXPos && distX < CheckDistance)
			{
				isDanger = true;
			}
			if (CheckYPos && distY < CheckDistance)
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
			if (CheckYPos && distY >= CheckDistance)
			{
				isDanger = true;
			}
		}

		auto sourceCol = ReddoLine->GetColor();
		if (isDanger)
		{
			m_internalTimer += Time::GetDeltaTime();
			float speed = 3.0f;
			float a = (1.0f - cos(m_internalTimer * speed)) * 0.5f;  // 0~1
			ReddoLine->SetColor({ sourceCol.R(),sourceCol.G(),sourceCol.B(),  a});
		}
		else
		{
			m_internalTimer = std::min(1.0f, m_internalTimer);
			m_internalTimer -= Time::GetDeltaTime();
			m_internalTimer = std::max(0.0f, m_internalTimer);
			ReddoLine->SetColor({ sourceCol.R(),sourceCol.G(),sourceCol.B(), m_internalTimer });
		}
	}
}
