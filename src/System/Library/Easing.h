#pragma once

// �C�[�W���O�֐�
struct Easing
{
public:
	/*---------------------------------------------------------------------
	* �@time	�c�@�o�ߎ���
	* �@total	�c�@��������܂ł̎���
	* �@max		�c�@�ő�l(��{�l 1)
	* �@min		�c�@�ŏ��l(��{�l 0)
	---------------------------------------------------------------------*/
	//�@Sine�֐�
	float InSine(float _time, float _total, float _max = 1, float _min = 0);
	float OutSine(float _time, float _total, float _max = 1, float _min = 0);
	float InOutSine(float _time, float _total, float _max = 1, float _min = 0);

	//�@�񎟊֐�
	float InQuad(float _time, float _total, float _max = 1, float _min = 0);
	float OutQuad(float _time, float _total, float _max = 1, float _min = 0);
	float InOutQuad(float _time, float _total, float _max = 1, float _min = 0);

	//�@�O���֐�
	float InCubic(float _time, float _total, float _max = 1, float _min = 0);
	float OutCubic(float _time, float _total, float _max = 1, float _min = 0);
	float InOutCubic(float _time, float _total, float _max = 1, float _min = 0);

	//�@�l���֐�
	float InQuart(float _time, float _total, float _max = 1, float _min = 0);
	float OutQuart(float _time, float _total, float _max = 1, float _min = 0);
	float InOutQuart(float _time, float _total, float _max = 1, float _min = 0);

	//�@�܎��֐�
	float InQuint(float _time, float _total, float _max = 1, float _min = 0);
	float OutQuint(float _time, float _total, float _max = 1, float _min = 0);
	float InOutQuint(float _time, float _total, float _max = 1, float _min = 0);

	//�@�w���֐�
	float InExpo(float _time, float _total, float _max = 1, float _min = 0);
	float OutExpo(float _time, float _total, float _max = 1, float _min = 0);
	float InOutExpo(float _time, float _total, float _max = 1, float _min = 0);

	//�@�~��֐�
	float InCirc(float _time, float _total, float _max = 1, float _min = 0);
	float OutCirc(float _time, float _total, float _max = 1, float _min = 0);
	float InOutCirc(float _time, float _total, float _max = 1, float _min = 0);

	//�@�o�b�N�֐�
	float InBack(float _time, float _total, float _back, float _max = 1, float _min = 0);
	float OutBack(float _time, float _total, float _back, float _max = 1, float _min = 0);
	float InOutBack(float _time, float _total, float _back, float _max = 1, float _min = 0);

	//�@�e�͊֐�
	float InBounce(float _time, float _total, float _max = 1, float _min = 0);
	float OutBounce(float _time, float _total, float _max = 1, float _min = 0);
	float InOutBounce(float _time, float _total, float _max = 1, float _min = 0);

	//�@����
	float Linear(float _time, float _total, float _max = 1, float _min = 0);

private:
	bool AdjustEqual(float _epsilon, float num)
	{
		num = fabs(num);
		return fabs(_epsilon - num) <= std::numeric_limits<float>::epsilon();
	}


	const float m_right		= D3DXToRadian(90.0f);
	const float m_straight	= D3DXToRadian(180.0f);
	const float m_pi		= D3DX_PI;

public:
	static Easing& GetInstance() {
		static Easing Instance;
		return Instance;
	}

private:
	Easing() {}
};

#define EASING Easing::GetInstance()