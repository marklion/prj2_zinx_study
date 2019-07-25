#include "timer_channel.h"
#include <sys/timerfd.h>
#include <stdio.h>

timer_channel::timer_channel()
{
}


timer_channel::~timer_channel()
{
}

/*创建timerfd*/
bool timer_channel::Init()
{
	bool bRet = false;
	int fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd >= 0)
	{
		m_fd = fd;
		struct itimerspec period = { {1,0},{1,0} };
		//开启定时器
		if (0 == timerfd_settime(m_fd, 0, &period, NULL))
		{
			bRet = true;
		}
		else
		{
			close(m_fd);
			m_fd = -1;
		}
		
	}
	return bRet;
}

/*_input要带出8字节的超时次数*/
bool timer_channel::ReadFd(std::string & _input)
{
	bool bRet = false;
	char buff[8];
	if (sizeof(buff) == read(m_fd, buff, sizeof(buff)))
	{
		_input.append(buff, sizeof(buff));
		bRet = true;
	}
	return bRet;
}

bool timer_channel::WriteFd(std::string & _output)
{
	return false;
}

void timer_channel::Fini()
{
	if (m_fd >= 0)
	{
		close(m_fd);
	}
}

int timer_channel::GetFd()
{
	return m_fd;
}

std::string timer_channel::GetChannelInfo()
{
	return "timerfd" + m_fd;
}

/*返回超时管理类的对象*/
AZinxHandler * timer_channel::GetInputNextStage(BytesMsg & _oInput)
{
	return timeout_deliver::GetInstance();
}

timeout_deliver::timeout_deliver()
{
}

timeout_deliver::~timeout_deliver()
{
}

timeout_deliver *timeout_deliver::m_single = NULL;

timeout_deliver * timeout_deliver::GetInstance()
{
	if (m_single == NULL)
	{
		m_single = new timeout_deliver();
	}
	return m_single;
}

//参数_oInput是超时通道传来的ByteMsg  其中-》szData是超时次数
IZinxMsg * timeout_deliver::InternelHandle(IZinxMsg & _oInput)
{
	GET_REF2DATA(BytesMsg, timeout_count, _oInput);

	/*强转8个字节的首地址无符号整数的指针*/
	unsigned long *pcont = (unsigned long *)timeout_count.szData.data();

	for (int i = 0; i < *pcont; i++)
	{
		/*遍历任务容器，减计数1*/
		for (auto ptask : m_task_list)
		{
			//ptask 就是m_task_list容器中的每个元素
			ptask->m_count--;
			/*若减为0则超时，调用超时处理函数,重置计数*/
			if (ptask->m_count <= 0)
			{
				ptask->proc_timeout();
				ptask->m_count = ptask->time_out;
			}
		}

		/*循环超时次数次*/
	}	

	/*没有下一个环节*/
	return nullptr;
}

AZinxHandler * timeout_deliver::GetNextHandler(IZinxMsg & _oNextMsg)
{
	/*没有下一个环节*/
	return nullptr;
}

void timeout_deliver::register_task(int _sec, timeout_task * _task)
{
	m_task_list.push_back(_task);
	_task->m_count = _sec;
	_task->time_out = _sec;
}

void timeout_deliver::unregister_task(timeout_task * _task)
{
	m_task_list.remove(_task);
}
