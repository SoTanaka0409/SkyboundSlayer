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

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Initialize();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void StartFadeIn();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void StartFadeOut();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	State GetState() const { return state_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsFadeInFinished() const { return state_ == State::FadeIn && alpha_ <= 0; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsFadeOutFinished() const { return state_ == State::FadeOut && alpha_ >= 255; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsFading() const { return state_ != State::None; }

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Fade();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Fade();

	State state_;
	int alpha_;
	int fade_speed_;
};
