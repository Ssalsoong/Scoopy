#include "Export.h"
#include "ScriptBehaviour.h"
#include "FadeInOutFX.h"
#include "MMMTime.h"
#include "Image.h"

using namespace MMMEngine;

ObjPtr<FadeInOutFX> FadeInOutFX::Instance = nullptr;

int MMMEngine::FadeInOutFX::GetState()
{
	return m_fadeState;
}

void MMMEngine::FadeInOutFX::Awake()
{
	if (!Instance.IsValid())
	{
		Instance = SelfPtr(this);
		DontDestroyOnLoad(SelfPtr(this));
	}
	else
	{
		GetGameObject()->Destroy(GetGameObject());
	}
}

void MMMEngine::FadeInOutFX::Update()
{
	if (m_fadeState == 0)
		return;

	if (m_fadeState == 1)
	{
		m_internalTime -= Time::GetDeltaTime();
		if (m_internalTime < 0)
		{
			m_fadeState = 0;
			m_internalTime = 0.0f;
		}
	}
	if (m_fadeState == 2)
	{
		m_internalTime += Time::GetDeltaTime();
		if (m_internalTime >= FadeDuration)
		{
			m_internalTime = FadeDuration; // clamp
			if (!FadeCurve.IsEmpty() && FadeImage.IsValid())
				FadeImage->SetAlpha(FadeCurve.Evaluate(1.0f)); // 마지막 프레임 확정
			 
			m_fadeState = 0;
		}
	}

	if(m_fadeState == 0 ||
		FadeCurve.IsEmpty() ||
		!FadeImage.IsValid())
		return;

	FadeImage->SetAlpha(FadeCurve.Evaluate(m_internalTime/FadeDuration));
}

void MMMEngine::FadeInOutFX::FadeIn()
{
	m_fadeState = 1;
	if (m_internalTime == 0.0f)
		m_internalTime = FadeDuration;
}

void MMMEngine::FadeInOutFX::FadeOut()
{
	m_fadeState = 2;
}
