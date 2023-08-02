#include "metal/add_wrapper.hpp"
#include <chrono>
#include <iostream>


int main(int argc, const char* argv[])
{
    constexpr int        num_elements = 100000000;
    NS::AutoreleasePool* pPool        = NS::AutoreleasePool::alloc()->init();
    MTL::Device*         pDevice      = MTL::CreateSystemDefaultDevice();

    // Create the custom object used to encapsulate the Metal code.
    // Initializes objects to communicate with the GPU.
    AddCompute* computer = new AddCompute();
    computer->initWithDevice(pDevice);

    // Create buffers to hold data
    computer->prepareData(num_elements);

    // Time the compute phase.
    auto start = std::chrono::steady_clock::now();

    // Send a command to the GPU to perform the calculation.
    computer->sendComputeCommand();

    // End of compute phase.
    auto end        = std::chrono::steady_clock::now();
    auto delta_time = end - start;

    delete computer;
    computer = nullptr;
    pPool->release();

    std::cout << "Metal Computation completed in "
              << std::chrono::duration<double, std::milli>(delta_time).count()
              << " ms for array of size " << num_elements << ".\n";

    float* a_buf = new float[num_elements];
    float* b_buf = new float[num_elements];
    float* c_buf = new float[num_elements];
    start        = std::chrono::steady_clock::now();
    for (int i = 0; i < num_elements; i++)
    {
        c_buf[i] = a_buf[i] + b_buf[i];
    }
    end        = std::chrono::steady_clock::now();
    delta_time = end - start;
    std::cout << "CPU Computation completed in "
              << std::chrono::duration<double, std::milli>(delta_time).count()
              << " ms for array of size " << num_elements << ".\n";

    auto* p = &c_buf[100];
    std::cout << *p << std::endl;
    delete[] a_buf;
    delete[] b_buf;
    delete[] c_buf;

    return 0;
}