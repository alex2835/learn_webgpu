

#include <iostream>
#include <cassert>

#include "renderer/instance.hpp"
#include "renderer/adapter.hpp"
#include "renderer/device.hpp"
#include "renderer/queue.hpp"
#include "renderer/swap_chain.hpp"
#include "window/window.hpp"


int main( int, char** )
{
    bubble::Window window;

    bubble::Instance instance;
	auto surface = instance.GetSurface( window );
	bubble::Adapter adapter = instance.RequestAdapter( surface );
	bubble::Device device = adapter.RequestDevice();
	
	bubble::SwapChain swapChain = CreateSwapChain( device, surface );
	bubble::Queue queue = device.getQueue();



	std::cout << "Creating shader module..." << std::endl;
	const char* shader_source = R"(
	// The `@location(0)` attribute means that this input variable is described
	// by the vertex buffer layout at index 0 in the `pipelineDesc.vertex.buffers`
	// array.
	// The type `vec2f` must comply with what we will declare in the layout.
	// The argument name `in_vertex_position` is up to you, it is only internal to
	// the shader code!
	@vertex
	fn vs_main(@location(0) in_vertex_position: vec2f) -> @builtin(position) vec4f {
		return vec4f(in_vertex_position, 0.0, 1.0);
	}

	@fragment
	fn fs_main() -> @location(0) vec4f {
		return vec4f(0.0, 0.4, 1.0, 1.0);
	}
	)";
	auto pipeline = device.CreatePipeline( shader_source, wgpu::TextureFormat::BGRA8Unorm );



	// Vertex buffer
	// There are 2 floats per vertex, one for x and one for y.
	// But in the end this is just a bunch of floats to the eyes of the GPU,
	// the *layout* will tell how to interpret this.
	std::vector<float> vertexData = {
		-0.5, -0.5,
		+0.5, -0.5,
		+0.0, +0.5,

		-0.55f, -0.5,
		-0.05f, +0.5,
		-0.55f, +0.5
	};
	int vertexCount = static_cast<int>( vertexData.size() / 2 );

	// Create vertex buffer 
	wgpu::BufferDescriptor bufferDesc;
	bufferDesc.size = vertexData.size() * sizeof( float );
	bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
	bufferDesc.mappedAtCreation = false;
	wgpu::Buffer vertexBuffer = device.createBuffer( bufferDesc );


	// Upload geometry data to the buffer
	queue.writeBuffer( vertexBuffer, 0, vertexData.data(), bufferDesc.size );


	while ( !glfwWindowShouldClose( window.mWindow ) )
	{
		glfwPollEvents();

		wgpu::TextureView nextTexture = swapChain.getCurrentTextureView();
		if ( !nextTexture )
		{
			std::cerr << "Cannot acquire next swap chain texture" << std::endl;
			return 1;
		}

		wgpu::CommandEncoderDescriptor commandEncoderDesc;
		commandEncoderDesc.label = "Command Encoder";
		wgpu::CommandEncoder encoder = device.createCommandEncoder( commandEncoderDesc );

		wgpu::RenderPassDescriptor renderPassDesc;

		wgpu::RenderPassColorAttachment renderPassColorAttachment;
		renderPassColorAttachment.view = nextTexture;
		renderPassColorAttachment.resolveTarget = nullptr;
		renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
		renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
		renderPassColorAttachment.clearValue = wgpu::Color{ 0.9, 0.1, 0.2, 1.0 };
		renderPassDesc.colorAttachmentCount = 1;
		renderPassDesc.colorAttachments = &renderPassColorAttachment;

		renderPassDesc.depthStencilAttachment = nullptr;
		renderPassDesc.timestampWriteCount = 0;
		renderPassDesc.timestampWrites = nullptr;
		wgpu::RenderPassEncoder renderPass = encoder.beginRenderPass( renderPassDesc );

		renderPass.setPipeline( pipeline );

		// Set vertex buffer while encoding the render pass
		renderPass.setVertexBuffer( 0, vertexBuffer, 0, vertexData.size() * sizeof( float ) );

		// We use the `vertexCount` variable instead of hard-coding the vertex count
		renderPass.draw( vertexCount, 1, 0, 0 );

		renderPass.end();

		nextTexture.release();

		wgpu::CommandBufferDescriptor cmdBufferDescriptor;
		cmdBufferDescriptor.label = "Command buffer";
		wgpu::CommandBuffer command = encoder.finish( cmdBufferDescriptor );
		queue.submit( command );

		swapChain.present();

#ifdef WEBGPU_BACKEND_DAWN
		// Check for pending error callbacks
		device.tick();
#endif
	}

	//swapChain.release();
	//device.release();
	//adapter.release();
	//instance.release();

	return 0;
}
