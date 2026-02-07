#include "SnowballManager2.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SnowCollider.h"
#include "../Dongho/Snow/Snowball.h"
#include "../Dongho/Castle/Castle.h"
#include "../Dongho/Building/Building.h"

void MMMEngine::SnowballManager2::Start()
{

}

void MMMEngine::SnowballManager2::Update()
{
	AssembleSnow();
	ConsumeToCastle();
	ConsumeToBuilding();
}

void MMMEngine::SnowballManager2::Make_snow(ObjPtr<GameObject> player)
{
	if (!Pre_Snow) return;

	auto obj_snow = Instantiate(Pre_Snow);
	Snows.push_back(obj_snow);

	auto p_trans = player->GetTransform();

	if (p_trans.IsValid())
	{
		auto PlayerPosition = p_trans->GetWorldPosition();
		PlayerPosition.y = 0;
		auto PlayerForward = p_trans->GetWorldMatrix().Backward();
		//PlayerPosition.x += (0.2 * PlayerForward.z);
		auto targetPos = PlayerPosition + PlayerForward * 0.3;

		obj_snow->GetTransform()->SetWorldPosition(targetPos);
	}

	//obj_snow->GetComponent<SnowCollider>()->Start();
	obj_snow->GetComponent<SnowCollider>()->SetSize(0.1);
	//obj_snow->GetComponent<SnowCollider>()->SetOnPlayer(true, player);
}




void MMMEngine::SnowballManager2::RemoveFromList(ObjPtr<GameObject> obj)
{
	auto it = std::find(Snows.begin(), Snows.end(), obj);
	if (it != Snows.end()) Snows.erase(it);
}

void MMMEngine::SnowballManager2::AssembleSnow()
{
	ObjPtr<GameObject> mainObj = nullptr;
	ObjPtr<Snowball> mainSnow = nullptr;
	ObjPtr<SnowCollider> mainSc = nullptr;

	for (auto& obj : Snows)
	{
		if (!obj) continue;

		auto sc = obj->GetComponent<SnowCollider>();

		if (sc->CheckOnPlayer())
		{
			mainObj = obj;
			mainSnow = obj->GetComponent<Snowball>();
			mainSc = sc;
			break;
		}
	}
	if (!mainObj || !mainSc) return;

	auto mainPos = mainObj->GetTransform()->GetWorldPosition();
	float mainR = mainSc->GetSize()/2;
	for (int i = 0; i < (int)Snows.size(); ++i)
	{
		auto otherObj = Snows[i];
		if (!otherObj) continue;
		if (otherObj == mainObj) continue;

		auto otherSc = otherObj->GetComponent<SnowCollider>();
		if (!otherSc) continue;

		auto otherPos = otherObj->GetTransform()->GetWorldPosition();
		float otherR = otherSc->GetSize()/2;
		float dx = otherPos.x - mainPos.x;
		float dz = otherPos.z - mainPos.z;
		float sumR = mainR + otherR;

		if (dx * dx + dz * dz <= sumR * sumR)
		{
			mainSnow->EatSnow(otherObj);
			otherSc->SnowDestory();
			Destroy(otherObj, 0.001f);
			--i;
		}
	}
}

void MMMEngine::SnowballManager2::ConsumeToCastle()
{
	ObjPtr<GameObject> mainObj = nullptr;
	ObjPtr<Snowball> mainSnow = nullptr;
	ObjPtr<SnowCollider> mainSc = nullptr;

	for (auto& obj : Snows)
	{
		if (!obj) continue;

		auto sc = obj->GetComponent<SnowCollider>();

		if (sc->CheckOnPlayer())
		{
			mainObj = obj;
			mainSnow = obj->GetComponent<Snowball>();
			mainSc = sc;
			break;
		}
	}
	if (!mainObj || !mainSc) return;

	auto mainPos = mainObj->GetTransform()->GetWorldPosition();
	float mainR = mainSc->GetSize() / 2;

	auto castlepos = m_Castle->GetTransform()->GetWorldPosition();

	float dx = mainPos.x - castlepos.x;
	float dz = mainPos.z - castlepos.z;
	float d2 = dx * dx + dz * dz;

	float sumR = mainR + 0.8f;
	if (d2 > sumR * sumR)return;

	m_Castle->GetComponent<Castle>()->PointUp(mainSnow->GetPoint());
	mainSc->SnowDestory();
	Destroy(mainObj, 0.001f);
}

void MMMEngine::SnowballManager2::ConsumeToBuilding()
{
	ObjPtr<GameObject> mainObj = nullptr;
	ObjPtr<Snowball> mainSnow = nullptr;
	ObjPtr<SnowCollider> mainSc = nullptr;

	for (auto& obj : Snows)
	{
		if (!obj) continue;

		auto sc = obj->GetComponent<SnowCollider>();

		if (sc->CheckOnPlayer())
		{
			mainObj = obj;
			mainSnow = obj->GetComponent<Snowball>();
			mainSc = sc;
			break;
		}
	}
	if (!mainObj || !mainSc) return;

	auto mainPos = mainObj->GetTransform()->GetWorldPosition();
	float mainR = mainSc->GetSize() / 2;

	auto buildings = GameObject::FindGameObjectsWithTag("Building");
	if (buildings.empty()) return;

	ObjPtr<GameObject> bestBuildingObj = nullptr;
	float bestD2 = FLT_MAX;

	const float r = 0.5f + mainR;
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
		bcomp->PointUp(mainSnow->GetPoint());
		mainSc->SnowDestory();
		Destroy(mainObj, 0.001f);
	}
	else
	{
		return;
	}
}
