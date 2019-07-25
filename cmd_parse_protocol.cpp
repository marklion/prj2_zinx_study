#include "cmd_parse_protocol.h"
#include "cmd_msg.h"
#include <list>
#include"echo_role.h"

cmd_parse_protocol cmd_parse_protocol::m_instance;

cmd_parse_protocol::cmd_parse_protocol()
{
}


cmd_parse_protocol::~cmd_parse_protocol()
{
}

/*参数就是通道对象readfd的传参参数*/
//调用时机，数据被读取之后
UserData * cmd_parse_protocol::raw2request(std::string _szInput)
{
	//命令解析
	cmd_msg *pout = new cmd_msg();
	if ("exit" == _szInput)
	{
		pout->is_frame_exit = true;
	}
	if ("close" == _szInput)
	{
		pout->output_open = false;
	}
	if ("open" == _szInput)
	{
		pout->output_open = true;
	}

	pout->echo_string = _szInput;
	return pout;
}

/*调用时机：调用zinxSendOUt之后*/
/*参数来自于sendout的第一个参数，返回值（转换的结果）要交给通道对象，被通道对象的writefd函数消费*/
std::string * cmd_parse_protocol::response2raw(UserData & _oUserData)
{
	GET_REF2DATA(cmd_msg, output, _oUserData);

	return new std::string(output.echo_string);
}

/*确定谁来处理命令消息（cmd_msg对象）----》返回该对象*/
/*在原始数据转换成用户请求后被调用*/
Irole * cmd_parse_protocol::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	//记录数据来源的通道信息
	szInputChannelInfo = _oUserDataMsg.szInfo;

	//判断消息内容分发到不同role对象
	GET_REF2DATA(cmd_msg, output, *_oUserDataMsg.poUserData);
	//查找role对象函数
	std::list<Irole*> role_list = ZinxKernel::Zinx_GetAllRole();
	/*过滤所需的role对象*/
	if (output.echo_string == "exit")
	{
		/*过滤exit_framework对象*/
		for (auto role : role_list)
		{
			if (NULL != dynamic_cast<exit_framework_role *>(role))
			{
				return role;
			}
		}
	}
	if (output.echo_string == "open" || output.echo_string == "close")
	{
		/*过滤output_mng_role对象*/
		for (auto role : role_list)
		{
			if (NULL != dynamic_cast<output_mng_role *>(role))
			{
				return role;
			}
		}
	}
	/*过滤echo_role对象*/
	for (auto role : role_list)
	{
		if (NULL != dynamic_cast<echo_role *>(role))
		{
			return role;
		}
	}
	return nullptr;
}

Ichannel * cmd_parse_protocol::GetMsgSender(BytesMsg & _oBytes)
{
	if (szInputChannelInfo == "stdin_channel")
	{
		return ZinxKernel::Zinx_GetChannel_ByInfo("stdout_channel");
	}
	return ZinxKernel::Zinx_GetChannel_ByInfo(szInputChannelInfo);
}
