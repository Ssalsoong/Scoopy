#include "Export.h"
#include "ScriptBehaviour.h"
#include "PrefabTest.h"
#include "../Mingi/ExplosionPool.h"
#include "MMMInput.h"

void MMMEngine::PrefabTest::Start()
{
}

void MMMEngine::PrefabTest::Update()
{
	if (Input::GetKeyDown(KeyCode::G))
	{
		/*if (ExplosionPool::Instance.IsValid())
			ExplosionPool::Instance->Spawn({ 1.0f,2.0f,3.0f });*/
		Instantiate(Teees);
	}
}
