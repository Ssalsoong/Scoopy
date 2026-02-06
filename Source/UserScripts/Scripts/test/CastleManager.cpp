#include "Export.h"
#include "ScriptBehaviour.h"
#include "CastleManager.h"
#include "ColliderComponent.h"
#include "MeshColliderComponent.h"

void MMMEngine::CastleManager::Start()
{
	auto MeshCol = GetComponent<ColliderComponent>();
	if (MeshCol.IsValid())
	{
		MeshCol->UpdateShapeGeometry();
	}
}

void MMMEngine::CastleManager::Update()
{
}
