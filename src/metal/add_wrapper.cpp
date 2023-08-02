#include "metal/add_wrapper.hpp"
#include <iostream>

void AddCompute::initWithDevice(MTL::Device* device)
{
    mDevice = device;
    NS::Error* error;

    // auto defaultLibrary = mDevice->newDefaultLibrary();
    NS::String* filePath       = NS::String::string("compute.metallib", NS::UTF8StringEncoding);
    auto        defaultLibrary = mDevice->newLibrary(filePath, &error);

    if (!defaultLibrary)
    {
        std::cerr << "Failed to find the default library.\n";
        exit(-1);
    }

    auto functionName    = NS::String::string("work_on_arrays", NS::ASCIIStringEncoding);
    auto computeFunction = defaultLibrary->newFunction(functionName);

    if (!computeFunction)
    {
        std::cerr << "Failed to find the compute function.\n";
    }

    mComputeFunctionPSO = mDevice->newComputePipelineState(computeFunction, &error);

    if (!computeFunction)
    {
        std::cerr << "Failed to create the pipeline state object.\n";
        exit(-1);
    }

    mCommandQueue = mDevice->newCommandQueue();

    if (!mCommandQueue)
    {
        std::cerr << "Failed to find command queue.\n";
        exit(-1);
    }
}

void AddCompute::prepareData(int num_elements)
{
    const int BUFFER_SIZE = num_elements * sizeof(float);
    // Allocate three buffers to hold our initial data and the result.
    mBufferA      = mDevice->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    mBufferB      = mDevice->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    mBufferResult = mDevice->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);

    generateRandomFloatData(mBufferA, num_elements);
    generateRandomFloatData(mBufferB, num_elements);
}

void AddCompute::generateRandomFloatData(MTL::Buffer* buffer, int num_elements)
{
    mNumElements   = num_elements;
    float* dataPtr = (float*)buffer->contents();

    for (unsigned long int index = 0; index < num_elements; index++)
        dataPtr[index] = float(rand()) / float(RAND_MAX);
}

void AddCompute::sendComputeCommand()
{
    // Create a command buffer to hold commands.
    MTL::CommandBuffer* commandBuffer = mCommandQueue->commandBuffer();
    assert(commandBuffer != nullptr);

    // Start a compute pass.
    MTL::ComputeCommandEncoder* computeEncoder = commandBuffer->computeCommandEncoder();
    assert(computeEncoder != nullptr);

    encodeComputeCommand(computeEncoder, mNumElements);

    // End the compute pass.
    computeEncoder->endEncoding();

    // Execute the command.
    commandBuffer->commit();

    // Normally, you want to do other work in your app while the GPU is running,
    // but in this example, the code simply blocks until the calculation is complete.
    commandBuffer->waitUntilCompleted();
}

void AddCompute::encodeComputeCommand(MTL::ComputeCommandEncoder* computeEncoder, int num_elements)
{
    // Encode the pipeline state object and its parameters.
    computeEncoder->setComputePipelineState(mComputeFunctionPSO);
    computeEncoder->setBuffer(mBufferA, 0, 0);
    computeEncoder->setBuffer(mBufferB, 0, 1);
    computeEncoder->setBuffer(mBufferResult, 0, 2);

    MTL::Size gridSize = MTL::Size(num_elements, 1, 1);

    // Calculate a threadgroup size.
    NS::UInteger threadGroupSize = mComputeFunctionPSO->maxTotalThreadsPerThreadgroup();
    if (threadGroupSize > num_elements)
    {
        threadGroupSize = num_elements;
    }
    MTL::Size threadgroupSize = MTL::Size(threadGroupSize, 1, 1);

    // Encode the compute command.
    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
}