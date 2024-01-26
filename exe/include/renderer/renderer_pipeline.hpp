#pragma once
#include <string_view>
#include "renderer/webgpu.hpp"
#include "renderer/device.hpp"

namespace bubble
{

class RenderPipeline : public wgpu::RenderPipeline
{
public:

    RenderPipeline( wgpu::RenderPipeline&& pipeline )
        : wgpu::RenderPipeline( pipeline )
    {
    }

    ~RenderPipeline()
    {
        release();
    }

};


RenderPipeline CreatePipeline( Device& device,
                               std::string shader_source,
                               const wgpu::TextureFormat& attachment_format )
{
    wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc;
    shaderCodeDesc.chain.next = nullptr;
    shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
    shaderCodeDesc.code = shader_source.c_str();
    wgpu::ShaderModuleDescriptor shaderDesc;
    shaderDesc.nextInChain = &shaderCodeDesc.chain;
#ifdef WEBGPU_BACKEND_WGPU
    shaderDesc.hintCount = 0;
    shaderDesc.hints = nullptr;
#endif

    wgpu::ShaderModule shaderModule = device.createShaderModule( shaderDesc );
    std::cout << "Shader module: " << shaderModule << std::endl;

    std::cout << "Creating render pipeline..." << std::endl;
    wgpu::RenderPipelineDescriptor pipelineDesc;

    // Vertex fetch
    wgpu::VertexAttribute vertexAttrib;
    // == Per attribute ==
    // Corresponds to @location(...)
    vertexAttrib.shaderLocation = 0;
    // Means vec2<f32> in the shader
    vertexAttrib.format = wgpu::VertexFormat::Float32x2;
    // Index of the first element
    vertexAttrib.offset = 0;

    wgpu::VertexBufferLayout vertexBufferLayout;
    // [...] Build vertex buffer layout
    vertexBufferLayout.attributeCount = 1;
    vertexBufferLayout.attributes = &vertexAttrib;
    // == Common to attributes from the same buffer ==
    vertexBufferLayout.arrayStride = 2 * sizeof( float );
    vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;

    pipelineDesc.vertex.bufferCount = 1;
    pipelineDesc.vertex.buffers = &vertexBufferLayout;

    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = "vs_main";
    pipelineDesc.vertex.constantCount = 0;
    pipelineDesc.vertex.constants = nullptr;

    pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;
    pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;
    pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;
    pipelineDesc.primitive.cullMode = wgpu::CullMode::None;

    wgpu::FragmentState fragmentState;
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = "fs_main";
    fragmentState.constantCount = 0;
    fragmentState.constants = nullptr;

    wgpu::BlendState blendState;
    blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
    blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
    blendState.color.operation = wgpu::BlendOperation::Add;
    blendState.alpha.srcFactor = wgpu::BlendFactor::Zero;
    blendState.alpha.dstFactor = wgpu::BlendFactor::One;
    blendState.alpha.operation = wgpu::BlendOperation::Add;

    wgpu::ColorTargetState colorTarget;
    colorTarget.format = attachment_format;
    colorTarget.blend = &blendState;
    colorTarget.writeMask = wgpu::ColorWriteMask::All;

    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;

    pipelineDesc.fragment = &fragmentState;

    pipelineDesc.depthStencil = nullptr;

    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = ~0u;
    pipelineDesc.multisample.alphaToCoverageEnabled = false;

    wgpu::PipelineLayoutDescriptor layoutDesc;
    layoutDesc.bindGroupLayoutCount = 0;
    layoutDesc.bindGroupLayouts = nullptr;
    wgpu::PipelineLayout layout = createPipelineLayout( layoutDesc );
    pipelineDesc.layout = layout;

    return RenderPipeline( createRenderPipeline( pipelineDesc ) );
}

}
