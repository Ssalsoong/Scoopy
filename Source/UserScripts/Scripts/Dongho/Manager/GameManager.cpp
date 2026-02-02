#include "GameManager.h"
#include "../Player/Player.h"
#include "../Castle/Castle.h"
#include "MMMTime.h"
#include "Transform.h"
#include "EnemySpawner.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<GameManager>("GameManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<GameManager>"));

	registration::class_<ObjPtr<GameManager>>("ObjPtr<GameManager>")
		.constructor(
			[]() {
				return Object::NewObject<GameManager>();
			}).method("Inject", &ObjPtr<GameManager>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::GameManager> MMMEngine::GameManager::instance = nullptr;

void MMMEngine::GameManager::Start()
{
	castlemesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Castle/CS_Hexagon_Castle_Stage01_StaticMesh.staticmesh");
	castle = NewObject<GameObject>();
	castle->SetName("Castle");
	castle->SetTag("Castle");
	castle->AddComponent<Castle>();
	castle->AddComponent<MeshRenderer>();
	castle->GetComponent<MeshRenderer>()->SetMesh(castlemesh);
	castle->GetTransform()->SetWorldPosition(0.f, 0.f, 0.f);
	castle->GetTransform()->SetWorldScale(0.17f, 0.17f, 0.17f);
	DirectX::SimpleMath::Quaternion euler{0.f,1.f,0.f,0.f};
	castle->GetTransform()->SetWorldRotation(euler);
	playermesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Player/ANI/Player_Idle_StaticMesh.staticmesh");
	player = NewObject<GameObject>();
	player->SetName("Player");
	player->SetTag("Player");
	player->AddComponent<Player>();
	player->AddComponent<MeshRenderer>();
	player->GetComponent<MeshRenderer>()->SetMesh(playermesh);
	player->GetTransform()->SetWorldPosition(0.f, 0.f, 0.f);
	player->GetTransform()->SetWorldScale(1.2f, 1.2f, 1.2f);
	//player->GetTransform()->SetWorldRotation(euler);

	instance = GetGameObject()->GetComponent<GameManager>();
}

void MMMEngine::GameManager::Update()
{
	auto playercomp = player->GetComponent<Player>();
	auto castlecomp = castle->GetComponent<Castle>();

	/*if (nowSetting)
	{
		settingTimer += Time::GetDeltaTime();
		if (settingTimer >= settingfullTime)
		{
			nowSetting = false;
			settingTimer = 0.0f;
		}
	}
	else
	{
		NorenemySpawnTimer += Time::GetDeltaTime();
		if (NorenemySpawnTimer >= NorenemySpawnDelay) {
			EnemySpawner::instance->SpawnNormalEnemy();
			NorenemySpawnTimer = 0.0f;
		}
		ArenemySpawnTimer += Time::GetDeltaTime();
		if (ArenemySpawnTimer >= ArenemySpawnDelay) {
			EnemySpawner::instance->SpawnArrowEnemy();
			ArenemySpawnTimer = 0.0f;
		}
	}*/
	NorenemySpawnTimer += Time::GetDeltaTime();
	if (NorenemySpawnTimer >= NorenemySpawnDelay) {
		EnemySpawner::instance->SpawnNormalEnemy();
		NorenemySpawnTimer = 0.0f;
	}
	ArenemySpawnTimer += Time::GetDeltaTime();
	if (ArenemySpawnTimer >= ArenemySpawnDelay) {
		EnemySpawner::instance->SpawnArrowEnemy();
		ArenemySpawnTimer = 0.0f;
	}



	if (playercomp->PlayerDeath() || castlecomp->CastleDeath())
	{
		GameOver = true;
	}
}