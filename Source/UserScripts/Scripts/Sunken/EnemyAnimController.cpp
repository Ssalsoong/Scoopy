#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyAnimController.h"
#include "InputManager.h"
#include "Animator.h"

#include "../Dongho/Enemy/Enemy.h"
#include "../Dongho/Enemy/ArrowEnemy.h"
#include "../Dongho/Enemy/NormalEnemy.h"
#include "../Dongho/Enemy/ThiefEnemy.h"

void MMMEngine::EnemyAnimController::UpdateArcher()
{
	if (prevStat != mEnemy->state) {
		prevStat = mEnemy->state;
		switch (prevStat)
		{
		case Enemy::EnemyState::GoToBuilding:
			[[fallthrough]];
		case Enemy::EnemyState::GoToCastle:
			[[fallthrough]];
		case Enemy::EnemyState::ChasePlayer:
			mAnimator->PlayClip("Anim_Goblin_Archer_Move", true);
			break;
		case Enemy::EnemyState::AttackBuilding:
			[[fallthrough]];
		case Enemy::EnemyState::AttackCastle:
			[[fallthrough]];
		case Enemy::EnemyState::AttackPlayer:
			mAnimator->PlayClip("Anim_Goblin_Archer_Attack", true);
			break;
		case Enemy::EnemyState::Dead:
			mAnimator->PlayClip("Archer_Dead", false);
			break;
		default:
			break;
		}
	}
}

void MMMEngine::EnemyAnimController::UpdateWarrior()
{
	if (prevStat != mEnemy->state) {
		prevStat = mEnemy->state;
		switch (prevStat)
		{
		case Enemy::EnemyState::GoToBuilding:
			[[fallthrough]];
		case Enemy::EnemyState::GoToCastle:
			[[fallthrough]];
		case Enemy::EnemyState::ChasePlayer:
			mAnimator->PlayClip("Anim_Goblin_Warrior_Move", true);
			break;
		case Enemy::EnemyState::AttackBuilding:
			[[fallthrough]];
		case Enemy::EnemyState::AttackCastle:
			[[fallthrough]];
		case Enemy::EnemyState::AttackPlayer:
			mAnimator->PlayClip("Anim_Goblin_Warrior_Attack", true);
			break;
		case Enemy::EnemyState::Dead:
			mAnimator->PlayClip("Warrior_Dead", false);
			break;
		default:
			break;
		}
	}
}

void MMMEngine::EnemyAnimController::UpdateScout()
{
	if (prevStat != mEnemy->state) {
		prevStat = mEnemy->state;
		switch (prevStat)
		{
		case Enemy::EnemyState::GoToBuilding:
			[[fallthrough]];
		case Enemy::EnemyState::GoToCastle:
			[[fallthrough]];
		case Enemy::EnemyState::ChasePlayer:
			mAnimator->PlayClip("Anim_Goblin_Scout_Move", true);
			break;
		case Enemy::EnemyState::AttackBuilding:
			[[fallthrough]];
		case Enemy::EnemyState::AttackCastle:
			[[fallthrough]];
		case Enemy::EnemyState::AttackPlayer:
			mAnimator->PlayClip("Anim_Goblin_Scout_Attack", true);
			break;
		case Enemy::EnemyState::Dead:
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
		mEnemy = GetComponent<Enemy>();
		if (!mEnemy.IsValid()) {
			std::cerr << GetName() << "::NO Enemy Component!!!" << std::endl;
			Destroy(SelfPtr(this));
		}
	}

	// 에너미 타입 찾기
	if (!mArcher.IsValid()) {
		mArcher = GetComponent<ArrowEnemy>();
		mAnimType = AT_Archer;
		if (!mArcher.IsValid()) {
			mWarrior = GetComponent<NormalEnemy>();
			mAnimType = AT_Warrior;
			if (!mWarrior.IsValid()) {
				mScout = GetComponent<ThiefEnemy>();
				mAnimType = AT_Scout;
				if (!mScout.IsValid()) {
					std::cerr << GetName() << "::NO EnemyType!!!" << std::endl;
					Destroy(SelfPtr(this));
				}
			}
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
