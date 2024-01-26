#pragma once
#include "window/window.hpp"
#include "renderer/webgpu.hpp"

namespace bubble
{
class Instance : public wgpu::Instance
{
public:
    Instance()
        : wgpu::Instance( wgpu::createInstance( wgpu::InstanceDescriptor{} ) )
    {
        if ( !this->operator bool() )
            throw std::runtime_error( "Could not initialize WebGPU instance!" );
    }

    ~Instance()
    {
        release();
    }

    wgpu::Surface GetSurface( const Window& window )
    {
        return window.GetSurface( *this );
    }
};


}
