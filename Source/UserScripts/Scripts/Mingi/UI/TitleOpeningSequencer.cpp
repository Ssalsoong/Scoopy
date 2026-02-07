#include "Export.h"
#include "ScriptBehaviour.h"
#include "TitleOpeningSequencer.h"
#include "../../Mingi/UI/FadeInOutFX.h"
#include "MMMTime.h"
#include "Image.h"
#include "Transform.h"
#include "../../Mingi/UI/TitleMenu.h"

void MMMEngine::TitleOpeningSequencer::Start()
{
	m_internalTime = 0.0f;
	m_state = 0;

	m_playBY = PlayImage->GetTransform()->GetWorldPosition().y;
	m_creditBY = CreditImage->GetTransform()->GetWorldPosition().y;
	m_exitBY = ExitImage->GetTransform()->GetWorldPosition().y;
}

void MMMEngine::TitleOpeningSequencer::Update()
{
	if (!FadeInOutFX::Instance.IsValid())
		return;
	// 15프레임 대기
	if (m_state < 15)
	{
		m_state++;
		return;
	}
	m_internalTime += Time::GetDeltaTime();

	switch (m_state)
	{
	case 15:
		if (m_internalTime > 0.55f)
		{
			m_state++;
			m_internalTime = 0.0f;
			FadeInOutFX::Instance->FadeIn();
		}
		break;
	case 16:
		if (m_internalTime > titleFadeStartTime)
		{
			m_state++;
			m_internalTime = 0.0f;
		}
		break;
	case 17:
	{
		TitleImage->SetAlpha(m_internalTime / titleLoadFadeTime);

		float t = TitleRotate.Evaluate(m_internalTime);

		TitleImage->GetTransform()->SetWorldEulerRotation({ 0,0,TitleRotate.Evaluate(m_internalTime) });
		TitleImage->GetTransform()->SetWorldScale({ TitleScaleX.Evaluate(m_internalTime),TitleScaleY.Evaluate(m_internalTime),0 });

		float maxTime = TitleRotate.GetKeyframes().back().time > titleLoadFadeTime ? TitleRotate.GetKeyframes().back().time : titleLoadFadeTime;

		//maxTime = maxTime > TitleScaleX.GetKeyframes().back().time

		if (m_internalTime > maxTime)
		{
			m_state++;
			m_internalTime = 0.0f;
		}
	}

		break;
	case 18:
		if (m_internalTime > 0.18f)
		{
			m_state++;
			m_internalTime = 0.0f;
		}
		break;
	case 19:

		PlayImage->SetAlpha(m_internalTime / buttonLoadFadeTime);
		CreditImage->SetAlpha((-buttonAnimOffsetTime + m_internalTime) / buttonLoadFadeTime);
		ExitImage->SetAlpha((-buttonAnimOffsetTime * 2 + m_internalTime) / buttonLoadFadeTime);

		auto ownX = PlayImage->GetTransform()->GetLocalPosition().x;
		auto ownZ = PlayImage->GetTransform()->GetLocalPosition().z;
		float diffY = (buttonAnimYDiff * (m_internalTime / buttonLoadFadeTime));
		diffY = buttonAnimYDiff < diffY ? buttonAnimYDiff : diffY;
		PlayImage->GetTransform()->SetLocalPosition({ ownX , m_playBY - diffY,ownZ });

		ownX = CreditImage->GetTransform()->GetLocalPosition().x;
		ownZ = CreditImage->GetTransform()->GetLocalPosition().z;
		diffY = (buttonAnimYDiff * ((-buttonAnimOffsetTime + m_internalTime) / buttonLoadFadeTime));
		diffY = buttonAnimYDiff < diffY ? buttonAnimYDiff : diffY;
		CreditImage->GetTransform()->SetLocalPosition({ ownX , m_creditBY - diffY,ownZ });
		
		ownX = ExitImage->GetTransform()->GetLocalPosition().x;
		ownZ = ExitImage->GetTransform()->GetLocalPosition().z;
		diffY = (buttonAnimYDiff * ((-buttonAnimOffsetTime * 2 + m_internalTime) / buttonLoadFadeTime));
		diffY = buttonAnimYDiff < diffY ? buttonAnimYDiff : diffY;
		ExitImage->GetTransform()->SetLocalPosition({ ownX , m_exitBY - diffY ,ownZ });

		PlayImage->GetTransform()->SetWorldScale({ ButtonScaleX.Evaluate(m_internalTime),ButtonScaleY.Evaluate(m_internalTime),0 });
		CreditImage->GetTransform()->SetWorldScale({ ButtonScaleX.Evaluate(-buttonAnimOffsetTime + m_internalTime),ButtonScaleY.Evaluate(-buttonAnimOffsetTime + m_internalTime),0 });
		ExitImage->GetTransform()->SetWorldScale({ ButtonScaleX.Evaluate(-buttonAnimOffsetTime * 2 + m_internalTime),ButtonScaleY.Evaluate(-buttonAnimOffsetTime * 2 + m_internalTime),0 });

		auto backMaxX = ButtonScaleX.GetKeyframes().back().time;
		auto backMaxY = ButtonScaleY.GetKeyframes().back().time;

		auto backMax = backMaxX > backMaxY ? backMaxX : backMaxY;
		backMax += buttonLoadFadeTime + (buttonAnimOffsetTime * 2);
		if (m_internalTime > backMax)
		{
			m_state++;
			m_internalTime = 0.0f;
			TitleMenu->TurnOffAllButton();
			TitleMenu->TurnOnPlayButton();
			TitleMenu->IsControllAble = true;
		}
		break;
	}
}
