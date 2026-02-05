#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowTrigger.h"
#include "EnemyMove.h"
#include "Transform.h"

void MMMEngine::SnowTrigger::Start()
{
	auto ColObj = GetComponent<ColliderComponent>();
		if(ColObj.IsValid())
		{
			ColObj->SetTriggerQueryEnabled(true);
			/*uint32_t t = 6;
			ColObj->SetLayer(t);*/
		}
	
}

void MMMEngine::SnowTrigger::Update()
{

}

//void MMMEngine::SnowTrigger::OnTriggerEnter(MMMEngine::CollisionInfo info)
//{
//	if (info.other->GetTag() == "Enemy")
//	{
//		auto Enemy_Move = info.other->GetComponent<EnemyMove>();
//		if (!Enemy_Move.IsValid()) {
//			std::cout << "Enemy Component not found" << std::endl;
//			return;
//		}
//
//		if (auto t_self = info.self->GetTransform(); t_self.IsValid()) {
//			auto parentTr = t_self->GetParent();
//			if (!parentTr.IsValid()) return;
//			auto parentGo = parentTr->GetGameObject();
//			if (!parentGo.IsValid()) return;
//
//			Enemy_Move->AddObjPtr(parentGo);
//		}
//	}
//}
//
//void MMMEngine::SnowTrigger::OnTriggerExit(MMMEngine::CollisionInfo info)
//{
//	if (info.other->GetTag() == "Enemy")
//	{
//		auto Enemy_Move = info.other->GetComponent<EnemyMove>();
//		if (!Enemy_Move.IsValid()) {
//			std::cout << "Enemy Component not found" << std::endl;
//			return;
//		}
//
//		if (auto t_self = info.self->GetTransform(); t_self.IsValid()) {
//			auto parentTr = t_self->GetParent();
//			if (!parentTr.IsValid()) return;
//			auto parentGo = parentTr->GetGameObject();
//			if (!parentGo.IsValid()) return;
//
//			Enemy_Move->AddObjPtr(parentGo);
//		}
//	}
//}
