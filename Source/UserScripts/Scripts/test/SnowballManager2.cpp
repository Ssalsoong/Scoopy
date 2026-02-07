#include "SnowballManager2.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SnowCollider.h"

void MMMEngine::SnowballManager2::Start()
{
	
}

void MMMEngine::SnowballManager2::Update()
{

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
