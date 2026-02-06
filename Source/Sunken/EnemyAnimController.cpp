#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyAnimController.h"
#include "InputManager.h"

void MMMEngine::EnemyAnimController::Start()
{
	// 없으면 찾기
	if (!mAnimManager) {
		auto arm = GameObject::Find("AnimResourceManager");
		if (arm)
			mAnimManager = arm->GetComponent<AnimResourceManager>();
	}
	
	if (!mAnimManager) {
		std::cerr << GetName() << "::NO AnimResourceManager!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mAnimator) {
		mAnimator = GetComponent<Animator>();
	}

	if (!mAnimator) {
		std::cerr << GetName() << "::NO Animator!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	auto clips = mAnimManager->GetAnimClips(AnimType::Archer);

	// 클립 불러오기 실패시 리턴
	if (clips == nullptr)
		return;

	for (auto& clip : *clips) {
		mAnimator->AddAnimClip(clip);
	}
}

void MMMEngine::EnemyAnimController::Update()
{
	if(InputManager::Get().GetKeyDown(KeyCode::Alpha0))
		mAnimator->PlayClip("Anim_Goblin_Archer_Attack", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha1))
		mAnimator->PlayClip("Anim_Goblin_Archer_Move", true);

}
