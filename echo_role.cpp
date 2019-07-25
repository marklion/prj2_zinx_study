#include "echo_role.h"

#include "cmd_msg.h"
#include "cmd_parse_protocol.h"

echo_role::echo_role()
{
}


echo_role::~echo_role()
{
}

/*����ʱ��--������ӵ�kernel��ʱ������*/
bool echo_role::Init()
{
	//�����ʼ���ɹ�
	return true;
}

//������Ϣ������
/*����ʱ����ԭʼ����ת�����û������*/
//����������Э�����raw2request�ķ���ֵ
UserData * echo_role::ProcMsg(UserData & _poUserData)
{
	//ȡ�������Ե�����
	GET_REF2DATA(cmd_msg, input, _poUserData);
	std::string echo_string = input.echo_string;
	
	//������������Ϣ
	cmd_msg *pout = new cmd_msg();
	pout->echo_string = echo_string;
	//���ͳ�ȥ
	ZinxKernel::Zinx_SendOut(*pout, *(cmd_parse_protocol::GetInstance()));

	//û�к�������
	return nullptr;
}

/*��ժ��kernelʱ������*/
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
		/*�����ͨ��*/
		ZinxKernel::Zinx_Add_Channel(*m_channle);
	}
	else
	{
		/*�ر����ͨ��*/
		ZinxKernel::Zinx_Del_Channel(*m_channle);
	}

	return nullptr;
}

void output_mng_role::Fini()
{
}
