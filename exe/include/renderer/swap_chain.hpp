#pragma once
#include "renderer/webgpu.hpp"
#include "renderer/device.hpp"

namespace bubble
{

class SwapChain : public wgpu::SwapChain
{
public:
    SwapChain( wgpu::SwapChain&& swapChain )
        : wgpu::SwapChain( swapChain )
    {
    }

    ~SwapChain()
    {
        release();
    }

};

SwapChain CreateSwapChain( Device& device,
                           wgpu::Surface& surface )
{
    wgpu::SwapChainDescriptor swapChainDesc;
    swapChainDesc.width = 640;
    swapChainDesc.height = 480;
    swapChainDesc.usage = wgpu::TextureUsage::RenderAttachment;
    swapChainDesc.format = wgpu::TextureFormat::BGRA8Unorm;
    swapChainDesc.presentMode = wgpu::PresentMode::Fifo;
    return device.createSwapChain( surface, swapChainDesc );
}

}
