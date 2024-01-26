#pragma once 
#include <glfw3webgpu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>

#include "renderer/webgpu.hpp"

namespace bubble
{

class Window
{
public:
    Window()
    {
        if ( !glfwInit() )
            throw std::runtime_error( "Could not initialize GLFW!" );

        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
        glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
        mWindow = glfwCreateWindow( 640, 480, "Learn WebGPU", NULL, NULL );
        if ( !mWindow )
            throw std::runtime_error( "Could not open window!" );
    }

    wgpu::Surface GetSurface( const wgpu::Instance& instance ) const
    {
        return glfwGetWGPUSurface( instance, mWindow );
    }

    ~Window()
    {
        glfwDestroyWindow( mWindow );
        glfwTerminate();
    }

//private:
    GLFWwindow* mWindow;
};

}
