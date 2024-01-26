#pragma once 
#include "renderer/webgpu.hpp"

namespace bubble
{

class Queue : public wgpu::Queue
{
public:
    Queue( wgpu::Queue&& queue )
        : wgpu::Queue( std::move( queue ) )
    {
    }

    ~Queue()
    {
        release();
    }

};

}
