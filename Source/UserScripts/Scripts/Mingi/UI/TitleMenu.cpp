#include "Export.h"
#include "ScriptBehaviour.h"
#include "TitleMenu.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "Image.h"
#include "Transform.h"
//#include "../../Dongho/Manager/SnowballManager.h"

using namespace MMMEngine;


ObjPtr<Image> TitleMenu::GetButtonFromIdx(int idx)
{
	switch (idx)
	{
	case 0:
		return playButton;
	case 1:
		return creditButton;
	case 2:
		return exitButton;
	default:
		return nullptr;
	}
}

int MMMEngine::TitleMenu::GetIdxFromButtonString(std::string name)
{
	if (name == "Play")
		return 0;
	else if (name == "Credit")
		return 1;
	else if (name == "Exit")
		return 2;

	return -1;
}

void MMMEngine::TitleMenu::MenuControl()
{
	/*if (!SnowballManager::instance.IsValid())
		auto s = SnowballManager::instance->castlepos;*/

	// 메뉴 이동
	int lastSelected = -1;
	if (Input::GetKeyDown(KeyCode::DownArrow))
	{
		lastSelected = m_selected;
		m_selected += 1;
	}
	else if (Input::GetKeyDown(KeyCode::UpArrow))
	{
		lastSelected = m_selected;
		m_selected -= 1;

		if (m_selected < 0)
			m_selected = 2;
	}
	m_selected %= 3;

	ObjPtr<Image> selectedImage = GetButtonFromIdx(m_selected);
	ObjPtr<Image> prevImage = nullptr;

	if (lastSelected != -1)
	{
		// 선택 애니메이션 실행
		m_internalTimer[0] = 0.0f;
		prevImage = GetButtonFromIdx(lastSelected);
		selectedImage->SetRenderOrder(1);
	}

	if (prevImage.IsValid())
	{
		prevImage->SetColor({ 0.5f,0.5f,0.5f,1.0f });
		prevImage->GetTransform()->SetWorldScale({ 1,1,1 });
		prevImage->SetRenderOrder(0);
	}

	// 메뉴 선택
	if (Input::GetKeyDown(KeyCode::Enter))
	{
		switch (m_selected)
		{
		case 1:
			m_internalTimer[2] = 0.0f;
			m_popupstate = 1;
			m_popupOn = true;
			break;
		}

		m_internalTimer[1] = 0.0f;
	}
}

void MMMEngine::TitleMenu::PopupAnimation()
{
	if (m_popupstate != 0)
		m_internalTimer[2] += Time::GetDeltaTime();

	switch (m_popupstate)
	{
	case 1:
	{
		creditPopup->SetColor({ 1,1,1,(m_internalTimer[2] / popupFadeDuration) });
		creditPopup->GetTransform()->SetWorldScale({ PopUpScaleXY.Evaluate(m_internalTimer[2]),PopUpScaleXY.Evaluate(m_internalTimer[2]),0.0f });
	
		if (m_internalTimer[2] > PopUpScaleXY.GetKeyframes().back().time)
			m_popupstate = 0;
	}
		break;
	case 2:
	{
		creditPopup->SetColor({ 1,1,1,1 - (m_internalTimer[2] / popupFadeDuration) });
		creditPopup->GetTransform()->SetWorldScale({ PopUpOffScaleXY.Evaluate(m_internalTimer[2]),PopUpOffScaleXY.Evaluate(m_internalTimer[2]),0.0f });

		if (m_internalTimer[2] > PopUpOffScaleXY.GetKeyframes().back().time)
		{
			m_internalTimer[2] = 0.0f;
			m_popupstate = 0;
		}
	}
		break;
	default:
		break;
	}
}

void MMMEngine::TitleMenu::MenuAnimation()
{
	auto selectedImage = GetButtonFromIdx(m_selected);
	if (selectedImage.IsValid())
	{
		m_internalTimer[0] += Time::GetDeltaTime();
		m_internalTimer[1] += Time::GetDeltaTime();

		float buttonScaleX = ButtonOnScaleX.Evaluate(m_internalTimer[0]) + ButtonExecuteScaleX.Evaluate(m_internalTimer[1]);
		float buttonScaleY = ButtonOnScaleY.Evaluate(m_internalTimer[0]) + ButtonExecuteScaleY.Evaluate(m_internalTimer[1]);
		float buttonRotZ = ButtonExecuteRotZ.Evaluate(m_internalTimer[1]);

		selectedImage->GetTransform()->SetWorldScale({ buttonScaleX, buttonScaleY,0.0f });
		selectedImage->GetTransform()->SetWorldEulerRotation({ 0,0,buttonRotZ });
		selectedImage->SetColor({ 1,1,1,1 });
	}
}

void MMMEngine::TitleMenu::Start()
{

}

void MMMEngine::TitleMenu::Update()
{
	if (!IsControllAble)
		return;

	if (m_popupOn)
	{
		if (m_popupstate == 0 &&
			(Input::GetKeyDown(KeyCode::Enter) ||
				Input::GetKeyDown(KeyCode::Escape)))
		{
			m_internalTimer[2] = 0.0f;
			m_popupstate = 2;
		}

		if (m_popupstate == 2
			&& m_internalTimer[2] > popupFadeDuration)
		{
			m_popupOn = false;
		}
	}
	else
	{
		MenuControl();
	}

	MenuAnimation();
	PopupAnimation();
}

void MMMEngine::TitleMenu::TurnOffAllButton()
{
	m_internalTimer[1] = 300.0f;
	playButton->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	playButton->SetRenderOrder(0);
	creditButton->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	creditButton->SetRenderOrder(0);
	exitButton->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	exitButton->SetRenderOrder(0);
}

void MMMEngine::TitleMenu::TurnOnPlayButton()
{
	m_internalTimer[1] = 300.0f;
	playButton->SetColor({ 1,1,1,1 });
	playButton->SetRenderOrder(1);
	m_internalTimer[0] = 0.0f;
}

void MMMEngine::TitleMenu::TurnOnOneButton(std::string buttonName)
{
	int id = GetIdxFromButtonString(buttonName);

	if (id == -1)
		return;
	auto button = GetButtonFromIdx(id);
	if (button.IsValid())
	{
		button->SetRenderOrder(1);
		button->SetColor({ 1,1,1,1 });
	}

	m_internalTimer[0] = 0.0f;
}
