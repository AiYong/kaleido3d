#include "Kaleido3D.h"
#if K3DPLATFORM_OS_WIN
#include <RHI/D3D12/DXCommon.h>
#include <RHI/D3D12/Public/D3D12RHI.h>
#endif
#ifndef K3DPLATFORM_OS_MAC
#include <RHI/Vulkan/VkCommon.h>
#include <RHI/Vulkan/Public/VkRHI.h>
#endif
#include "Core/LogUtil.h"
#include "Render.h"

using namespace k3d;
using namespace v8;

namespace render
{
    const char * TAG_RENDERCONTEXT = "RenderContext";
    
	typedef void(*PFNRHIEnumFunc)(rhi::IDeviceAdapter**&,uint32*);

	PFNRHIEnumFunc s_RHIEnumFunction[] = {
		nullptr,
		reinterpret_cast<PFNRHIEnumFunc>(::k3d::vk::EnumAllDeviceAdapter),
#if K3DPLATFORM_OS_WIN
		reinterpret_cast<PFNRHIEnumFunc>(::k3d::d3d12::EnumAllDeviceAdapter)
#endif
	};


    RenderContext::RenderContext()
		: m_RenderVp(nullptr)
		, m_ScriptEngine(nullptr)
    {
    }
    
    void RenderContext::Init(RHIType type)
    {
		m_ScriptEngine = new ScriptEngine;
		m_RhiType = type;
        rhi::IDeviceAdapter ** adapters = nullptr;
        uint32 adapterCount = 0;
		if (type == RHIType::ERTVulkan)
		{
			InitializeVulkanRHI("Render_Vk", true);
		}
		s_RHIEnumFunction[static_cast<uint32>(type)](adapters, &adapterCount);
		m_pDevice = adapters[0]->GetDevice();
        rhi::IDevice::Result result = m_pDevice->Create(adapters[0], true);
        Log::Out(TAG_RENDERCONTEXT, result == rhi::IDevice::DeviceFound ?
                 "Device found !" : "Device unfound...");
    }

	void RenderContext::Attach(::k3d::IWindow::Ptr hostWindow)
	{
		if (!m_RenderVp)
		{
			rhi::GfxSetting setting{ 
				1920, 1080, rhi::EPF_RGBA8Unorm, 
				rhi::EPF_D32Float, true, 2 };
			m_RenderVp = m_pDevice->NewRenderViewport(hostWindow->GetHandle(), setting);
			m_RenderVp->InitViewport(nullptr, nullptr, setting);
		}
//		m_ScriptEngine->
	}

	void RenderContext::PrepareRenderingResource()
	{
	}

	void RenderContext::PreRender()
	{
	}

	void RenderContext::Render()
	{
		if (m_RenderVp)
		{
			m_RenderVp->PrepareNextFrame();
			m_RenderVp->Present(false);
		}
	}

	void RenderContext::PostRender()
	{
	}
    
    void RenderContext::Destroy()
    {
		if (m_RenderVp)
		{
			delete m_RenderVp;
			m_RenderVp = nullptr;
		}
		if (m_pDevice)
		{
			delete m_pDevice;
			m_pDevice = nullptr;
		}
		Log::Out(LogLevel::Info, "RenderContext", "Destroyed Render Context.");
		if (m_RhiType == RHIType::ERTVulkan)
		{
			DestroyVulkanRHI();
		}
		if (m_ScriptEngine) 
		{
			delete m_ScriptEngine;
			m_ScriptEngine = nullptr;
		}
    }
    
    RenderContext::~RenderContext()
    {
    }
}