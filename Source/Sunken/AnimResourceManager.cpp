#include "Export.h"
#include "ScriptBehaviour.h"
#include "AnimResourceManager.h"
#include "AnimationClip.h"

void MMMEngine::AnimResourceManager::Start()
{
	// 플레이어 클립
	mAnimClips[AnimType::Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Idle_0.animclip"));
	mAnimClips[AnimType::Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Move_0.animclip"));
	mAnimClips[AnimType::Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Attack_0.animclip"));
	mAnimClips[AnimType::Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Idle_0.animclip"));
	mAnimClips[AnimType::Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Move Attack_0.animclip"));
	mAnimClips[AnimType::Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Move_0.animclip"));

	// 적 클립
	mAnimClips[AnimType::Archer].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Archer/Monster_Archer_Attack_0.animclip"));
	mAnimClips[AnimType::Archer].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Archer/Monster_Archer_Move_0.animclip"));
	mAnimClips[AnimType::Warrior].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Archer/Monster_Warrior_Attack_0.animclip"));
	mAnimClips[AnimType::Warrior].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Archer/Monster_Warrior_Move_0.animclip"));
	mAnimClips[AnimType::Scout].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Scout/Monster_Scout_Attack_0.animclip"));
	mAnimClips[AnimType::Scout].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Scout/Monster_Scout_Move_0.animclip"));
}

void MMMEngine::AnimResourceManager::Update()
{

}

const MMMEngine::ResPtr<MMMEngine::AnimationClip> MMMEngine::AnimResourceManager::GetAnimClip(AnimType _type, std::string _name)
{
	auto it = mAnimClips.find(_type);
	if (it != mAnimClips.end()) {
		auto& clips = it->second;
		auto aIt = std::find_if(clips.begin(), clips.end(), [&](ResPtr<AnimationClip>& a) {
			return a->mName == _name;
			});
		if (aIt != clips.end()) {
			return *aIt;
		}
	}
	return nullptr;
}

const std::vector<MMMEngine::ResPtr<MMMEngine::AnimationClip>>* MMMEngine::AnimResourceManager::GetAnimClips(AnimType _type)
{
	auto it = mAnimClips.find(_type);
	if (it != mAnimClips.end()) {
		return &it->second;
	}

	return nullptr;
}
