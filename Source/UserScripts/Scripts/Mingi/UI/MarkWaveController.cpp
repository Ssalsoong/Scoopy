#include "Export.h"
#include "ScriptBehaviour.h"
#include "MarkWaveController.h"
#include "WorldSpaceUI.h"
#include "MarkUIInfo.h"
#include "Text.h"

using namespace MMMEngine;

ObjPtr<MarkWaveController> MarkWaveController::Instance = nullptr;


void MMMEngine::MarkWaveController::OnDestroy()
{
	if (Instance == SelfPtr(this))
		Instance = nullptr;
}

void MMMEngine::MarkWaveController::Awake()
{
	if (!Instance.IsValid())
	{
		Instance = SelfPtr(this);
	}
	else
	{
		GetGameObject()->Destroy(GetGameObject());
	}
}

void MMMEngine::MarkWaveController::Update()
{
}

void MMMEngine::MarkWaveController::SetWave(int wave)
{
	switch (wave)
	{
	case 1:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num2_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("5");
		break;
	case 2:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num4_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("10");
		break;
	case 3:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num6_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("10");

		Ranger1->GetGameObject()->SetActive(true);
		Ranger1->worldSpaceUI->TargetTransform = num2_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger1->countText->SetTextUtf8("10");
		break;
	case 4:
		Assassin1->GetGameObject()->SetActive(true);
		Assassin1->worldSpaceUI->TargetTransform = num4_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Assassin1->countText->SetTextUtf8("10");
		break;
	case 5:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num8_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("10");

		Ranger1->GetGameObject()->SetActive(true);
		Ranger1->worldSpaceUI->TargetTransform = num9_UP_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger1->countText->SetTextUtf8("10");
		break;
	case 6:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num6_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("15");

		Assassin1->GetGameObject()->SetActive(true);
		Assassin1->worldSpaceUI->TargetTransform = num4_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Assassin1->countText->SetTextUtf8("10");
		break;

	case 7:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num2_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("15");

		Ranger1->GetGameObject()->SetActive(true);
		Ranger1->worldSpaceUI->TargetTransform = num1_L_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger1->countText->SetTextUtf8("10");

		Ranger2->GetGameObject()->SetActive(true);
		Ranger2->worldSpaceUI->TargetTransform = num3_R_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger2->countText->SetTextUtf8("10");
		break;
	case 8:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num6_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("10");

		Ranger1->GetGameObject()->SetActive(true);
		Ranger1->worldSpaceUI->TargetTransform = num3_R_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger1->countText->SetTextUtf8("10");

		Ranger2->GetGameObject()->SetActive(true);
		Ranger2->worldSpaceUI->TargetTransform = num1_L_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger2->countText->SetTextUtf8("10");

		Assassin1->GetGameObject()->SetActive(true);
		Assassin1->worldSpaceUI->TargetTransform = num4_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Assassin1->countText->SetTextUtf8("10");
		break;

	case 9:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num8_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("15");

		Warrior2->GetGameObject()->SetActive(true);
		Warrior2->worldSpaceUI->TargetTransform = num6_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior2->countText->SetTextUtf8("15");

		Ranger1->GetGameObject()->SetActive(true);
		Ranger1->worldSpaceUI->TargetTransform = num9_R_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger1->countText->SetTextUtf8("10");

		Assassin1->GetGameObject()->SetActive(true);
		Assassin1->worldSpaceUI->TargetTransform = num2_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Assassin1->countText->SetTextUtf8("10");
		break;

	case 10:
		Warrior1->GetGameObject()->SetActive(true);
		Warrior1->worldSpaceUI->TargetTransform = num8_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior1->countText->SetTextUtf8("20");

		Warrior2->GetGameObject()->SetActive(true);
		Warrior2->worldSpaceUI->TargetTransform = num4_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Warrior2->countText->SetTextUtf8("20");

		Ranger1->GetGameObject()->SetActive(true);
		Ranger1->worldSpaceUI->TargetTransform = num7_UP_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger1->countText->SetTextUtf8("10");

		Ranger2->GetGameObject()->SetActive(true);
		Ranger2->worldSpaceUI->TargetTransform = num6_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Ranger2->countText->SetTextUtf8("10");

		Assassin1->GetGameObject()->SetActive(true);
		Assassin1->worldSpaceUI->TargetTransform = num2_TR;
		Warrior1->worldSpaceUI->LateUpdate();
		Assassin1->countText->SetTextUtf8("10");
		break;
	}
}

void MMMEngine::MarkWaveController::AllOff()
{
	Warrior1->worldSpaceUI->TargetTransform = nullptr;
	Warrior2->worldSpaceUI->TargetTransform = nullptr;
	Warrior3->worldSpaceUI->TargetTransform = nullptr;

	Warrior1->GetGameObject()->SetActive(false);
	Warrior2->GetGameObject()->SetActive(false);
	Warrior3->GetGameObject()->SetActive(false);

	Assassin1->worldSpaceUI->TargetTransform = nullptr;
	Assassin2->worldSpaceUI->TargetTransform = nullptr;
	Assassin3->worldSpaceUI->TargetTransform = nullptr;

	Assassin1->GetGameObject()->SetActive(false);
	Assassin2->GetGameObject()->SetActive(false);
	Assassin3->GetGameObject()->SetActive(false);

	Ranger1->worldSpaceUI->TargetTransform = nullptr;
	Ranger2->worldSpaceUI->TargetTransform = nullptr;
	Ranger3->worldSpaceUI->TargetTransform = nullptr;

	Ranger1->GetGameObject()->SetActive(false);
	Ranger2->GetGameObject()->SetActive(false);
	Ranger3->GetGameObject()->SetActive(false);
}
