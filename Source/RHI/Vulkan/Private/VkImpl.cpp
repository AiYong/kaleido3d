#include "VkCommon.h"
#include "Public/VkRHI.h"
#include <cstdarg>

PFN_vklogCallBack g_logCallBack = nullptr;

void VkLog(k3d::LogLevel const& level, const char * tag, const char * fmt, ...) 
{
	if (g_logCallBack != nullptr)
	{
		va_list va;
		va_start(va, fmt);
		g_logCallBack(level, tag, fmt, va);
		va_end(va);
	}
}

void SetVkLogCallback(PFN_vklogCallBack func)
{
	g_logCallBack = func;
}