#include "Explode.CS.hlsli"
#include "../../RandomGenerator/RandomGenerator.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);

ConstantBuffer<PerFrame> gPerFrame : register(b0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    
    uint32_t particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {

        if (gParticles[particleIndex].color.a != 0)
        {
            gParticles[particleIndex].color.a -= 0.01f;
            float32_t3 velocity = gParticles[particleIndex].velocity;
            float32_t3 acceleration = float32_t3(0.0f, 0.0f, 0.0f);

            acceleration.y = -0.016f;

            velocity += acceleration;
            gParticles[particleIndex].velocity = velocity;

            gParticles[particleIndex].translate += velocity;
			
            gParticles[particleIndex].currentTime += gPerFrame.deltaTime;

            if (1.0f <= gParticles[particleIndex].currentTime * rcp(gParticles[particleIndex].lifeTime))
            {
                gParticles[particleIndex].color.a = 0.0f;
            }

            gParticles[particleIndex].rotate += gParticles[particleIndex].rotateVelocity;

            float32_t3 rotate = gParticles[particleIndex].rotate;
            float32_t4x4 rotateXMatrix = (float32_t4x4) 0;
            rotateXMatrix[0][0] = 1.0f;
            rotateXMatrix[1][1] = cos(rotate.x);
            rotateXMatrix[1][2] = sin(rotate.x);
            rotateXMatrix[2][1] = -sin(rotate.x);
            rotateXMatrix[2][2] = cos(rotate.x);
            rotateXMatrix[3][3] = 1.0f;

            float32_t4x4 rotateYMatrix = (float32_t4x4) 0;
            rotateYMatrix[0][0] = cos(rotate.y);
            rotateYMatrix[0][2] = -sin(rotate.y);
            rotateYMatrix[1][1] = 1.0f;
            rotateYMatrix[2][0] = sin(rotate.y);
            rotateYMatrix[2][2] = cos(rotate.y);
            rotateYMatrix[3][3] = 1.0f;

            float32_t4x4 rotateZMatrix = (float32_t4x4) 0;
            rotateZMatrix[0][0] = cos(rotate.z);
            rotateZMatrix[0][1] = sin(rotate.z);
            rotateZMatrix[1][0] = -sin(rotate.z);
            rotateZMatrix[1][1] = cos(rotate.z);
            rotateZMatrix[2][2] = 1.0f;
            rotateZMatrix[3][3] = 1.0f;

            gParticles[particleIndex].rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

        }
        else
        {

            gParticles[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
            int32_t freeListIndex;
            InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);

            if ((freeListIndex + 1) < kMaxParticles)
            {
                gFreeList[freeListIndex + 1] = particleIndex;
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            }

        }

    }

}