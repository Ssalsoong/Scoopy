#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyGageController.h"
#include "Gage.h"
#include "../../Dongho/Manager/EnemySpawner.h"
#include "../../Dongho/Battlestats.h"

void MMMEngine::EnemyGageController::OnEnable()
{
	if (hpUI.IsValid())
	{
		if (auto go = hpUI->GetGameObject(); go.IsValid())
			go->SetActive(true);
	}
}
void MMMEngine::EnemyGageController::OnDisable()
{
	if (hpUI.IsValid())
	{
		if (auto go = hpUI->GetGameObject(); go.IsValid())
			go->SetActive(false);
	}
}

void MMMEngine::EnemyGageController::Update()
{
	if (EnemySpawner::instance.IsValid())
	{
		switch (type)
		{
		case EnemyType::Warrior:
			if (hpUI.IsValid() && battlestats.IsValid() && EnemySpawner::instance.IsValid())
				hpUI->SetValue((float)battlestats->GetHP() / (float)EnemySpawner::instance->NormalHP);
			break;
		case EnemyType::Ranger:
			if (hpUI.IsValid() && battlestats.IsValid() && EnemySpawner::instance.IsValid())
				hpUI->SetValue((float)battlestats->GetHP() / (float)EnemySpawner::instance->ArrowHP);
			break;
		case EnemyType::Assassin:
			if (hpUI.IsValid() && battlestats.IsValid() && EnemySpawner::instance.IsValid())
				hpUI->SetValue((float)battlestats->GetHP() / (float)EnemySpawner::instance->ThiefHP);
			break;
		}
	}
}
