#include "Export.h"
#include "ScriptBehaviour.h"
#include "MiniMap.h"
#include "Transform.h"
#include "RectTransform.h"

void MMMEngine::MiniMap::Start()
{
}

void MMMEngine::MiniMap::LateUpdate()
{
	if (!Player.IsValid() || !PlayerRectUI.IsValid())
		return;

	//PlayerUI를 시험삼아 움직여볼꺼임
	PlayerRectUI->SetAnchoredPosition({ Player->GetWorldPosition().x * MinimapUnitScale,Player->GetWorldPosition().z * MinimapUnitScale } );
}
