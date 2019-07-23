#pragma once
#include <ZinxTCP.h>
class my_tcpData :
	public ZinxTcpData
{
public:
	my_tcpData(int _fd);
	virtual ~my_tcpData();

	// 通过 ZinxTcpData 继承
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
};

class my_tcpconnFact :public IZinxTcpConnFact {
	// 通过 IZinxTcpConnFact 继承
	virtual ZinxTcpData * CreateTcpDataChannel(int _fd) override;
};

