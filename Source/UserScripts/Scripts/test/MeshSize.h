#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
	class USERSCRIPTS MeshSize : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND
	public:
		MeshSize()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

		USCRIPT_MESSAGE()
		void Start();

		ObjPtr<Transform> MeshTrans;

		void SetMatrixMesh(float size);
	};
}
