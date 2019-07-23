#pragma once
#include <zinx.h>
#include <string>
class cmd_msg :
	public UserData
{
public:
	cmd_msg();
	virtual ~cmd_msg();
	bool is_frame_exit = false;
	bool output_open = true;
	std::string echo_string;
};

