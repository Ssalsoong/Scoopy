#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "CoreComponents.h"

namespace MMMEngine
{
    class PlayerController;
    class Player;
    class USERSCRIPTS ScoopGageScript : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            ObjPtr<Player> mPlayer;
            ObjPtr<PlayerController> mPlayerController;

            float m_showingGage = 0.0f;

            USCRIPT_PROPERTY()
                ObjPtr<Gage> mGage;

			USCRIPT_PROPERTY()
				ObjPtr<Image> mActiveImg;
			USCRIPT_PROPERTY()
				ObjPtr<Image> mDeactiveImg;
    public:
        ScoopGageScript()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();
    };
}
