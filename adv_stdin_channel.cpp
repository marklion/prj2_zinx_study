#include "adv_stdin_channel.h"
#include <iostream>
#include "cmd_parse_protocol.h"

using namespace std;

adv_stdin_channel::adv_stdin_channel()
{
}


adv_stdin_channel::~adv_stdin_channel()
{
}

/*����false�Ļ�ͨ����ӵ�kernel�ͻ�ʧ��*/
bool adv_stdin_channel::Init()
{
	return true;
}

/*�����������ٺ󱻵��ã�epollwait֮�󱻵��ã�*/
bool adv_stdin_channel::ReadFd(std::string & _input)
{
	cin >> _input;
	return true;
}

/*�����ļ����������������ʱ����ã��󲿷�ʱ��*/
bool adv_stdin_channel::WriteFd(std::string & _output)
{
	return false;
}

void adv_stdin_channel::Fini()
{
}

/*��ͨ����ӵ�kernel��ʱ�����ã���epoll��*/
int adv_stdin_channel::GetFd()
{
	return 0;
}

std::string adv_stdin_channel::GetChannelInfo()
{
	return "stdin_channel";
}

/*����ʱ��--�����ݱ���ȡ��ReadFd����*/
AZinxHandler * adv_stdin_channel::GetInputNextStage(BytesMsg & _oInput)
{
	//����Э�����--����������ͨ����ȡ��������
	return cmd_parse_protocol::GetInstance();
}
