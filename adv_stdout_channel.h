#pragma once
#include <zinx.h>
class adv_stdout_channel :
	public Ichannel
{
public:
	adv_stdout_channel();
	virtual ~adv_stdout_channel();

	// Í¨¹ý Ichannel ¼Ì³Ð
	virtual bool Init() override;
	virtual bool ReadFd(std::string & _input) override;
	virtual bool WriteFd(std::string & _output) override;
	virtual void Fini() override;
	virtual int GetFd() override;
	virtual std::string GetChannelInfo() override;
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
};

