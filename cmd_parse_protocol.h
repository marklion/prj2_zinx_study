#pragma once
#include <zinx.h>
class cmd_parse_protocol :
	public Iprotocol
{
	cmd_parse_protocol();
	static cmd_parse_protocol m_instance;
public:
	
	static cmd_parse_protocol *GetInstance() { return &m_instance; }
	virtual ~cmd_parse_protocol();

	// Í¨¹ý Iprotocol ¼Ì³Ð
	virtual UserData * raw2request(std::string _szInput) override;
	virtual std::string * response2raw(UserData & _oUserData) override;
	virtual Irole * GetMsgProcessor(UserDataMsg & _oUserDataMsg) override;
	virtual Ichannel * GetMsgSender(BytesMsg & _oBytes) override;
};

