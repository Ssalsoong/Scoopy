#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowTrigger.h"
#include "EnemyMove.h"
#include "Transform.h"
#include "SphereColliderComponent.h"
#include "PlayerController.h"

void MMMEngine::SnowTrigger::Start()
{
		ColObj = GetComponent<ColliderComponent>();
		if(ColObj.IsValid())
		{
			ColObj->SetTriggerQueryEnabled(true);
		}
		//SetTriggerSize(0);
}

void MMMEngine::SnowTrigger::Update()
{

}

void MMMEngine::SnowTrigger::SetTriggerSize(float size)
{
	if (ColObj.IsValid())
	{
		auto SphereCol = GetComponent<SphereColliderComponent>();
		SphereCol->SetRadius(size/2 + 0.5f);
		//auto Trans = GetTransform();
		//if (Trans.IsValid())
		//{
		//	auto cur_pos = Trans->GetLocalPosition();
		//	cur_pos.y = size / 2;
		//	Trans->SetLocalPosition(cur_pos);
		//}
	}
}

void MMMEngine::SnowTrigger::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player")
	{
		//Parent_Obj->GetComponent<snowscript> 눈의 matrix가 플레이어에 영향받도록
		is_player = true;
		auto P_Control = info.other->GetComponent<PlayerController>();
		if (P_Control.IsValid())
		{
			main_Player = P_Control->GetGameObject();
			main_Player->GetComponent<PlayerController>()->AddSnowList(Parent_Obj);
		}

	}
}

void MMMEngine::SnowTrigger::OnTriggerExit(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player")
	{
		is_player = false;
		auto P_Control = info.other->GetComponent<PlayerController>();
		if (P_Control.IsValid())
		{
			main_Player->GetComponent<PlayerController>()->RemoveSnowList(Parent_Obj);
			main_Player = nullptr;
		}
	}
}

void MMMEngine::SnowTrigger::SetParentPtr(ObjPtr<GameObject> mamaoooh)
{
	Parent_Obj = mamaoooh;
}

void MMMEngine::SnowTrigger::DestoryTrigger()
{
	if (is_player)
	{
		auto P_Con = main_Player->GetComponent<PlayerController>();
		P_Con->HasSnow(false);
		P_Con->RemoveSnowList(Parent_Obj);
		main_Player = nullptr;
	}
}
