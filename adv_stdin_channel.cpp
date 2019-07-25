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

/*返回false的话通道添加到kernel就会失败*/
bool adv_stdin_channel::Init()
{
	return true;
}

/*会在数据来临后被调用（epollwait之后被调用）*/
bool adv_stdin_channel::ReadFd(std::string & _input)
{
	cin >> _input;
	return true;
}

/*会在文件输出缓存区不满的时候调用（大部分时候）*/
bool adv_stdin_channel::WriteFd(std::string & _output)
{
	return false;
}

void adv_stdin_channel::Fini()
{
}

/*在通道添加到kernel中时被调用（加epoll）*/
int adv_stdin_channel::GetFd()
{
	return 0;
}

std::string adv_stdin_channel::GetChannelInfo()
{
	return "stdin_channel";
}

/*调用时机--》数据被读取（ReadFd）后*/
AZinxHandler * adv_stdin_channel::GetInputNextStage(BytesMsg & _oInput)
{
	//返回协议对象--》用来处理本通道读取到的数据
	return cmd_parse_protocol::GetInstance();
}
