#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

#include "CoreComponents.h"
#include "InputManager.h"

namespace MMMEngine
{
    class USERSCRIPTS ControlManager : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        // idx = KeyCode, int = Layer 
        std::array<int, 256> mControlArray;

        // int = Layer, vec = KeyCodes
        std::unordered_map<int, std::vector<KeyCode>> mControlMap;

        static ObjPtr<ControlManager> instance;

        void RemoveKey(std::vector<KeyCode>& _keycodes, int _idx);
    public:
        ControlManager()
        {
        }

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void LateUpdate();

        bool GetKeyDown(KeyCode _keycode, int _layer);
        bool GetKeyUp(KeyCode _keycode, int _layer);
        bool GetKey(KeyCode _keycode, int _layer);

        static const ObjPtr<ControlManager> Get() { return instance; }
    };
}
