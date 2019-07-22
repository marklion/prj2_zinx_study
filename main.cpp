#include <cstdio>
#include <iostream>
#include <zinx.h>

using namespace std;

//2. 创建数据处理类--》继承AzhinxHandler
class echo :public AZinxHandler {
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*将数据输出到标准输出*/
		GET_REF2DATA(BytesMsg, byte, _oInput);
		cout << byte.szData << endl;
		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		//没有下一个环节
		return nullptr;
	}
};

echo g_echo;

//3. 创建IO通道类-- - 》继承Ichannel类
class stdin_channel :public Ichannel {
	// 通过 Ichannel 继承
	virtual bool Init() override
	{
		return true;
	}
	virtual bool ReadFd(std::string & _input) override
	{
		//将标准输入的数据存到_input中
		cin >> _input;
		return true;
	}
	virtual bool WriteFd(std::string & _output) override
	{
		return false;
	}
	virtual void Fini() override
	{
	}
	virtual int GetFd() override
	{
		return 0;
	}
	virtual std::string GetChannelInfo() override
	{
		return "stdin_channel";
	}
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override
	{
		/*返回处理读取到数据的对象,返回g_echo对象*/
		return &g_echo;
	}
};

int main()
{
	//1. 初始化框架
	ZinxKernel::ZinxKernelInit();
	stdin_channel *pchannel = new stdin_channel();


	//4. 添加通道类对象到kernel中
	ZinxKernel::Zinx_Add_Channel(*pchannel);
	//5. 调用run
	ZinxKernel::Zinx_Run();

}