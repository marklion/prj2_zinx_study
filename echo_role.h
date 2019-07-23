#pragma once
#include <zinx.h>
class echo_role :
	public Irole
{
public:
	echo_role();
	virtual ~echo_role();

	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;
};

class exit_framework_role :public Irole {
	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;
};

class output_mng_role :public Irole {
	Ichannel *m_channle = NULL;
	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;
};

