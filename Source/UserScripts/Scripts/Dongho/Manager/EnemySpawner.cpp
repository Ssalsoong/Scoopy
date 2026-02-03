#include "EnemySpawner.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/NormalEnemy.h"
#include "../Enemy/ArrowEnemy.h"
#include "../Enemy/ThiefEnemy.h"
#include "../Player/Player.h"
#include "MMMTime.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "StaticMesh.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<EnemySpawner>("EnemySpawner")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EnemySpawner>"));

	registration::class_<ObjPtr<EnemySpawner>>("ObjPtr<EnemySpawner>")
		.constructor(
			[]() {
				return Object::NewObject<EnemySpawner>();
			}).method("Inject", &ObjPtr<EnemySpawner>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::EnemySpawner> MMMEngine::EnemySpawner::instance = nullptr;

void MMMEngine::EnemySpawner::Start()
{
	instance = GetGameObject()->GetComponent<EnemySpawner>();
	normalenemymesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Enemy/Mesh/Goblin_Warrior_StaticMesh.staticmesh");
	for (int i = 0; i < 40; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("NormalEnemy");
		obj->SetTag("Enemy");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Enemy>();
		obj->AddComponent<NormalEnemy>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(normalenemymesh);
		obj->GetTransform()->SetWorldScale(normalsize);
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		NormalEnemys.push(obj);
	}
	arrowenemymesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Enemy/Mesh/Goblin_Archer_StaticMesh.staticmesh");
	for (int i = 0; i < 20; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("ArrowEnemy");
		obj->SetTag("Enemy");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Enemy>();
		obj->AddComponent<ArrowEnemy>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(arrowenemymesh);
		obj->GetTransform()->SetWorldScale(arrowsize);
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		ArrowEnemys.push(obj);
	}
	thiefenemymesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Enemy/Mesh/Goblin_Scout_StaticMesh.staticmesh");
	for (int i = 0; i < 20; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("ThiefEnemy");
		obj->SetTag("Enemy");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Enemy>();
		obj->AddComponent<ThiefEnemy>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(thiefenemymesh);
		obj->GetTransform()->SetWorldScale(thiefsize);
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		ThiefEnemys.push(obj);
	}
}

void MMMEngine::EnemySpawner::Update()
{

}

void MMMEngine::EnemySpawner::SpawnNormalEnemy(const DirectX::SimpleMath::Vector3& pos)
{
	if (NormalEnemys.empty())
		return;
	auto obj = NormalEnemys.front();
	NormalEnemys.pop();

	if (!obj)
		return;
	obj->GetTransform()->SetWorldPosition(pos);
	obj->GetComponent<NormalEnemy>()->ApplyStats();
	obj->SetActive(true);
}

void MMMEngine::EnemySpawner::SpawnArrowEnemy(const DirectX::SimpleMath::Vector3& pos)
{
	if (ArrowEnemys.empty())
		return;
	auto obj = ArrowEnemys.front();
	ArrowEnemys.pop();

	if (!obj)
		return;
	obj->GetTransform()->SetWorldPosition(pos);
	obj->GetComponent<ArrowEnemy>()->ApplyStats();
	obj->SetActive(true);
}

void MMMEngine::EnemySpawner::SpawnThiefEnemy(const DirectX::SimpleMath::Vector3& pos)
{
	if (ThiefEnemys.empty())
		return;
	auto obj = ThiefEnemys.front();
	ThiefEnemys.pop();

	if (!obj)
		return;
	obj->GetTransform()->SetWorldPosition(pos);
	obj->GetComponent<ThiefEnemy>()->ApplyStats();
	obj->SetActive(true);
}

void MMMEngine::EnemySpawner::EnemyDeath(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<NormalEnemy>())
		NormalEnemys.push(obj);
	if (obj->GetComponent<ArrowEnemy>())
		ArrowEnemys.push(obj);
	if (obj->GetComponent<ThiefEnemy>())
		ThiefEnemys.push(obj);
	if (aliveCount > 0)
		aliveCount--;
}

void MMMEngine::EnemySpawner::WaveSetting(int wave)
{
	if (wave < 1) return;
	aliveCount = 0;
	leftCount = waveTable[wave-1];
	for (int i : leftCount)
		aliveCount += i;
	spawnTimer = 0.0f;
}

bool MMMEngine::EnemySpawner::WaveSpawn(int wave)
{
	spawnTimer += Time::GetDeltaTime();
	if (spawnTimer >= spawnDelay)
	{
		switch (wave)
		{
		case 1:
			if (leftCount[1] > 0) { SpawnNormalEnemy(spawnPos[1]); leftCount[1]--; }
			break;

		case 2:
			if (leftCount[10] > 0) { SpawnNormalEnemy(spawnPos[10]); leftCount[10]--; }
			break;

		case 3:
			if (leftCount[1] > 0) { SpawnArrowEnemy(spawnPos[1]); leftCount[1]--; }
			if (leftCount[4] > 0) { SpawnNormalEnemy(spawnPos[4]); leftCount[4]--; }
			break;

		case 4:
			if (leftCount[10] > 0) { SpawnThiefEnemy(spawnPos[10]); leftCount[10]--; }
			break;

		case 5:
			if (leftCount[6] > 0) { SpawnArrowEnemy(spawnPos[6]); leftCount[6]--; }
			if (leftCount[7] > 0) { SpawnNormalEnemy(spawnPos[7]); leftCount[7]--; }
			break;

		case 6:
			if (leftCount[5] > 0) { SpawnNormalEnemy(spawnPos[5]); leftCount[5]--; }
			if (leftCount[10] > 0) { SpawnArrowEnemy(spawnPos[10]); leftCount[10]--; }
			break;

		case 7:
			if (leftCount[0] > 0) { SpawnArrowEnemy(spawnPos[0]); leftCount[0]--; }
			if (leftCount[1] > 0) { SpawnNormalEnemy(spawnPos[1]); leftCount[1]--; }
			if (leftCount[2] > 0) { SpawnArrowEnemy(spawnPos[2]); leftCount[2]--; }
			break;

		case 8:
			if (leftCount[3] > 0) { SpawnArrowEnemy(spawnPos[3]); leftCount[3]--; }
			if (leftCount[4] > 0) { SpawnNormalEnemy(spawnPos[4]); leftCount[4]--; }
			if (leftCount[10] > 0) { SpawnThiefEnemy(spawnPos[10]); leftCount[10]--; }
			if (leftCount[11] > 0) { SpawnArrowEnemy(spawnPos[11]); leftCount[11]--; }
			break;

		case 9:
			if (leftCount[1] > 0) { SpawnThiefEnemy(spawnPos[1]); leftCount[1]--; }
			if (leftCount[4] > 0) { SpawnNormalEnemy(spawnPos[4]); leftCount[4]--; }
			if (leftCount[6] > 0) { SpawnArrowEnemy(spawnPos[6]); leftCount[6]--; }
			if (leftCount[7] > 0) { SpawnNormalEnemy(spawnPos[7]); leftCount[7]--; }
			break;

		case 10:
			if (leftCount[1] > 0) { SpawnThiefEnemy(spawnPos[1]); leftCount[1]--; }
			if (leftCount[4] > 0) { SpawnArrowEnemy(spawnPos[4]); leftCount[4]--; }
			if (leftCount[7] > 0) { SpawnNormalEnemy(spawnPos[7]); leftCount[7]--; }
			if (leftCount[8] > 0) { SpawnArrowEnemy(spawnPos[8]); leftCount[8]--; }
			if (leftCount[10] > 0) { SpawnNormalEnemy(spawnPos[10]); leftCount[10]--; }
			break;

		default:
			break;
		}
		spawnTimer = 0.0f;
	}
	if (aliveCount == 0)
		return false;
	return true;
}