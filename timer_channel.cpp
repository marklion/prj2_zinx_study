#include "timer_channel.h"
#include <sys/timerfd.h>
#include <stdio.h>

timer_channel::timer_channel()
{
}


timer_channel::~timer_channel()
{
}

/*����timerfd*/
bool timer_channel::Init()
{
	bool bRet = false;
	int fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd >= 0)
	{
		m_fd = fd;
		struct itimerspec period = { {1,0},{1,0} };
		//������ʱ��
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

/*_inputҪ����8�ֽڵĳ�ʱ����*/
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

/*���س�ʱ������Ķ���*/
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

//����_oInput�ǳ�ʱͨ��������ByteMsg  ����-��szData�ǳ�ʱ����
IZinxMsg * timeout_deliver::InternelHandle(IZinxMsg & _oInput)
{
	GET_REF2DATA(BytesMsg, timeout_count, _oInput);

	/*ǿת8���ֽڵ��׵�ַ�޷���������ָ��*/
	unsigned long *pcont = (unsigned long *)timeout_count.szData.data();

	for (int i = 0; i < *pcont; i++)
	{
		/*��������������������1*/
		for (auto ptask : m_task_list)
		{
			//ptask ����m_task_list�����е�ÿ��Ԫ��
			ptask->m_count--;
			/*����Ϊ0��ʱ�����ó�ʱ������,���ü���*/
			if (ptask->m_count <= 0)
			{
				ptask->proc_timeout();
				ptask->m_count = ptask->time_out;
			}
		}

		/*ѭ����ʱ������*/
	}	

	/*û����һ������*/
	return nullptr;
}

AZinxHandler * timeout_deliver::GetNextHandler(IZinxMsg & _oNextMsg)
{
	/*û����һ������*/
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
