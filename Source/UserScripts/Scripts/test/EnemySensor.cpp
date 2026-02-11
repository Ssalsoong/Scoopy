#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemySensor.h"
#include "EnemyController.h"
#include "SphereColliderComponent.h"

void MMMEngine::EnemySensor::Start()
{
	ColObj = GetComponent<SphereColliderComponent>();
}

void MMMEngine::EnemySensor::OnTriggerEnter(MMMEngine::TriggerInfo info)
{
	TargetList.insert(info.other);

	std::cout << "Trigger enter: other=" << info.other->GetTag() << std::endl;
	std::cout << "Trigger enter: otherid = " << info.other->GetInstanceID() << std::endl;
}

void MMMEngine::EnemySensor::OnTriggerExit(MMMEngine::TriggerInfo info)
{
	TargetList.erase(info.other);
}

MMMEngine::ObjPtr<MMMEngine::GameObject> MMMEngine::EnemySensor::GetTarget()
{
	int bestPriority = INT_MAX;

	if (TargetList.empty()) return nullptr;
	for (auto& TList : TargetList)
	{
		if (!TList.IsValid())
		{
			TargetList.erase(TList);
			continue;
		};

		int t_Priority = GetPriority(TList);

		if (t_Priority < bestPriority)
		{
			bestPriority = t_Priority;
			MainTarget = TList;
		}
	}
	if (bestPriority == INT_MAX) return nullptr;
	return MainTarget;
}

int MMMEngine::EnemySensor::GetPriority(const ObjPtr<GameObject> obj)
{
	if (obj->GetTag() =="Building") return 0;
	//Todo :: 플레이어가 나를 때렸다는 정보가 필요함 -> 동호씨 로직 연결 필요
	if (obj->GetTag() == "Player")
	{
		if (auto it = EnemyObj->GetComponent<EnemyController>(); (it.IsValid() && it->CheckHurt()))
		{
			if (!it.IsValid())
			{
				std::cout << "Enemy not found" << std::endl;
			}
			it->OnHurtFlag(false);
			return 1;
		}
	}
	if (obj->GetTag() == "Snow") return 2;
	if (obj->GetTag() == "Castle") {
		int a = 0;
		return 3;
	};

	return INT_MAX;
}
