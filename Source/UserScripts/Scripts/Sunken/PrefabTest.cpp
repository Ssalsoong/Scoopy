#include "Export.h"
#include "ScriptBehaviour.h"
#include "PrefabTest.h"

void MMMEngine::PrefabTest::Start()
{
	std::cout << "Start!!" << std::endl;
	Destroy(SelfPtr(this));
}

void MMMEngine::PrefabTest::Update()
{
}
