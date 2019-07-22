#include <cstdio>
#include <iostream>
#include <zinx.h>

using namespace std;



//创建标准输出通道类
class stdout_channel :public Ichannel {
	// 通过 Ichannel 继承
	virtual bool Init() override
	{
		return true;
	}
	virtual bool ReadFd(std::string & _input) override
	{
		return false;
	}
	virtual bool WriteFd(std::string & _output) override
	{
		cout << _output << endl;
		return true;
	}
	virtual void Fini() override
	{
	}
	virtual int GetFd() override
	{
		return 1;
	}
	virtual std::string GetChannelInfo() override
	{
		return "stdout_channel";
	}
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override
	{
		return nullptr;
	}
};



//2. 创建数据处理类--》继承AzhinxHandler
class echo :public AZinxHandler {
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*将数据输出到标准输出*/
		GET_REF2DATA(BytesMsg, byte, _oInput);
	
		ZinxKernel::Zinx_SendOut(byte.szData, *(ZinxKernel::Zinx_GetChannel_ByInfo("stdout_channel")));

		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		//没有下一个环节
		return nullptr;
	}
};

echo g_echo;



class exitFramework :public AZinxHandler {
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		ZinxKernel::Zinx_Exit();
		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return nullptr;
	}
};

exitFramework g_exit;

class cmd :public AZinxHandler {
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*透传输入的字节--》copy参数*/
		GET_REF2DATA(BytesMsg, byte, _oInput);
		return new BytesMsg(byte);
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		/*返回echo对象或者exitframework对象*/
		GET_REF2DATA(BytesMsg, byte, _oNextMsg);
		if ("exit" == byte.szData)
		{
			//返回退出类的对象
			return &g_exit;
		}
		else
		{
			//返回回显对象
			return &g_echo;
		}
		return nullptr;
	}
};

cmd g_cmd;

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
		/*返回处理读取到数据的对象,返回命令判断类的对象*/
		return &g_cmd;
	}
};

int main()
{
	//1. 初始化框架
	if (true == ZinxKernel::ZinxKernelInit())
	{
		stdin_channel *pstdin_channel = new stdin_channel();
		stdout_channel *pstdout_channel = new stdout_channel();
		//4. 添加通道类对象到kernel中
		ZinxKernel::Zinx_Add_Channel(*pstdin_channel);
		ZinxKernel::Zinx_Add_Channel(*pstdout_channel);
		//5. 调用run
		ZinxKernel::Zinx_Run();

		ZinxKernel::Zinx_Del_Channel(*pstdin_channel);
		ZinxKernel::Zinx_Del_Channel(*pstdout_channel);

		delete pstdin_channel;
		delete pstdout_channel;

		ZinxKernel::ZinxKernelFini();
	}
}