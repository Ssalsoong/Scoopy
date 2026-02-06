#include "Export.h"
#include "ScriptBehaviour.h"
#include "TitleMenu.h"
#include "MMMInput.h"
#include "MMMTime.h"
#include "Image.h"
#include "Transform.h"

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
	}
}

void MMMEngine::TitleMenu::Start()
{

}

void MMMEngine::TitleMenu::Update()
{
	if (!IsControllAble)
		return;

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
		m_internalTimer[lastSelected] = 0.0f;
		m_internalTimer[m_selected] = 0.0f;
		prevImage = GetButtonFromIdx(lastSelected);
	}

	if (prevImage.IsValid())
	{
		prevImage->SetColor({ 0.5f,0.5f,0.5f,1.0f });
		prevImage->GetTransform()->SetWorldScale({ 1,1,1 });
	}


	m_internalTimer[m_selected] += Time::GetDeltaTime();
	if (selectedImage.IsValid())
	{
		selectedImage->GetTransform()->SetWorldScale({ButtonOnScaleX.Evaluate(m_internalTimer[m_selected]),ButtonOnScaleY.Evaluate(m_internalTimer[m_selected]),0.0f});
		selectedImage->SetColor({ 1,1,1,1 });
	}

}

void MMMEngine::TitleMenu::TurnOffAllButton()
{
	playButton->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	creditButton->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	exitButton->SetColor({ 0.5f,0.5f,0.5f,1.0f });
}

void MMMEngine::TitleMenu::TurnOnPlayButton()
{
	playButton->SetColor({ 1,1,1,1 });
	m_internalTimer[0] = 0.0f;
}
