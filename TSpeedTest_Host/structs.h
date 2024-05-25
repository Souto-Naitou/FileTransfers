#pragma once
#include <thread>

enum class ThreadStatus
{
	DEAD,
	WAIT_JOIN,
	LIVE
};

enum class FunctionStatus
{
	EXIT,
	IN_PROGRESS,
	WAIT
};

struct ThreadData
{
	std::thread		thread;
	ThreadStatus	status;
};