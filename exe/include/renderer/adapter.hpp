#pragma once
#include <iostream>
#include <vector>
#include <cassert>

#include "webgpu.hpp"
#include "window/window.hpp"
#include "renderer/instance.hpp"

namespace bubble
{

class Adapter : public wgpu::Adapter
{
public:
    Adapter( wgpu::Adapter&& adapter )
        : wgpu::Adapter( adapter )
    {}

    ~Adapter()
    {
        release();
    }

    void InspectAdapter()
    {
        size_t featureCount = wgpuAdapterEnumerateFeatures( *this, nullptr );
        std::vector<WGPUFeatureName> features( featureCount );
        wgpuAdapterEnumerateFeatures( *this, features.data() );

        std::cout << "Adapter features:" << std::endl;
        for ( auto f : features )
            std::cout << " - " << f << std::endl;

        WGPUSupportedLimits limits = {};
        limits.nextInChain = nullptr;
        bool success = wgpuAdapterGetLimits( *this, &limits );
        if ( success )
        {
            std::cout << "Adapter limits:" << std::endl;
            std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
            std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
            std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
            std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
            std::cout << " - maxBindGroups: " << limits.limits.maxBindGroups << std::endl;
            std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxSampledTexturesPerShaderStage: " << limits.limits.maxSampledTexturesPerShaderStage << std::endl;
            std::cout << " - maxSamplersPerShaderStage: " << limits.limits.maxSamplersPerShaderStage << std::endl;
            std::cout << " - maxStorageBuffersPerShaderStage: " << limits.limits.maxStorageBuffersPerShaderStage << std::endl;
            std::cout << " - maxStorageTexturesPerShaderStage: " << limits.limits.maxStorageTexturesPerShaderStage << std::endl;
            std::cout << " - maxUniformBuffersPerShaderStage: " << limits.limits.maxUniformBuffersPerShaderStage << std::endl;
            std::cout << " - maxUniformBufferBindingSize: " << limits.limits.maxUniformBufferBindingSize << std::endl;
            std::cout << " - maxStorageBufferBindingSize: " << limits.limits.maxStorageBufferBindingSize << std::endl;
            std::cout << " - minUniformBufferOffsetAlignment: " << limits.limits.minUniformBufferOffsetAlignment << std::endl;
            std::cout << " - minStorageBufferOffsetAlignment: " << limits.limits.minStorageBufferOffsetAlignment << std::endl;
            std::cout << " - maxVertexBuffers: " << limits.limits.maxVertexBuffers << std::endl;
            std::cout << " - maxVertexAttributes: " << limits.limits.maxVertexAttributes << std::endl;
            std::cout << " - maxVertexBufferArrayStride: " << limits.limits.maxVertexBufferArrayStride << std::endl;
            std::cout << " - maxInterStageShaderComponents: " << limits.limits.maxInterStageShaderComponents << std::endl;
            std::cout << " - maxComputeWorkgroupStorageSize: " << limits.limits.maxComputeWorkgroupStorageSize << std::endl;
            std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
            std::cout << " - maxComputeWorkgroupSizeX: " << limits.limits.maxComputeWorkgroupSizeX << std::endl;
            std::cout << " - maxComputeWorkgroupSizeY: " << limits.limits.maxComputeWorkgroupSizeY << std::endl;
            std::cout << " - maxComputeWorkgroupSizeZ: " << limits.limits.maxComputeWorkgroupSizeZ << std::endl;
            std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.limits.maxComputeWorkgroupsPerDimension << std::endl;
        }

        WGPUAdapterProperties properties = {};
        properties.nextInChain = nullptr;
        wgpuAdapterGetProperties( *this, &properties );
        std::cout << "Adapter properties:" << std::endl;
        std::cout << " - vendorID: " << properties.vendorID << std::endl;
        std::cout << " - deviceID: " << properties.deviceID << std::endl;
        std::cout << " - name: " << properties.name << std::endl;
        if ( properties.driverDescription )
            std::cout << " - driverDescription: " << properties.driverDescription << std::endl;
        std::cout << " - adapterType: " << properties.adapterType << std::endl;
        std::cout << " - backendType: " << properties.backendType << std::endl;
    }

};



Adapter RequestAdapter( Instance& insatnce,
                        wgpu::PowerPreference powerPerfomance,
                        wgpu::Surface& surface )
{
    wgpu::RequestAdapterOptions adapterOpts;
    adapterOpts.compatibleSurface = surface;
    adapterOpts.powerPreference = powerPerfomance;
    return insatnce.requestAdapter( adapterOpts );
}


}
