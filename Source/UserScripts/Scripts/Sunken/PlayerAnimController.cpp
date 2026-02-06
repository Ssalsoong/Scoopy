#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerAnimController.h"
#include "AnimResourceManager.h"
#include "Animator.h"
#include "TimeManager.h"
#include "AnimatorController.h"

void MMMEngine::PlayerAnimController::CreateAnimNode()
{
	AnimState as = {};
	as.clipName = "Player_Idle";
	as.loop = true;
	as.rootIdx = mAnimator->GetBoneIdx("Hip");

	mAnimController->AddState("Idle", as);
	
	as.clipName = "Player_Move";
	mAnimController->AddState("Move", as);

	as.clipName = "Player_Scoop_Idle";
	mAnimController->AddState("ScoopIdle", as);

	as.clipName = "Player_Scoop_Move";
	mAnimController->AddState("ScoopMove", as);

	as.clipName = "Player_Attack";
	as.rootIdx = mAnimator->GetBoneIdx("Bone");
	mAnimController->AddState("ScoopAttack", as);
}

void MMMEngine::PlayerAnimController::CreateParams()
{
	mAnimController->SetFloat("MoveSpeed", 0.0f);
}

void MMMEngine::PlayerAnimController::CreateTrans()
{
	mAnimController->SetEntryState("Idle");

	AnimTransition at;
	at.toState = "Move";
	at.conditions
	mAnimController->AddTransition("Idle", at);
	at.toState = "ScoopIdle";
	mAnimController->AddTransition("Idle", at);
}

void MMMEngine::PlayerAnimController::Start()
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

	if (!mAnimController.IsValid()) {
		mAnimController = GetGameObject()->AddComponent<AnimatorController>();
		if (!mAnimController.IsValid()) {
			std::cerr << GetName() << "::NO AnimController!!!" << std::endl;
			Destroy(SelfPtr(this));
		}
	}

	auto clips = mAnimManager->GetAnimClips(AnimType::AT_Player);

	// 클립 불러오기 실패시 리턴
	if (clips == nullptr) {
		std::cerr << GetName() << "::NO AnimClips!!!" << std::endl;
		return;
	}

	// 노드생성
	CreateAnimNode();


	mAnimSize = (int)clips->size();

	for (auto& clip : *clips) {
		mAnimator->AddAnimClip(clip);
	}
}

void MMMEngine::PlayerAnimController::Update()
{
	mAnimator->SetSpeed(mAnimSpeed);

	if (isAtking)
		mAtkElipsedTime += TimeManager::Get().GetDeltaTime();

	static PSTAT prevStat = PSTAT::END;
	if (prevStat != mCurrStat) {
		prevStat = mCurrStat;

		switch (prevStat)
		{
		case MMMEngine::PlayerAnimController::IDLE:
			mAnimator->PlayClip("Player_Idle", true);
			break;
		case MMMEngine::PlayerAnimController::MOVE:
			mAnimator->PlayClip("Player_Move", true);
			break;
		case MMMEngine::PlayerAnimController::ATTACK:
			mAnimator->PlayClip("Player_Attack", true);
			mAnimator->
			break;
		case MMMEngine::PlayerAnimController::MOVEATTACK:
			mAnimator->PlayClip("Player_Move_Attack", true);
			break;
		case MMMEngine::PlayerAnimController::SCOOPIDLE:
			break;
		case MMMEngine::PlayerAnimController::SCOOPMOVE:
			break;
		case MMMEngine::PlayerAnimController::END:
			break;
		default:
			break;
		}
	}
}

void MMMEngine::PlayerAnimController::SetAnimSpeed(float _speed)
{
	mAnimSpeed = _speed;
}

void MMMEngine::PlayerAnimController::SetMoveSpeed(float _speed)
{

}

void MMMEngine::PlayerAnimController::SetAttack(bool _isAttacking)
{
	if (_isAttacking) {
		isAtking = true;
		mCurrStat = PSTAT::ATTACK;
	}
	else {
		isAtking = false;
		mCurrStat = PSTAT::END;
	}
}

void MMMEngine::PlayerAnimController::PlayAttack()
{

}
