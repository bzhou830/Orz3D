#include "BzTimer.h"
using namespace std::chrono;

//! 构造函数，创建Timer，并且获取当前时间保存到last成员里面.
/*!
\param  none
\return none
*/
BzTimer::BzTimer()
{
    last = steady_clock::now();
}

//! 获取当前时间，更新原时间为当前时间，并且返回上个时间节点和这个时间节点之间的时间差
/*!
\param  none
\return 上个时间节点和这个时间节点之间的时间差
*/
float BzTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

//! 获取上个标记点和当前时间之间的时间差
/*!
\param  none
\return 上个标记点和当前时间之间的时间差
*/
float BzTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}