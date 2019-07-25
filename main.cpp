#include <cstdio>
#include <iostream>
#include <zinx.h>
#include <map>
#include "adv_stdin_channel.h"
#include "adv_stdout_channel.h"
#include "echo_role.h"
#include <ZinxTCP.h>
#include "my_tcpData.h"
#include "timer_channel.h"

using namespace std;


#if 0
//������׼���ͨ����
class stdout_channel :public Ichannel {
	// ͨ�� Ichannel �̳�
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



//2. �������ݴ�����--���̳�AzhinxHandler
class echo :public AZinxHandler {
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*�������������׼���*/
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
		//û����һ������
		return nullptr;
	}
};

echo g_echo;



class exitFramework :public AZinxHandler {
	// ͨ�� AZinxHandler �̳�
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
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		if (NULL == m_channel)
		{
			Ichannel *pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout_channel");
			m_channel = pchannel;
		}
		
		GET_REF2DATA(BytesMsg, byte, _oInput);
		//ժ�����ͨ��
		if ("close" == byte.szData)
		{
			ZinxKernel::Zinx_Del_Channel(*m_channel);
		}
		else
		{
			//������ͨ��
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
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		/*͸��������ֽ�--��copy����*/
		GET_REF2DATA(BytesMsg, byte, _oInput);
		return new BytesMsg(byte);
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		/*����echo�������exitframework����*/
		GET_REF2DATA(BytesMsg, byte, _oNextMsg);
		
		//���������ַ�����Ӧ�������
		if (m_cmd_map.end() != m_cmd_map.find(byte.szData))
		{
			return m_cmd_map[byte.szData];
		}
		//�����������򷢸�������
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

//3. ����IOͨ����-- - ���̳�Ichannel��
class stdin_channel :public Ichannel {
	// ͨ�� Ichannel �̳�
	virtual bool Init() override
	{
		return true;
	}
	virtual bool ReadFd(std::string & _input) override
	{
		//����׼��������ݴ浽_input��
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
		/*���ش����ȡ�����ݵĶ���,���������ж���Ķ���*/
		return &g_cmd;
	}
};

#endif


class timer_out_hello :public timeout_task {
	// ͨ�� timeout_task �̳�
	virtual void proc_timeout() override
	{
		cout << "hello " << endl;
	}
};

int main()
{

	//1. ��ʼ�����
	if (true == ZinxKernel::ZinxKernelInit())
	{
#if 0
		stdin_channel *pstdin_channel = new stdin_channel();
		stdout_channel *pstdout_channel = new stdout_channel();
		//4. ���ͨ�������kernel��
		ZinxKernel::Zinx_Add_Channel(*pstdin_channel);
		ZinxKernel::Zinx_Add_Channel(*pstdout_channel);
		g_cmd.add_cmd("exit", &g_exit);
		g_cmd.add_cmd("close", &g_output_mng);
		g_cmd.add_cmd("open", &g_output_mng);
		//5. ����run
		ZinxKernel::Zinx_Run();

		ZinxKernel::Zinx_Del_Channel(*pstdin_channel);
		ZinxKernel::Zinx_Del_Channel(*pstdout_channel);

		delete pstdin_channel;
		delete pstdout_channel;
#endif
		/*���ͨ����kernel*/
		adv_stdin_channel *pin = new adv_stdin_channel();
		adv_stdout_channel *pout = new adv_stdout_channel();
		ZinxKernel::Zinx_Add_Channel(*pin);
		ZinxKernel::Zinx_Add_Channel(*pout);

		/*��Ӽ���ͨ����kernel*/
		auto plisten = new ZinxTCPListen(33333, new my_tcpconnFact());
		ZinxKernel::Zinx_Add_Channel(*plisten);

		/*��Ӷ�ʱ��ͨ����kernel��*/
		auto ptimerchannel = new timer_channel();
		ZinxKernel::Zinx_Add_Channel(*ptimerchannel);

		/*ע�ᶨʱ������*/
		timer_out_hello my_task;
		timeout_deliver::GetInstance()->register_task(3, &my_task);

		/*���role����kernel*/
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