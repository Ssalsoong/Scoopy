#include "Export.h"
#include "ScriptBehaviour.h"
#include "ClearEmitTrail.h"
#include "TrailRenderer.h"
#include "GameObject.h"

void MMMEngine::ClearEmitTrail::OnDisable()
{
	if (!GetGameObject().IsValid() || GetGameObject()->IsDestroyed())
		return;

	if (auto trail = GetComponent<TrailRenderer>(); trail.IsValid())
	{
		trail->Clear();
	}
}
