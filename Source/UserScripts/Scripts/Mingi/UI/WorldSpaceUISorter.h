#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <vector>

namespace MMMEngine
{
    class WorldSpaceUI;
    class USERSCRIPTS WorldSpaceUISorter : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND

        // UI들이 자신을 등록/해제할 리스트
        std::vector<ObjPtr<WorldSpaceUI>> m_uiElements;

    public:
        WorldSpaceUISorter()
        {
        SetExecutionOrder(-10);
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(LateUpdate);

        }

        void RegisterUI(ObjPtr<WorldSpaceUI> ui);
        void UnregisterUI(ObjPtr<WorldSpaceUI> ui);

        USCRIPT_PROPERTY()
        int GlobalOrder = -1000;

		static ObjPtr<WorldSpaceUISorter> Instance;

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void LateUpdate();
    };
}
