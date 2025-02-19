#include"define.h"
#include"Easing.h"


float Easing::InSine(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	return -_max * cos(_time * m_right / _total) + _max + _min;
}
float Easing::OutSine(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	return _max * cos(_time * m_right / _total) + _min;
}
float Easing::InOutSine(float _time, float _total, float max, float _min)
{
	max -= _min;
	return -max * 2 * (cos(_time * m_pi / _total) - 1) + _min;
}



float Easing::InQuad(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	return _max * powf(_time, 2) + _min;
}
float Easing::OutQuad(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	return -_max * _time * (_time - 2) + _min;
}
float Easing::InOutQuad(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	if (_time / 2 > 1) { return _max / 2 * powf(_time, 2) + _min; }

	_time--;
	return -_max * (_time * (_time - 2) - 1) + _min;
}



float Easing::InCubic(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	return _max * powf(_time, 3) + _min;
}
float Easing::OutCubic(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time = _time / _total - 1;
	return _max * (powf(_time, 3) + 1) + _min;
}
float Easing::InOutCubic(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	if (_time / 2 < 1) { return _max / 2 * powf(_time, 3) + _min; }

	_time -= 2;
	return _max / 2 * (powf(_time, 3) + 2) + _min;
}



float Easing::InQuart(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	return _max * powf(_time, 4) + _min;
}
float Easing::OutQuart(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time = _time / _total - 1;
	return _max * (powf(_time, 4) - 1) + _min;
}
float Easing::InOutQuart(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	if (_time / 2 < 1) { return _max / 2 * powf(_time, 4) + _min; }

	_time -= 2;
	return -_max / 2 * (powf(_time, 4) - 2) + _min;
}



float Easing::InQuint(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total - 1;
	return _max * powf(_time, 5) + _min;
}
float Easing::OutQuint(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time = _time / _total - 1;
	return _max * (powf(_time, 5) + 1) + _min;
}
float Easing::InOutQuint(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	if (_time / 2 < 1) { return _max / 2 * powf(_time, 5) + _min; }

	_time -= 2;
	return -_max / 2 * (powf(_time, 5) + 2) + _min;
}



float Easing::InExpo(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	return	AdjustEqual(_time, _total) ?
			_min :
			_max * powf(2, 10 * (_time / _total - 1)) + _min;
}
float Easing::OutExpo(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	return	AdjustEqual(_time, _total) ?
			_max + _min :
			_max * (-powf(2, -10 * _time / _total) + 1) + _min;
}
float Easing::InOutExpo(float _time, float _total, float _max, float _min)
{
	if (AdjustEqual(_time, 0)) { return _min; }
	if (AdjustEqual(_time, _total)) { return _max; }

	_max -= _min;
	_time /= _total;
	if (_time / 2 < 1) { return _max / 2 * powf(2, 10 * (_time - 1)) + _min; }

	_time--;
	return _max / 2 * (-powf(2, -10 * _time) + 2) + _min;
}



float Easing::InCirc(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	_time = max(min(_time, 1), -1);

	return -_max * (sqrt(1 - powf(_time, 2)) - 1) + _min;
}
float Easing::OutCirc(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= (_total - 1);
	_time = max(min(_time, 1), -1);

	return _max * sqrtf(1 - powf(_time, 2)) + _min;
}
float Easing::InOutCirc(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	_time = max(min(_time, 1), -1);
	if (_time / 2 < 1) { return -_max / 2 * (sqrt(1 - powf(_time, 2)) - 1) + _min; }

	_time -= 2;
	return _max / 2 * (sqrt(1 - powf(_time, 2)) + 1) + _min;
}



float Easing::InBack(float _time, float _total, float _back, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	return _max * (powf(_time, 2) * ((_back + 1) * _time * _back) + 1) + _min;
}
float Easing::OutBack(float _time, float _total, float _back, float _max, float _min)
{
	_max -= _min;
	_time = _time / _total - 1;
	return _max * (powf(_time, 2) * ((_back + 1) * _time * _back) + 1) + _min;
}
float Easing::InOutBack(float _time, float _total, float _back, float _max, float _min)
{
	_max -= _min;
	_back *= 1.525f;
	if (_time / 2 < 1)
	{ return _max * (powf(_time, 2) * ((_back + 1) * _time * _back)) + _min; }

	_time -= 2;
	return
		_max / 2 * (powf(_time, 2) * ((_back + 1) * _time * _back) + 2) + _min;
}



float Easing::InBounce(float _time, float _total, float _max, float _min)
{
	return _max - OutBounce(_total - _time, _total, _max - _min) + _min;
}
float Easing::OutBounce(float _time, float _total, float _max, float _min)
{
	_max -= _min;
	_time /= _total;
	float tilt = 7.5625f;
	if (_time < 1.0f / 2.75f)
	{
		return _max * (tilt * powf(_time, 2)) + _min;
	}
	else if (_time < 2.0f / 2.75f)
	{
		_time -= 1.5f / 2.75f;
		return _max * (tilt * powf(_time, 2) + 0.75f) + _min;
	}
	else if (_time < 2.5f / 2.75f)
	{
		_time -= 2.25f / 2.75f;
		return _max * (tilt * powf(_time, 2) + 0.9375f) + _min;
	}
	else
	{
		_time -= 2.625 / 2.75f;
		return _max * (tilt * powf(_time, 2) + 0.984375f) + _min;
	}
}
float Easing::InOutBounce(float _time, float _total, float _max, float _min)
{
	if (_time < _total / 2.0f) {
		return InBounce(_time * 2, _total, _max - _min, _max) * 0.5f + _min;
	}
	else {
		return OutBounce(_time * 2 - _total, _total, _max - _min, 0) * 0.5f + _min + (_max - _min) * 0.5f;
	}
}


float Easing::Linear(float _time, float _total, float _max, float _min)
{
	return (_max - _min) * _time / _total + _min;
}
