#include "iostream"
#include "thread_pool.hpp"
#include <cassert>

void* operator new ( size_t size, const char* filename, int line )
{
    assert( false );
    void* ptr = new char[size];
    return ptr;
}

int main( void )
{
    {
        ThreadPool thread_pool;
        for ( size_t i = 0; i < 10; i++ )
            thread_pool.AddTask( [i]()
        {
            std::cout << "hello" << i << std::endl;
        } );
        std::cout << "End will be in destructor";
    }
    std::cout << "\n\n\n";
    {
        ThreadPool thread_pool;
        std::vector<std::future<void>> futures;
        for ( size_t i = 0; i < 10; i++ )
        {
            std::packaged_task<void( void )> task( []()
            {
                std::cout << "hello future" << std::endl;
            } );
            futures.push_back( task.get_future() );
            thread_pool.AddTask( task );
        }
        for ( auto& future : futures )
            future.get();
        std::cout << "\n future end \n";
    }
    return 0;
}
