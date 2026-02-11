#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyAnimController.h"
#include "InputManager.h"
#include "Animator.h"

#include "../test/EnemyController.h"

void MMMEngine::EnemyAnimController::UpdateArcher()
{
	if (prevStat != mEnemy->curState) {
		prevStat = mEnemy->curState;
		switch (prevStat)
		{
		case EnemyController::EnemyState::Move:
			mAnimator->PlayClip("Anim_Goblin_Archer_Move", true);
			break;
		case EnemyController::EnemyState::Attack:
			mAnimator->PlayClip("Anim_Goblin_Archer_Attack", true);
			break;
		case EnemyController::EnemyState::Dead:
			mAnimator->PlayClip("Archer_Dead", false);
			break;
		default:
			break;
		}
	}
}

void MMMEngine::EnemyAnimController::UpdateWarrior()
{
	if (prevStat != mEnemy->curState) {
		prevStat = mEnemy->curState;
		switch (prevStat)
		{
		case EnemyController::EnemyState::Move:
			mAnimator->PlayClip("Anim_Goblin_Warrior_Move", true);
			break;
		case EnemyController::EnemyState::Attack:
			mAnimator->PlayClip("Anim_Goblin_Warrior_Attack", true);
			break;
		case EnemyController::EnemyState::Dead:
			mAnimator->PlayClip("Warrior_Dead", false);
			break;
		default:
			break;
		}
	}
}

void MMMEngine::EnemyAnimController::UpdateScout()
{
	if (prevStat != mEnemy->curState) {
		prevStat = mEnemy->curState;
		switch (prevStat)
		{
		case EnemyController::EnemyState::Move:
			mAnimator->PlayClip("Anim_Goblin_Scout_Move", true);
			break;
		case EnemyController::EnemyState::Attack:
			mAnimator->PlayClip("Anim_Goblin_Scout_Attack", true);
			break;
		case EnemyController::EnemyState::Dead:
			mAnimator->PlayClip("Scout_Dead", false);
			break;
		default:
			break;
		}
	}
}

void MMMEngine::EnemyAnimController::Start()
{
	// 없으면 찾기, 생성
	if (!mAnimManager.IsValid()) {
		mAnimManager = AnimResourceManager::Get();

		if (!mAnimManager) {
			std::cerr << GetName() << "::NO AnimResourceManager!!!" << std::endl;
			Destroy(SelfPtr(this));
		}
	}

	if (!mAnimator.IsValid()) {
		mAnimator = GetGameObject()->AddComponent<Animator>();
		if (!mAnimator.IsValid()) {
			std::cerr << GetName() << "::NO Animator!!!" << std::endl;
			Destroy(SelfPtr(this));
		}
	}

	if (!mEnemy.IsValid()) {
		mEnemy = GetComponent<EnemyController>();
		if (!mEnemy.IsValid()) {
			std::cerr << GetName() << "::NO Enemy Component!!!" << std::endl;
			Destroy(SelfPtr(this));
		}
	}

	 //에너미 타입 찾기
	if (auto enemyController = GetComponent<EnemyController>();enemyController.IsValid())
	{
		auto type = enemyController->GetType();
		switch (type)
		{
		case EnemyController::EnemyType::Warrior:
			mAnimType = AT_Warrior;
			break;
		case EnemyController::EnemyType::Archer:
			mAnimType = AT_Archer;
			break;
		case EnemyController::EnemyType::Scout:
			mAnimType = AT_Scout;
			break;
		default:
			break;
		}
	}


	auto clips = mAnimManager->GetAnimClips(mAnimType);

	// 클립 불러오기 실패시 리턴
	if (clips == nullptr) {
		std::cerr << GetName() << "::NO AnimClips!!!" << std::endl;
		return;
	}
		
	AnimSize = (int)clips->size();

	for (auto& clip : *clips) {
		mAnimator->AddAnimClip(clip);
	}
}

void MMMEngine::EnemyAnimController::Update()
{
	// TODO::재생속도 설정하기 
	mAnimator->SetSpeed(mAnimSpeed);

	/*if (InputManager::Get().GetKeyDown(KeyCode::Alpha1))
		mAnimator->PlayClip("Anim_Goblin_Archer_Move", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha2))
		mAnimator->PlayClip("Anim_Goblin_Archer_Attack", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha3))
		mAnimator->PlayClip("Archer_Dead", false);*/

	switch (mAnimType)
	{
	case MMMEngine::AT_Archer:
		UpdateArcher();
		break;
	case MMMEngine::AT_Warrior:
		UpdateWarrior();
		break;
	case MMMEngine::AT_Scout:
		UpdateScout();
		break;
	default:
		Destroy(SelfPtr(this));
		break;
	}
}

void MMMEngine::EnemyAnimController::SetAniType(AnimType type)
{
	mAnimType = type;
	auto clips = mAnimManager->GetAnimClips(mAnimType);

	// 클립 불러오기 실패시 리턴
	if (clips == nullptr) {
		std::cerr << GetName() << "::NO AnimClips!!!" << std::endl;
		return;
	}

	AnimSize = (int)clips->size();

	for (auto& clip : *clips) {
		mAnimator->AddAnimClip(clip);
	}
}