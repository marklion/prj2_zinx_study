#include "adv_stdout_channel.h"
#include <iostream>

using namespace std;


adv_stdout_channel::adv_stdout_channel()
{
}


adv_stdout_channel::~adv_stdout_channel()
{
}

bool adv_stdout_channel::Init()
{
	return true;
}

bool adv_stdout_channel::ReadFd(std::string & _input)
{
	return false;
}

bool adv_stdout_channel::WriteFd(std::string & _output)
{
	cout << _output << endl;

	return true;
}

void adv_stdout_channel::Fini()
{
}

int adv_stdout_channel::GetFd()
{
	return 1;
}

std::string adv_stdout_channel::GetChannelInfo()
{
	return "stdout_channel";
}

AZinxHandler * adv_stdout_channel::GetInputNextStage(BytesMsg & _oInput)
{
	return nullptr;
}
