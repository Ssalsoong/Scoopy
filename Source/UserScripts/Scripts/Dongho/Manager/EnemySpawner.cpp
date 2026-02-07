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
#include "../Battlestats.h"
#include "Prefab.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<EnemySpawner>("EnemySpawner")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EnemySpawner>"))
		.property("pre_normalenemy", &EnemySpawner::pre_normalenemy)
		.property("pre_arrowenemy", &EnemySpawner::pre_arrowenemy)
		.property("pre_thiefenemy", &EnemySpawner::pre_thiefenemy);

	registration::class_<ObjPtr<EnemySpawner>>("ObjPtr<EnemySpawner>")
		.constructor(
			[]() {
				return Object::NewObject<EnemySpawner>();
			}).method("Inject", &ObjPtr<EnemySpawner>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::EnemySpawner> MMMEngine::EnemySpawner::instance = nullptr;

void MMMEngine::EnemySpawner::Start()
{
	pre_normalenemy = ResourceManager::Get().Load<Prefab>(L"Assets/Prefab/NormalEnemy.Prefab");
	pre_arrowenemy = ResourceManager::Get().Load<Prefab>(L"Assets/Prefab/ArrowEnemy.Prefab");
	pre_thiefenemy = ResourceManager::Get().Load<Prefab>(L"Assets/Prefab/ThiefEnemy.Prefab");
	instance = GetGameObject()->GetComponent<EnemySpawner>();
	for (int i = 0; i < 40; ++i)
	{
		auto obj = Instantiate(pre_normalenemy);
		obj->GetTransform()->SetParent(GetTransform());
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		NormalEnemys.push(obj);
	}
	for (int i = 0; i < 20; ++i)
	{
		auto obj = Instantiate(pre_arrowenemy);
		obj->GetTransform()->SetParent(GetTransform());
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		ArrowEnemys.push(obj);
	}
	for (int i = 0; i < 20; ++i)
	{
		auto obj = Instantiate(pre_thiefenemy);
		obj->GetTransform()->SetParent(GetTransform());
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

void MMMEngine::EnemySpawner::EnemyUpgrade()
{
	size_t n = NormalEnemys.size();
	for (size_t i = 0; i < n; ++i)
	{
		auto e = NormalEnemys.front();
		NormalEnemys.pop();
		if (e)
			e->GetComponent<NormalEnemy>()->HP += 3;
		NormalEnemys.push(e);
	}
	size_t a = ArrowEnemys.size();
	for (size_t i = 0; i < a; ++i)
	{
		auto e = ArrowEnemys.front();
		ArrowEnemys.pop();
		if (e)
			e->GetComponent<ArrowEnemy>()->HP += 1;
		ArrowEnemys.push(e);
	}
	size_t t = ThiefEnemys.size();
	for (size_t i = 0; i < t; ++i)
	{
		auto e = ThiefEnemys.front();
		ThiefEnemys.pop();
		if (e)
			e->GetComponent<ThiefEnemy>()->HP += 2;
		ThiefEnemys.push(e);
	}
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