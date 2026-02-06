#include "Export.h"
#include "ScriptBehaviour.h"
#include "AnimResourceManager.h"
#include "AnimationClip.h"


MMMEngine::ObjPtr<MMMEngine::AnimResourceManager> MMMEngine::AnimResourceManager::instance;

void MMMEngine::AnimResourceManager::Awake()
{
	if(!instance)
		instance = SelfPtr(this);

	// 플레이어 클립
	mAnimClips[AnimType::AT_Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Idle_0.animclip"));
	mAnimClips[AnimType::AT_Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Move_0.animclip"));
	mAnimClips[AnimType::AT_Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Attack_0.animclip"));
	mAnimClips[AnimType::AT_Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Idle_0.animclip"));
	mAnimClips[AnimType::AT_Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Move Attack_0.animclip"));
	mAnimClips[AnimType::AT_Player].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Player/Anims/Player_Scoop_Move_0.animclip"));

	// 적 클립
	mAnimClips[AnimType::AT_Archer].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Archer/Monster_Archer_Attack_0.animclip"));
	mAnimClips[AnimType::AT_Archer].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Archer/Monster_Archer_Move_0.animclip"));
	mAnimClips[AnimType::AT_Warrior].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Warrior/Monster_Warrior_Attack_0.animclip"));
	mAnimClips[AnimType::AT_Warrior].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Warrior/Monster_Warrior_Move_0.animclip"));
	mAnimClips[AnimType::AT_Scout].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Scout/Monster_Scout_Attack_0.animclip"));
	mAnimClips[AnimType::AT_Scout].push_back(ResourceManager::Get()
		.Load<AnimationClip>(L"Assets/Sunken/Enemy/Anim/Scout/Monster_Scout_Move_0.animclip"));

	animSize = (int)mAnimClips[AnimType::AT_Archer].size();
}

void MMMEngine::AnimResourceManager::Start()
{
	
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
		return &(it->second);
	}

	std::cerr << "GetAnimClips Failed" << std::endl;
	return nullptr;
}
