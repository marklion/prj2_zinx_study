#include <cstdio>
#include <iostream>
#include <zinx.h>
#include <map>
#include "adv_stdin_channel.h"
#include "adv_stdout_channel.h"
#include "echo_role.h"

using namespace std;


#if 0
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
		Ichannel *pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout_channel");
		if (NULL != pchannel)
		{
			ZinxKernel::Zinx_SendOut(byte.szData, *pchannel);
		}
		

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
class output_mng :public AZinxHandler {
	Ichannel *m_channel = NULL;
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		if (NULL == m_channel)
		{
			Ichannel *pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout_channel");
			m_channel = pchannel;
		}
		
		GET_REF2DATA(BytesMsg, byte, _oInput);
		//摘出输出通道
		if ("close" == byte.szData)
		{
			ZinxKernel::Zinx_Del_Channel(*m_channel);
		}
		else
		{
			//添加输出通道
			ZinxKernel::Zinx_Add_Channel(*m_channel);
		}
		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return nullptr;
	}
};

output_mng g_output_mng;
class cmd :public AZinxHandler {
	map<string, AZinxHandler *> m_cmd_map;
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
		
		//若是命令，则分发给对应处理对象
		if (m_cmd_map.end() != m_cmd_map.find(byte.szData))
		{
			return m_cmd_map[byte.szData];
		}
		//若不是命令则发给回显类
		else
		{
			return &g_echo;
		}
	}
public:
	void add_cmd(string _cmd, AZinxHandler *_handler)
	{
		m_cmd_map[_cmd] = _handler;
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

#endif

int main()
{

	//1. 初始化框架
	if (true == ZinxKernel::ZinxKernelInit())
	{
#if 0
		stdin_channel *pstdin_channel = new stdin_channel();
		stdout_channel *pstdout_channel = new stdout_channel();
		//4. 添加通道类对象到kernel中
		ZinxKernel::Zinx_Add_Channel(*pstdin_channel);
		ZinxKernel::Zinx_Add_Channel(*pstdout_channel);
		g_cmd.add_cmd("exit", &g_exit);
		g_cmd.add_cmd("close", &g_output_mng);
		g_cmd.add_cmd("open", &g_output_mng);
		//5. 调用run
		ZinxKernel::Zinx_Run();

		ZinxKernel::Zinx_Del_Channel(*pstdin_channel);
		ZinxKernel::Zinx_Del_Channel(*pstdout_channel);

		delete pstdin_channel;
		delete pstdout_channel;
#endif
		/*添加通道到kernel*/
		adv_stdin_channel *pin = new adv_stdin_channel();
		adv_stdout_channel *pout = new adv_stdout_channel();
		ZinxKernel::Zinx_Add_Channel(*pin);
		ZinxKernel::Zinx_Add_Channel(*pout);
		/*添加role对象到kernel*/
		echo_role *pecho = new echo_role();
		ZinxKernel::Zinx_Add_Role(*pecho);
		auto pexit = new exit_framework_role();
		ZinxKernel::Zinx_Add_Role(*pexit);
		auto poutput_mng = new output_mng_role();
		ZinxKernel::Zinx_Add_Role(*poutput_mng);
		ZinxKernel::Zinx_Run();

		ZinxKernel::ZinxKernelFini();
	}
}