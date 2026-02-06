#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerAnimController.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "InputManager.h"

void MMMEngine::PlayerAnimController::Start()
{
	auto anim = GetComponent<Animator>();

	if (anim) {
		mAnimator = anim;
		// IDLE
		anim->AddAnimClip(ResourceManager::Get()
			.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Idle_0.animclip"));
		// MOVE
		anim->AddAnimClip(ResourceManager::Get()
			.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Move_0.animclip"));
		// ATTACK
		anim->AddAnimClip(ResourceManager::Get()
			.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Attack_0.animclip"));
		// SCOOP_IDLE
		anim->AddAnimClip(ResourceManager::Get()
			.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Idle_0.animclip"));
		// SCOOP_MOVE
		anim->AddAnimClip(ResourceManager::Get()
			.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Move Attack_0.animclip"));
		// SCOOP_MOVE_ATTACK
		anim->AddAnimClip(ResourceManager::Get()
			.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Move_0.animclip"));

		animCount = static_cast<int>(mAnimator->GetAnimClips().size());
	}
}

void MMMEngine::PlayerAnimController::Update()
{
	/*if (InputManager::Get().GetKeyDown(KeyCode::Alpha1))
		mAnimator->PlayClip("Player_Idle", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha2))
		mAnimator->PlayClip("Player_Move", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha3))
		mAnimator->PlayClip("Player_Attack", false);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha4))
		mAnimator->PlayClip("Player_Scoop_Idle", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha5))
		mAnimator->PlayClip("Player_Scoop_Move", true);
	else if (InputManager::Get().GetKeyDown(KeyCode::Alpha6))
		mAnimator->PlayClip("Player_Scoop_Attack", false);*/
}

void MMMEngine::PlayerAnimController::PlayAnimation(PlayerStatus _status)
{
	switch (_status)
	{
	case MMMEngine::P_IDLE:
		mAnimator->PlayClip("Player_Idle", true);
		break;
	case MMMEngine::P_MOVE:
		mAnimator->PlayClip("Player_Move", true);
		break;
	case MMMEngine::P_ATTACK:
	{
		mAnimator->PlayClip("Player_Attack", false);
		break;
	}
	case MMMEngine::P_SCOOPIDLE:
		mAnimator->PlayClip("Player_Scoop_Idle", true);
		break;
	case MMMEngine::P_SCOOPMOVE:
		mAnimator->PlayClip("Player_Scoop_Move", true);
		break;
	case MMMEngine::P_SCOOPATTACK:
		mAnimator->PlayClip("Player_Scoop_Attack", false);
		break;
	case MMMEngine::P_END:
		mAnimator->PlayClip("Player_Idle", true);
		break;
	default:
		mAnimator->PlayClip("Player_Idle", true);
		break;
	}
}
