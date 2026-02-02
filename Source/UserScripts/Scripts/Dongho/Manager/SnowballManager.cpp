#include "SnowballManager.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "../Snow/Snowball.h"
#include "../Player/Player.h"
#include "Transform.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "MeshRenderer.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<SnowballManager>("SnowballManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowballManager>"));

	registration::class_<ObjPtr<SnowballManager>>("ObjPtr<SnowballManager>")
		.constructor(
			[]() {
				return Object::NewObject<SnowballManager>();
			}).method("Inject", &ObjPtr<SnowballManager>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::SnowballManager> MMMEngine::SnowballManager::instance = nullptr;

void MMMEngine::SnowballManager::Start()
{
	snowmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Snowball/snowball_StaticMesh.staticmesh");
	castle = GameObject::Find("Castle");
	if (castle)
	{
		castlecomp = castle->GetComponent<Castle>();
		castletr = castle->GetTransform();
	}
	instance = GetGameObject()->GetComponent<SnowballManager>();
}

void MMMEngine::SnowballManager::Update()
{
	if (!castletr|| !castlecomp) return;
	castlepos = castle->GetTransform()->GetWorldPosition();

	AssembleSnow();
	SnowToCastle();
	SnowToBuilding();
}

void MMMEngine::SnowballManager::OnScoopStart(Player& player)
{
	if (player.GetMatchedSnowball()) return;
	ObjPtr<GameObject> nearest = nullptr;
	auto playerPos = player.GetTransform()->GetWorldPosition();
	for (auto& sObj : Snows)
	{
		if (!sObj) continue;
		auto sc = sObj->GetComponent<Snowball>();
		if (!sc) continue;
		if (sc->IsCarried()) continue;

		auto snowPos = sObj->GetTransform()->GetWorldPosition();
		float bestD = sc->GetScale() * 2.8f;
		float bestD2 = bestD * bestD;
		float dx = snowPos.x - playerPos.x;
		float dz = snowPos.z - playerPos.z;
		float d2 = dx * dx + dz * dz;

		if (d2 < bestD2) {
			bestD2 = d2;
			nearest = sObj;
		}
	}
	if (nearest)
	{
		nearest->GetComponent<Snowball>()->carrier = &player;
		player.AttachSnowball(nearest);
		player.SnapToSnowball();
		scoopStates[&player] = {};
	}
	else
	{
		auto& st = scoopStates[&player];
		st.active = true;
		st.holdTime = 0.0f;
	}
}

void MMMEngine::SnowballManager::OnScoopHold(Player& player)
{
	// 이미 들고 있으면 생성할 이유 없음
	if (player.GetMatchedSnowball()) return;

	auto it = scoopStates.find(&player);
	if (it == scoopStates.end()) return;

	auto& st = it->second;
	if (!st.active) return;

	st.holdTime += Time::GetDeltaTime();
	if (st.holdTime < snowSpawnDelay) return;

	//여기서 지금 장판이 눈 장판인지 체크해야함

	// 눈 생성 + 등록
	auto obj = NewObject<GameObject>();
	obj->SetName("Snowball");
	obj->SetTag("Snowball");
	obj->AddComponent<Snowball>();
	obj->AddComponent<MeshRenderer>();
	obj->GetComponent<MeshRenderer>()->SetMesh(snowmesh);
	obj->GetTransform()->SetWorldScale(snowscale);
	Snows.push_back(obj);

	// 상태 전이(매니저 책임) + 플레이어 매칭
	if (auto sc = obj->GetComponent<Snowball>())
	{
		sc->carrier = &player;
	}
	player.AttachSnowball(obj);

	// 홀드 상태 종료/리셋
	st.active = false;
	st.holdTime = 0.0f;
}

void MMMEngine::SnowballManager::OnScoopEnd(Player& player)
{
	// 홀드/스폰 상태 정리 (매칭 유무와 무관)
	auto it = scoopStates.find(&player);
	if (it != scoopStates.end())
	{
		it->second.active = false;
		it->second.holdTime = 0.0f;
	}

	auto sObj = player.GetMatchedSnowball();
	if (!sObj) return;

	auto sc = sObj->GetComponent<Snowball>();
	if (sc)
	{
		sc->carrier = nullptr;
	}

	player.DetachSnowball();
}

void MMMEngine::SnowballManager::RemoveFromList(ObjPtr<GameObject> obj)
{
	auto it = std::find(Snows.begin(), Snows.end(), obj);
	if (it != Snows.end()) Snows.erase(it);
}

void MMMEngine::SnowballManager::AssembleSnow()
{
	ObjPtr<GameObject> mainObj = nullptr;
	ObjPtr<Snowball> mainSc = nullptr;

	for (auto& obj : Snows)
	{
		if (!obj) continue;

		auto sc = obj->GetComponent<Snowball>();
		if (!sc) continue;

		if (sc->IsCarried())
		{
			mainObj = obj;
			mainSc = sc;
			break;
		}
	}

	if (!mainObj || !mainSc) return;

	// 여기서부터 물리 충돌로 반드시 변경

	auto mainPos = mainObj->GetTransform()->GetWorldPosition();
	float mainR = mainSc->GetScale()/2;

	for (int i = 0; i < (int)Snows.size(); ++i)
	{
		auto& otherObj = Snows[i];
		if (!otherObj) continue;
		if (otherObj == mainObj) continue;

		auto scOther = otherObj->GetComponent<Snowball>();
		if (!scOther) continue;
		if (scOther->IsCarried()) continue; // 컨트롤 중인 애는 대상 제외

		auto otherPos = otherObj->GetTransform()->GetWorldPosition();
		float otherR = scOther->GetScale() / 2;

		float dx = otherPos.x - mainPos.x;
		float dz = otherPos.z - mainPos.z;
		float sumR = mainR + otherR;

		if (dx * dx + dz * dz <= sumR * sumR)
		{
			//이 두 함수가 충돌 시 실행되어야 함
			mainSc->EatSnow(otherObj);
			RemoveFromList(otherObj);
			//
			--i;
		}
	}
}

//이 밑의 두 함수는 물리 충돌 판정으로 반드시 변경

void MMMEngine::SnowballManager::SnowToCastle()
{
	ObjPtr<GameObject> mainObj = nullptr;
	ObjPtr<Snowball> mainSc = nullptr;

	for (auto& obj : Snows)
	{
		if (!obj) continue;

		auto sc = obj->GetComponent<Snowball>();
		if (!sc) continue;

		if (sc->IsCarried())
		{
			mainObj = obj;
			mainSc = sc;
			break;
		}
	}

	if (!mainObj || !mainSc) return;

	auto mainPos = mainObj->GetTransform()->GetWorldPosition();

	float dx = mainPos.x - castlepos.x;
	float dz = mainPos.z - castlepos.z;
	float d2 = dx * dx + dz * dz;

	float r = 1.0f;
	if (d2 > r * r) return;

	castlecomp->PointUp(mainSc->GetPoint());

	if (auto player = mainSc->carrier)
	{
		player->DetachSnowball();
		mainSc->carrier = nullptr;
	}

	Destroy(mainObj);
	RemoveFromList(mainObj);
}

void MMMEngine::SnowballManager::SnowToBuilding()
{
	ObjPtr<GameObject> mainObj = nullptr;
	ObjPtr<Snowball> mainSc = nullptr;

	for (auto& obj : Snows)
	{
		if (!obj) continue;

		auto sc = obj->GetComponent<Snowball>();
		if (!sc) continue;

		if (sc->IsCarried())
		{
			mainObj = obj;
			mainSc = sc;
			break;
		}
	}

	if (!mainObj || !mainSc) return;

	auto mainPos = mainObj->GetTransform()->GetWorldPosition();

	auto buildings = GameObject::FindGameObjectsWithTag("Building");
	if (buildings.empty()) return;

	ObjPtr<GameObject> bestBuildingObj = nullptr;
	float bestD2 = FLT_MAX;

	const float r = 1.0f;
	const float r2 = r * r;

	for (auto& b : buildings)
	{
		if (!b) continue;

		auto bcomp = b->GetComponent<Building>();
		if (!bcomp) continue;

		auto bpos = b->GetTransform()->GetWorldPosition();

		float dx = mainPos.x - bpos.x;
		float dz = mainPos.z - bpos.z;
		float d2 = dx * dx + dz * dz;

		if (d2 <= r2 && d2 < bestD2)
		{
			bestD2 = d2;
			bestBuildingObj = b;
		}
	}

	if (!bestBuildingObj) return;

	if (auto bcomp = bestBuildingObj->GetComponent<Building>())
	{
		bcomp->PointUp(mainSc->GetPoint());
	}
	else
	{
		return;
	}

	if (auto player = mainSc->carrier)
	{
		player->DetachSnowball();
		mainSc->carrier = nullptr;
	}

	Destroy(mainObj);
	RemoveFromList(mainObj);
}