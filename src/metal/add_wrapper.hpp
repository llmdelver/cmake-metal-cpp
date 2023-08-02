#pragma once

#include "platform/metal-cpp/Metal/Metal.hpp"

// const unsigned int ARRAY_LENGTH = 100000000;
// const unsigned int BUFFER_SIZE  = ARRAY_LENGTH * sizeof(float);

// const float DELTA = 1e-6;


class AddCompute final
{
public:
    void initWithDevice(MTL::Device*);
    void prepareData(int num_elements);
    void sendComputeCommand();

private:
    void encodeComputeCommand(MTL::ComputeCommandEncoder* encoder, int num_elements);
    void generateRandomFloatData(MTL::Buffer* buf, int num_elements);

private:
    MTL::Device* mDevice;

    // The compute pipeline generated from the compute kernel in the .metal shader file.
    MTL::ComputePipelineState* mComputeFunctionPSO;

    // The command queue used to pass commands to the device.
    MTL::CommandQueue* mCommandQueue;

    // Buffers to hold data.
    MTL::Buffer* mBufferA;
    MTL::Buffer* mBufferB;
    MTL::Buffer* mBufferResult;

    int mNumElements = 0;
};