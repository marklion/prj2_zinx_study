#include "echo_role.h"

#include "cmd_msg.h"
#include "cmd_parse_protocol.h"

echo_role::echo_role()
{
}


echo_role::~echo_role()
{
}

bool echo_role::Init()
{
	//代表初始化成功
	return true;
}

//回显消息处理函数
UserData * echo_role::ProcMsg(UserData & _poUserData)
{
	//取出待回显的内容
	GET_REF2DATA(cmd_msg, input, _poUserData);
	std::string echo_string = input.echo_string;
	
	//构建待发送消息
	cmd_msg *pout = new cmd_msg();
	pout->echo_string = echo_string;
	//发送出去
	ZinxKernel::Zinx_SendOut(*pout, *(cmd_parse_protocol::GetInstance()));

	//没有后续环节
	return nullptr;
}

void echo_role::Fini()
{
}

bool exit_framework_role::Init()
{
	return true;
}

UserData * exit_framework_role::ProcMsg(UserData & _poUserData)
{
	GET_REF2DATA(cmd_msg, cmd, _poUserData);
	if (cmd.is_frame_exit) {
		ZinxKernel::Zinx_Exit();
	}
	return nullptr;
}

void exit_framework_role::Fini()
{
}

bool output_mng_role::Init()
{
	return true;
}

UserData * output_mng_role::ProcMsg(UserData & _poUserData)
{
	GET_REF2DATA(cmd_msg, cmd, _poUserData);
	
	if (NULL == m_channle)
	{
		m_channle = ZinxKernel::Zinx_GetChannel_ByInfo("stdout_channel");
	}

	if (cmd.output_open == true)
	{
		/*打开输出通道*/
		ZinxKernel::Zinx_Add_Channel(*m_channle);
	}
	else
	{
		/*关闭输出通道*/
		ZinxKernel::Zinx_Del_Channel(*m_channle);
	}

	return nullptr;
}

void output_mng_role::Fini()
{
}
