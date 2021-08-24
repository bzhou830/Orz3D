#include "BzTimer.h"
using namespace std::chrono;

//! ���캯��������Timer�����һ�ȡ��ǰʱ�䱣�浽last��Ա����.
/*!
\param  none
\return none
*/
BzTimer::BzTimer()
{
    last = steady_clock::now();
}

//! ��ȡ��ǰʱ�䣬����ԭʱ��Ϊ��ǰʱ�䣬���ҷ����ϸ�ʱ��ڵ�����ʱ��ڵ�֮���ʱ���
/*!
\param  none
\return �ϸ�ʱ��ڵ�����ʱ��ڵ�֮���ʱ���
*/
float BzTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

//! ��ȡ�ϸ���ǵ�͵�ǰʱ��֮���ʱ���
/*!
\param  none
\return �ϸ���ǵ�͵�ǰʱ��֮���ʱ���
*/
float BzTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}