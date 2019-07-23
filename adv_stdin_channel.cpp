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

bool adv_stdin_channel::Init()
{
	return true;
}

bool adv_stdin_channel::ReadFd(std::string & _input)
{
	cin >> _input;
}

bool adv_stdin_channel::WriteFd(std::string & _output)
{
	return false;
}

void adv_stdin_channel::Fini()
{
}

int adv_stdin_channel::GetFd()
{
	return 0;
}

std::string adv_stdin_channel::GetChannelInfo()
{
	return "stdin_channel";
}

AZinxHandler * adv_stdin_channel::GetInputNextStage(BytesMsg & _oInput)
{
	//返回协议对象
	return cmd_parse_protocol::GetInstance();
}
