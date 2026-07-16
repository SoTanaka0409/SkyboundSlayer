#pragma once

class Fade
{
public:
	enum class State
	{
		None,
		FadeIn,
		FadeOut
	};

	static Fade* GetInstance()
	{
		static Fade instance;
		return &instance;
	}

	void Initialize();
	void Update();
	void Draw();

	void StartFadeIn();
	void StartFadeOut();

	State GetState() const { return mState; }
	bool IsFadeInFinished() const { return mState == State::FadeIn && mAlpha <= 0; }
	bool IsFadeOutFinished() const { return mState == State::FadeOut && mAlpha >= 255; }
	bool IsFading() const { return mState != State::None; }

private:
	Fade();
	~Fade();

	State mState;
	int mAlpha;
	int mFadeSpeed;
};
