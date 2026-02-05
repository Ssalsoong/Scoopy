#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class RectTransform;
    class Transform;
    class USERSCRIPTS MiniMap : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        MiniMap()
        {
        REGISTER_BEHAVIOUR_MESSAGE(LateUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void LateUpdate();

        USCRIPT_PROPERTY()
        float MinimapUnitScale = 10.0f;

        USCRIPT_PROPERTY()
        ObjPtr<Transform> Player;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> PlayerRectUI;
    };
}
