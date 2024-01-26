#pragma once
#include "renderer/webgpu.hpp"

namespace bubble
{

class CommandEncoder : public wgpu::CommandEncoder
{
public:
    CommandEncoder( wgpu::CommandEncoder&& commandEncoder )
        : wgpu::CommandEncoder( commandEncoder )
    {}
};

}
