#include "Export.h"
#include "ScriptBehaviour.h"
#include "BuildingPoint.h"
#include "../Player/Player.h"
#include "Transform.h"
#include "MeshRenderer.h"

void MMMEngine::BuildingPoint::Start()
{
	player = GetGameObject()->Find("Player");
}

void MMMEngine::BuildingPoint::Update()
{
	if (!player) return;
	playerpos = player->GetTransform()->GetWorldPosition();
	if (player->GetComponent<Player>()->buildchance)
	{
		GetComponent<MeshRenderer>()->SetEnabled(true);
	}
	else
	{
		GetComponent<MeshRenderer>()->SetEnabled(false);
	}
	CheckPlayer();
}

void MMMEngine::BuildingPoint::CheckPlayer()
{
	auto pos = GetTransform()->GetWorldPosition();
	float bestD2 = checkdist * checkdist;
	float dx = pos.x - playerpos.x;
	float dz = pos.z - playerpos.z;
	float d2 = dx * dx + dz * dz;

	if (d2 < bestD2 && !alreadybuilt)
	{
		canBuild = true;
	}
	else
	{
		canBuild = false;
	}
}