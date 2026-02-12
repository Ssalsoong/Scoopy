#include "Export.h"
#include "ScriptBehaviour.h"
#include "LevelUpBubble.h"
#include "InputManager.h"
#include "ControlManager.h"
#include "MMMTime.h"

void MMMEngine::LevelUpBubble::Start()
{
	if (!LevelUpManager::Get()) {
		std::cout << "LeveUpBubble::No LevelUpManager!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mPlayer) {
		mPlayer = GameObject::FindWithTag("Player");
		if (!mPlayer) {
			std::cout << "LeveUpBubble::No Player!!" << std::endl;
			Destroy(GetGameObject());
		}
	}

	mCanvas = LevelUpManager::Get()->GetCanvas();
	mSpeechBubbleIcon = LevelUpManager::Get()->mSpeechBubbleIcon;
	mHeadline = LevelUpManager::Get()->mHeadlineText;
	mScript = LevelUpManager::Get()->mScriptText;
}

void MMMEngine::LevelUpBubble::Update()
{
	if (isActive) {
		SetUIActive(true);

		Vector2 zero = Vector2::Zero;
		SetUITrans(mSpeechBubbleIcon->GetRectTransform(), mSpeechOffset, zero);
		SetUITrans(mHeadline->GetRectTransform(), mSpeechOffset + mHeadlineOffset, zero);
		SetUITrans(mScript->GetRectTransform(), mSpeechOffset + mScriptOffset, zero);

		SetIconTrans();
		UpdateIcon();
		UpdateControl();

		if (isDirty) {
			SetHeadlineText(LevelUpManager::Get()->GetHeadline(mSelectIdx));
			SetScriptText(LevelUpManager::Get()->GetScripts(mSelectIdx));
			isDirty = false;
		}

		// 애니메이션 효과
		if (isAnimating) {
			mElipsedTime += Time::GetDeltaTime();
			if (mElipsedTime > mBubbleSizeCurve.GetKeyframes().back().time)
				isAnimating = false;
			else {
				mCurveScale = mBubbleSizeCurve.Evaluate(mElipsedTime);
			}
		}

		// 최종스케일
		mFinalScale = mUIScale * mCurveScale;
	}
	else
	{
		mHeadline->GetGameObject()->SetActive(false);
		mScript->GetGameObject()->SetActive(false);

		if (isAnimating) {
			if (mElipsedTime > mCloseSizeCurve.GetKeyframes().back().time)
				isAnimating = false;

			mElipsedTime += Time::GetDeltaTime();
			mCurveScale = mCloseSizeCurve.Evaluate(mElipsedTime);

			// 최종스케일
			mFinalScale = mUIScale * mCurveScale;

			Vector2 zero = Vector2::Zero;
			SetUITrans(mSpeechBubbleIcon->GetRectTransform(), mSpeechOffset, zero);
			SetUITrans(mHeadline->GetRectTransform(), mSpeechOffset + mHeadlineOffset, zero);
			SetUITrans(mScript->GetRectTransform(), mSpeechOffset + mScriptOffset, zero);

			SetIconTrans();
			UpdateIcon();
		}
		else
			SetUIActive(false);
	}
}

void MMMEngine::LevelUpBubble::SetHeadlineText(const std::wstring& _text)
{
	mHeadline->SetText(_text);
}

void MMMEngine::LevelUpBubble::SetScriptText(const std::wstring& _text)
{
	mScript->SetText(_text);
}

void MMMEngine::LevelUpBubble::SetActive(bool _val)
{
	isActive = _val;
	
	mSelectIdx = 0;
	mElipsedTime = 0.0f;
	isAnimating = true;
	isDirty = true;

	if (!_val) {
		//// 아이콘들 끄기 확인
		//for (auto& icon : mIcons) {
		//	if (icon->GetGameObject().IsValid())
		//		icon->GetGameObject()->SetActive(false);
		//}

		if (ControlManager::Get()->GetMinLayer() == mInputLayer)
			ControlManager::Get()->ReleaseMinLayer();
	}
}

void MMMEngine::LevelUpBubble::SetIcons(std::vector<ObjPtr<Image>>& _vec)
{
	mIcons.clear();

	if (!_vec.empty()) {
		mIcons = _vec;
	}
}

void MMMEngine::LevelUpBubble::SetUIActive(bool _val)
{
	mSpeechBubbleIcon->GetGameObject()->SetActive(_val);
	mHeadline->GetGameObject()->SetActive(_val);
	mScript->GetGameObject()->SetActive(_val);

	for (auto& icon : mIcons) {
		icon->GetGameObject()->SetActive(_val);
	}
}

void MMMEngine::LevelUpBubble::SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding)
{
	auto tp = mPlayer->GetTransform();
	auto wPos = tp->GetWorldPosition();

	Vector2 canvasSize = mCanvas->GetCanvasSize();
	Vector2 canvasPos = LevelUpManager::Get()->GetCanvasPos(wPos);
	Vector2 achPos = _rectTrans->GetAnchorMin();
	float camDistance = Vector3::Distance(Camera::GetMainCamera()->GetTransform()->GetWorldPosition(), wPos);
	achPos *= canvasSize;

	float distFactor = camDistance * mDistanceFactor;

	Vector2 offset = (_offset * mFinalScale) / distFactor;
	Vector2 padding = (_mPadding * mFinalScale) / distFactor;

	_rectTrans->SetAnchoredPosition(
		(canvasPos - achPos) + offset + padding
	);

	Vector3 baseScale{ 1.0f, 1.0f, 1.0f };
	_rectTrans->SetWorldScale((baseScale * mFinalScale) / distFactor);
}

void MMMEngine::LevelUpBubble::SetIconTrans()
{
	if (mIcons.empty()) {
		std::cout << "LevelUpBubble::Icons Empty !!!" << std::endl;
		return;
	}

	const int N = (int)mIcons.size();
	if (N <= 0) return;

	// 평균 폭(요청대로 고려는 하되, 배치 간격 계산에는 padding 대신 mIconWidth 사용)
	float sumW = 0.0f;
	for (int i = 0; i < N; ++i) {
		sumW += mIcons[i]->GetRectTransform()->GetWidth();
	}
	const float avgW = sumW / (float)N;

	// 중앙 기준 고정 폭 안에서 균등 배치
	const float layoutWidth = mIconWidth * mFinalScale;
	const float step = layoutWidth / (float)N;                  // N칸
	const float left = -layoutWidth * 0.5f;

	for (int i = 0; i < N; ++i) {
		const float x = left + step * ((float)i + 0.5f);        // 각 칸 중앙
		const float y = 0.0f;

		Vector2 pos = (mIconOffset * mFinalScale) + Vector2{ x, y };

		SetUITrans(
			mIcons[i]->GetRectTransform(),
			mSpeechOffset + (mIconOffset * mFinalScale),
			pos
		);
	}
}

void MMMEngine::LevelUpBubble::UpdateControl()
{
	auto& input = ControlManager::Get();

	input->SetMinLayer(mInputLayer);

	static bool isIconAnimDirty = true;

	if (mSelectTime <= mSelectRotCurve.GetKeyframes().back().time) {
		mSelectTime += Time::GetDeltaTime();
		float selectRotZ = mSelectRotCurve.Evaluate(mSelectTime);
		mIcons[mSelectIdx]->GetRectTransform()->SetWorldEulerRotation(0.0f, 0.0f, selectRotZ);
	}
		
	if (isIconAnimDirty) {
		mSelectTime = 0.0f;
		isIconAnimDirty = false;
	}

	if (input->GetKeyDown(KeyCode::LeftArrow, mInputLayer)) {
		if (mSelectIdx > 0) {
			mSelectIdx--;
			isDirty = true;
			isIconAnimDirty = true;
		}
	}
	else if (input->GetKeyDown(KeyCode::RightArrow, mInputLayer)) {
		if (mSelectIdx < mIcons.size() - 1) {
			mSelectIdx++;
			isDirty = true;
			isIconAnimDirty = true;
		}
	}
	else if (input->GetKeyDown(KeyCode::Space, mInputLayer)) {
		//std::cout << "LevelUpBubble::Selected " << std::to_string(mSelectIdx) << std::endl;
		LevelUpManager::Get()->SetSelection(mSelectIdx);
		input->ReleaseMinLayer();
		isActive = false;
		
		// 닫기 애니메이션 재생용
		isAnimating = true;
		mElipsedTime = 0.0f;
	}
}

void MMMEngine::LevelUpBubble::UpdateIcon()
{
	auto tp = mPlayer->GetTransform();
	auto wPos = tp->GetWorldPosition();

	float camDistance = Vector3::Distance(Camera::GetMainCamera()->GetTransform()->GetWorldPosition(), wPos);
	float distFactor = camDistance * mDistanceFactor;

	float deselectIconSize = (mDeselectIconSize * mFinalScale) / distFactor;
	float selectIconSize = (mSelectIconSize * mFinalScale) / distFactor;

	for (auto& icon : mIcons) {
		icon->GetRectTransform()->SetWorldScale(
			deselectIconSize, deselectIconSize, deselectIconSize);
	}

	if (mIcons.size() > mSelectIdx) {
		mIcons[mSelectIdx]->GetRectTransform()->SetWorldScale(
			selectIconSize, selectIconSize, selectIconSize);
	}
}
