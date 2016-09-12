//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.

#ifndef __SAMPLE_H__
#define __SAMPLE_H__


// This definition enables the Android extensions
#define VK_USE_PLATFORM_ANDROID_KHR

// This definition allows prototypes of Vulkan API functions,
// rather than dynamically loading entrypoints to the API manually.
#define VK_PROTOTYPES

#include <RHI/IRHI.h>
#include <assert.h>
#include <stdlib.h>
//#include <vulkan/vulkan.h>
//#include <vulkan/vulkan.hpp>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SampleRHI", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SampleRHI", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SampleRHI", __VA_ARGS__))


// Helper define for tracking error locations in code
#define VK_CHECK(X) if (!(X)) { LOGE("VK_CHECK Failure"); assert((X));}

//struct DepthBuffer {
//    vk::Format format;
//    vk::Image image;
//    vk::DeviceMemory mem;
//    vk::ImageView view;
//};
//
//struct SwapchainBuffer
//{
//    vk::Image image;
//    vk::CommandBuffer cmdBuffer;
//    vk::ImageView view;
//};
//
//struct Vertices {
//    vk::Buffer buf;
//    vk::DeviceMemory mem;
//    vk::PipelineVertexInputStateCreateInfo vi;
//    vk::VertexInputBindingDescription      vi_bindings[1];
//    vk::VertexInputAttributeDescription    vi_attrs[2];
//};

class SampleRHI
{
public:
    SampleRHI();
    ~SampleRHI();

    bool Initialize(ANativeWindow* window);

    void DrawFrame();
    
    bool IsInitialized() { return m_initialized; }
    
    bool TearDown();
protected:

    //k3d::IShaderCompilerOutput * compile(const char *shaderPath, rhi::EShaderType const& type);
//    bool CreateInstance();
//    bool GetPhysicalDevices();
//    void InitSurface();
//    void InitDevice();
//    void InitSwapchain();
//    void InitCommandbuffers();
//    void InitSync();
//    void InitVertexBuffers();
//    void InitLayouts();
//    void InitRenderPass();
//    void InitPipeline();
//    void InitFrameBuffers();
//
//    void BuildCmdBuffer();
//    void SetNextBackBuffer();
//    void PresentBackBuffer();
//
//    vk::ShaderModule CreateShaderModule(const uint32_t* code, uint32_t size);
//    bool GetMemoryTypeFromProperties( uint32_t typeBits, vk::MemoryPropertyFlags requirements_mask, uint32_t* typeIndex);

    ANativeWindow* m_androidWindow;

//    vk::Instance instance;
//    vk::PhysicalDevice physicalDevice;
//    vk::PhysicalDeviceProperties physicalDeviceProps;
//    vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProps;

//    vk::Device device;
//    vk::Queue queue;
//
//    vk::SurfaceKHR surface;
//    vk::SurfaceFormatKHR surfaceFormat;
//    vk::SwapchainKHR swapChain;
//
//    vk::Semaphore backBufferSemaphore;
//    vk::Semaphore renderCompleteSemaphore;
//
//    vk::CommandPool commandPool;
//
//    std::vector<vk::Framebuffer> framebuffers;
//
//    vk::DescriptorSetLayout descriptorLayout;
//    vk::PipelineLayout pipelineLayout;
//    vk::RenderPass renderPass;
//    vk::Pipeline pipeline;
//
//    uint32_t m_queueFamilyIndex;
//
//    SwapchainBuffer* m_swapchainBuffers;
//    uint32_t m_swapchainCurrentIdx;
//    uint32_t m_height;
//    uint32_t m_width;
//    uint32_t m_swapchainImageCount;
//
//    DepthBuffer* m_depthBuffers;
//
//    Vertices m_vertices;


    void PreparePipeline();

    uint64_t GetTime();
    uint64_t m_frameTimeBegin;
    uint64_t m_frameTimeEnd;
    uint64_t m_frameIdx;
    
    bool m_initialized;
    bool m_initBegun;

    rhi::IDevice *			m_pDevice;
    rhi::IRenderViewport *	m_RenderVp;

    rhi::IPipelineStateObject*			m_pPso;
    rhi::IPipelineLayout*				m_pl;
    std::vector<rhi::ICommandContext*>	m_Cmds;
};

#endif
