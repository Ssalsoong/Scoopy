#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine { class AnimationClip; }

namespace MMMEngine
{
	enum AnimType : int {
		AT_Player = 0,
		AT_Archer,
		AT_Warrior,
		AT_Scout
	};
	class USERSCRIPTS AnimResourceManager : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND

			// AnimType, vec<AnimClip>
			std::unordered_map<AnimType, std::vector<ResPtr<AnimationClip>>> mAnimClips;

	public:
		AnimResourceManager()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

		USCRIPT_PROPERTY()
		int animSize = 0;

		USCRIPT_MESSAGE()
			void Awake();

		USCRIPT_MESSAGE()
			void Start();

		USCRIPT_MESSAGE()
			void Update();

		const ResPtr<AnimationClip> GetAnimClip(AnimType _type, std::string _name);
		const std::vector<ResPtr<AnimationClip>>* GetAnimClips(AnimType _type);
	};
}
