#include "Export.h"
#include "ScriptBehaviour.h"
#include "TimerUI.h"
#include "RectTransform.h"
#include "MMMTime.h"
#include "Image.h"
#include "Gage.h"
#include "Text.h"

void MMMEngine::TimerUI::Start()
{
	m_rect = GetComponent<RectTransform>();
	if (m_rect.IsValid())
	{
		m_initPosY = m_rect->GetAnchoredPosition().y;
		m_initPosX = m_rect->GetAnchoredPosition().x;
	}

	if (WaveBack.IsValid())
	{
		WaveBack->SetAlpha(0.0f);
	}

	if (WaveText.IsValid())
	{
		std::string wave = "-/" + std::to_string(m_maxWaveNum);
		WaveText->SetTextUtf8(wave);
	}

	if (TimerGage.IsValid())
	{
		TimerGage->SetColor({1.0f,1.0f,1.0f,1.0f});
	}

	if(!scaleCurve.IsEmpty())
	{
		float curveTime = scaleCurve.GetKeyframes().back().time;
		m_maximumAnimTime = m_maximumAnimTime > curveTime ? m_maximumAnimTime : curveTime;
	}

	if (!rotCurve.IsEmpty())
	{
		float curveTime = rotCurve.GetKeyframes().back().time;
		m_maximumAnimTime = m_maximumAnimTime > curveTime ? m_maximumAnimTime : curveTime;
	}
	if (!posCurve.IsEmpty())
	{
		float curveTime = posCurve.GetKeyframes().back().time;
		m_maximumAnimTime = m_maximumAnimTime > curveTime ? m_maximumAnimTime : curveTime;
	}
}

void MMMEngine::TimerUI::Update()
{
	if (!m_playingAnimation)
	{
		m_animationTime = 0.0f;
		return;
	}

	if (!m_rect.IsValid() || 
		!TimerGage.IsValid() ||
		!WaveBack.IsValid())
		return;


	//USCRIPT_PROPERTY()
	//	ObjPtr<Text> WaveText = nullptr;

	//USCRIPT_PROPERTY()
	//	ObjPtr<Gage> TimerGage = nullptr;


	//USCRIPT_PROPERTY()
	//	ObjPtr<Image> WaveBack = nullptr;

	m_animationTime += Time::GetDeltaTime();

	if (m_isWaveMode)
	{
		switch (m_animState)
		{
		case 0:
			TimerGage->SetColor({ 1,1,1, 1.0f - alphaCurve.Evaluate(m_animationTime) });
			if (m_animationTime > alphaCurve.GetKeyframes().back().time)
			{
				m_animState++;
				m_animationTime = 0.0f;
			}
			break;
		case 1:
			WaveText->SetColor({ 1,1,1,alphaCurve.Evaluate(m_animationTime) });
			WaveBack->SetColor({ 1,1,1,alphaCurve.Evaluate(m_animationTime) });

			m_rect->SetWorldScale({ scaleCurve.Evaluate(m_animationTime),scaleCurve.Evaluate(m_animationTime),1 });
			m_rect->SetAnchoredPosition({ m_initPosX, m_initPosY + posCurve.Evaluate(m_animationTime) });
			m_rect->SetWorldEulerRotation({ 0,0,rotCurve.Evaluate(m_animationTime) });

			if (!m_textChanged && m_animationTime > changeTextTime)
			{
				m_textChanged = true;
				// 새 텍스트 문자열 
				// 최대 웨이브 수치를 넘은 경우는 원래대로 돌아가기
				++m_waveCount;
				std::string wave = std::to_string(m_waveCount) + "/" + std::to_string(m_maxWaveNum);
				WaveText->SetTextUtf8(wave);

				m_waveCount %= m_maxWaveNum;
			}

			if (m_animationTime > m_maximumAnimTime)
			{
				m_textChanged = false;
				m_playingAnimation = false;
				m_animState = 0;
			}
			break;
		}
	}
	else
	{
		switch (m_animState)
		{
		case 0:
			WaveText->SetColor({ 1,1,1, 1.0f - alphaCurve.Evaluate(m_animationTime) });
			WaveBack->SetColor({ 1,1,1, 1.0f - alphaCurve.Evaluate(m_animationTime) });
			if (m_animationTime > alphaCurve.GetKeyframes().back().time)
			{
				m_animState++;
				m_animationTime = 0.0f;
			}
			break;
		case 1:
			TimerGage->SetColor({ 1,1,1,alphaCurve.Evaluate(m_animationTime) });

			float alphaEndTime = alphaCurve.GetKeyframes().back().time;

			TimerGage->SetValue(gageCurve.Evaluate(m_animationTime));

			float gageEndTime = gageCurve.GetKeyframes().back().time;

			float animWholeTime = alphaEndTime > gageEndTime ? alphaEndTime : gageEndTime;

			if (m_animationTime > animWholeTime)
			{
				m_textChanged = false;
				m_playingAnimation = false;
				m_animState = 0;
			}
			break;
		}
	}
}

void MMMEngine::TimerUI::SetWaveCount(int count)
{
	m_waveCount = count;
}

void MMMEngine::TimerUI::SetMaxWaveNum(int num)
{
	m_maxWaveNum = num;
}

void MMMEngine::TimerUI::SwitchWave()
{
	if (m_playingAnimation)
	{
		return;
	}
	m_textChanged = false;
	m_animationTime = 0.0f;
	m_playingAnimation = true;
	m_isWaveMode = true;
	m_animState = 0;
}

void MMMEngine::TimerUI::SwitchTimer()
{
	if (m_playingAnimation)
	{
		return;
	}
	m_textChanged = false;
	m_animationTime = 0.0f;
	m_playingAnimation = true;
	m_isWaveMode = false;
	m_animState = 0;
}

void MMMEngine::TimerUI::ForceSwitchWave()
{
	m_isWaveMode = true;
	WaveText->SetColor({ 1,1,1,1.0f });
	WaveBack->SetColor({ 1,1,1,1.0f });
	TimerGage->SetColor({ 1,1,1,0.0f });
}

void MMMEngine::TimerUI::ForceSwitchTimer()
{
	m_isWaveMode = false;
	WaveText->SetColor({ 1,1,1,0.0f });
	WaveBack->SetColor({ 1,1,1,0.0f });
	TimerGage->SetColor({ 1,1,1,1.0f });
}

bool MMMEngine::TimerUI::IsPlayingAnimation()
{
	return m_playingAnimation;
}
