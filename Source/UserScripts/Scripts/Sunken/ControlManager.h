#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

#include "CoreComponents.h"
#include "InputKeyCode.h"

namespace MMMEngine
{
    class USERSCRIPTS ControlManager : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        // idx = KeyCode, int = Layer 
        std::array<int, 256> mControlArray;

        // int = Layer, set = KeyCodes
        std::unordered_map<int, std::unordered_set<KeyCode>> mControlMap;

        std::set<int> mInputLayers;

        int minLayer = -1;

        static ObjPtr<ControlManager> instance;
    public:
        ControlManager()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(LateUpdate);

        }

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void LateUpdate();

        bool GetKeyDown(KeyCode _keycode, int _layer);
        bool GetKeyUp(KeyCode _keycode, int _layer);
        bool GetKey(KeyCode _keycode, int _layer);

        void SetMinLayer(int _layer);
        void ReleaseMinLayer();
        int GetMinLayer() { return minLayer; }

        static const ObjPtr<ControlManager> Get() { return instance; }
    };
}
