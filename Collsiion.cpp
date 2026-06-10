#include "Collision.h"


bool Collision::CheckCircleToCircle(
	const VECTOR& centerPosA,
	const float& radiusA,
	const VECTOR& centerPosB,
	const float& radiusB)
{
	// 当たり判定を書く

	// X軸の距離（緑の横線）
	float distanceX = centerPosA.x - centerPosB.x;
	// Y軸の距離（緑の横線）
	float distanceY = centerPosA.y - centerPosB.y;

	// 判定
	float powX = distanceX * distanceX; // 横線の2乗
	float powY = distanceY * distanceY; // 縦線の2乗
	float powR = (radiusA + radiusB) * (radiusA + radiusB); // それぞれの半径を足したものの2乗
	if ((powX+powY) <=powR)
	{
		return true;    // 成功
	}

	// 条件を満たさないのであれば失敗
	return false;
}