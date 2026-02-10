#include "Export.h"
#include "ScriptBehaviour.h"
#include "Testpooling.h"
#include "MMMInput.h"
#include "EnemyController.h"
#include "Transform.h"

void MMMEngine::Testpooling::Start()
{

}

void MMMEngine::Testpooling::Update()
{
	if (Input::GetKeyDown(KeyCode::L))
	{
		auto t = e1->GetComponent<EnemyController>();
		t->InitEnemy(EnemyController::EnemyType::Warrior, GetTransform()->GetWorldPosition(), 30.f);
	}

	if (Input::GetKeyDown(KeyCode::K))
	{
		auto t = e2->GetComponent<EnemyController>();
		t->InitEnemy(EnemyController::EnemyType::Archer, GetTransform()->GetWorldPosition(), 30.f);
	}

	if (Input::GetKeyDown(KeyCode::J))
	{
		auto t = e3->GetComponent<EnemyController>();
		t->InitEnemy(EnemyController::EnemyType::Scout, GetTransform()->GetWorldPosition(), 30.f);
	}
}
