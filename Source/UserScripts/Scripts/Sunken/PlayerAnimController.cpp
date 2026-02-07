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

	//as.clipName = "Player_Scoop_Idle";
	//as.rootIdx = mAnimator->GetBoneIdx("Bone");	// 스쿱만 움직일수 있나? 확인하기
	//mAnimController->AddState("ScoopIdle", as);

	//as.clipName = "Player_Scoop_Move";
	//mAnimController->AddState("ScoopMove", as);

	//as.clipName = "Player_Attack";
	//as.rootIdx = mAnimator->GetBoneIdx("Bone");
	//mAnimController->AddState("Attack", as);
}

void MMMEngine::PlayerAnimController::CreateParams()
{
	mAnimController->SetFloat("MoveSpeed", 0.0f);
	/*mAnimController->SetBool("Scoop", false);
	mAnimController->SetBool("Attack", false);*/
}

void MMMEngine::PlayerAnimController::CreateTrans()
{
	mAnimController->SetEntryState("Idle");

	// IDLE State
	{
		AnimCondition cd = {};
		cd.param = "MoveSpeed";
		cd.op = CondOp::Greater;
		cd.value = mIdleSpeed;

		AnimTransition at;
		at.toState = "Move";
		at.conditions.push_back(cd);
		mAnimController->AddTransition("Idle", at);
	}
	/*{
		AnimCondition cd = {};
		cd.param = "Scoop";
		cd.op = CondOp::Equal;
		cd.value = 1.0f;

		AnimTransition at;
		at.toState = "ScoopIdle";
		at.conditions.push_back(cd);
		mAnimController->AddTransition("Idle", at);
	}*/
	/*{
		AnimCondition cd = {};
		cd.param = "Attack";
		cd.op = CondOp::Equal;
		cd.value = 1.0f;

		AnimTransition at;
		at.toState = "Attack";
		at.conditions.push_back(cd);
		mAnimController->AddTransition("Idle", at);
	}*/

	// MOVE
	{
		AnimCondition cd = {};
		cd.param = "MoveSpeed";
		cd.op = CondOp::Less;
		cd.value = mIdleSpeed;

		AnimTransition at;
		at.toState = "Idle";
		at.conditions.push_back(cd);
		mAnimController->AddTransition("Move", at);
	}
	/*{
		AnimCondition cd = {};
		cd.param = "Attack";
		cd.op = CondOp::Equal;
		cd.value = 1.0f;

		AnimTransition at;
		at.toState = "Attack";
		at.conditions.push_back(cd);
		mAnimController->AddTransition("Move", at);
	}*/

	
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

	mAnimSize = (int)clips->size();

	for (auto& clip : *clips) {
		mAnimator->AddAnimClip(clip);
	}

	// 노드생성
	CreateAnimNode();
	CreateParams();
	CreateTrans();
}

void MMMEngine::PlayerAnimController::Update()
{
	mAnimator->SetSpeed(mAnimSpeed);
	mAnimController->SetFloat("MoveSpeed", mMoveSpeed);
	//mAnimController->SetBool("Scoop", mAttacking);

	if (mAttacking) {
		mCurrStat = PSTAT::ATTACK;
	}
	else if (mScooping) {
		mCurrStat = PSTAT::SCOOP;
	}
	else {
		mCurrStat = PSTAT::IDLE;
	}

	static PSTAT prevStat = PSTAT::END;
	if (prevStat != mCurrStat) {
		prevStat = mCurrStat;

		//int scoopIdx = mAnimator->GetBoneIdx("Armature.001");	// unordered_map::find 할때 문자열이 깨짐. 왜인지는 모르겠음
		int scoopIdx = 3;

		switch (prevStat)
		{
		case PSTAT::ATTACK:
			mAnimator->PlayBlendClip("Player_Attack", 1.0f, true, scoopIdx);
			mAnimator->PlayBlendClip("Player_Scoop_Idle", 0.0f, true, scoopIdx);
			break;
		case PSTAT::SCOOP:
			mAnimator->PlayBlendClip("Player_Attack", 0.0f, true, scoopIdx);
			mAnimator->PlayBlendClip("Player_Scoop_Idle", 1.0f, true, scoopIdx);
			break;
		default:
			mAnimator->PlayBlendClip("Player_Attack", 0.0f, true, scoopIdx);
			mAnimator->PlayBlendClip("Player_Scoop_Idle", 0.0f, true, scoopIdx);
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
	mMoveSpeed = _speed;
}

void MMMEngine::PlayerAnimController::SetAttack(bool _isAttacking)
{
	mAttacking = _isAttacking;
}
