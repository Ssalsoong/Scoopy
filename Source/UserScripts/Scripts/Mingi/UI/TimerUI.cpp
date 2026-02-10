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
	if (!TimerAlpha.IsEmpty())
	{
		float curveTime = TimerAlpha.GetKeyframes().back().time;
		m_maximumAnimTime = m_maximumAnimTime > curveTime ? m_maximumAnimTime : curveTime;
	}
	if (!WaveBackAlpha.IsEmpty())
	{
		float curveTime = WaveBackAlpha.GetKeyframes().back().time;
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

	m_animationTime += Time::GetDeltaTime();
	m_rect->SetWorldScale({ scaleCurve.Evaluate(m_animationTime),scaleCurve.Evaluate(m_animationTime),1 });
	m_rect->SetAnchoredPosition({ m_initPosX, m_initPosY + posCurve.Evaluate(m_animationTime) });
	m_rect->SetWorldEulerRotation({ 0,0,rotCurve.Evaluate(m_animationTime)});

	TimerGage->SetColor({ 1.0f,1.0f,1.0f,TimerAlpha.Evaluate(m_animationTime) });
	WaveText->SetColor({ 1.0f,1.0f,1.0f,WaveBackAlpha.Evaluate(m_animationTime) });
	WaveBack->SetAlpha(WaveBackAlpha.Evaluate(m_animationTime));

	if (!m_textChanged && m_animationTime > changeTextTime)
	{
		m_textChanged = true;
		// 새 텍스트 문자열 
		// 최대 웨이브 수치를 넘은 경우는 원래대로 돌아가기
		m_waveCount %= m_maxWaveNum;
		std::string wave = std::to_string(m_waveCount + 1) + "/" + std::to_string(m_maxWaveNum);
		WaveText->SetTextUtf8(wave);

		// 웨이브 표기 후 증가
		++m_waveCount;
	}

	if (m_animationTime > m_maximumAnimTime)
	{
		m_textChanged = false;
		m_playingAnimation = false;
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

void MMMEngine::TimerUI::ShowNextWave()
{
	if (m_playingAnimation)
	{
		return;
	}

	m_textChanged = false;
	m_animationTime = 0.0f;
	m_playingAnimation = true;
}
