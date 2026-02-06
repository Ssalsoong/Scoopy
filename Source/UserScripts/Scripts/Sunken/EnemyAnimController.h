#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine { class Animator; }

namespace MMMEngine { class AnimResourceManager; }

namespace MMMEngine
{
	class USERSCRIPTS EnemyAnimController : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND

			float animSpeed = 1.0f;
	public:
		EnemyAnimController()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

		USCRIPT_PROPERTY()
			ObjPtr<AnimResourceManager> mAnimManager;
		USCRIPT_PROPERTY()
			ObjPtr<Animator> mAnimator;

		USCRIPT_PROPERTY()
			int AnimSize = 0;

		USCRIPT_MESSAGE()
			void Start();

		USCRIPT_MESSAGE()
			void Update();

	};
}
