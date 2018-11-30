/* Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <memory>

#include "tensorrt/playground/utils.h"

namespace yais
{
/**
 * @brief Abstract base Memory class
 *
 * Abstract base class that tracks the base pointer and size of a memory allocation.
 * Derived classes are expected to allocate memory and initialize the base pointer
 * ink their protected/private constructor.  Derived classes are also expected to
 * implement a static Deleter method responsible for freeing the memory allocation.
 *
 * Derived classes are expected to use protected or private constructors.  Memory
 * objects will only be created by Allocator factory functions which return either a
 * `shared_ptr` or `unique_ptr` of an Allocator object of MemoryType.
 *
 * Recommended alignment values are not directly used by the Memory or the Allocator
 * object; however, subsequent specializations like MemoryStacks can use the DefaultAlignment
 * to specialize their function specific to the MemoryType used.
 */
template<class MemoryType>
struct IMemory
{
    virtual void* Data() const = 0;
    virtual size_t Size() const = 0;
    virtual void Fill(char) = 0;
    virtual size_t DefaultAlignment() const = 0;
    virtual const std::string& Type() const = 0;
};

template<class MemoryType>
class Memory : public IMemory<MemoryType>
{
  public:
    // Memory(Memory&&);
    // Memory& operator=(Memory&& other);

    DELETE_COPYABILITY(Memory);

    virtual ~Memory() {}

    inline void* Data() const final override;
    inline size_t Size() const final override;

  protected:
    Memory(void* ptr, size_t size) : m_MemoryAddress(ptr), m_BytesAllocated(size) {}

  private:
    void* m_MemoryAddress;
    size_t m_BytesAllocated;
};

struct AllocatableMemory
{
    virtual void* Allocate(size_t) = 0;
    virtual void Free() = 0;
};

class HostMemory : public Memory<HostMemory>
{
  public:
    using Memory<HostMemory>::Memory;
    using GenericMemoryType = HostMemory;
    void Fill(char) override;
    size_t DefaultAlignment() const override;
    const std::string& Type() const override;
    static std::shared_ptr<HostMemory> UnsafeWrapRawPointer(void*, size_t,
                                                            std::function<void(HostMemory*)>);
};

class SystemMallocMemory : public HostMemory, public AllocatableMemory
{
  protected:
    using HostMemory::HostMemory;
    void* Allocate(size_t) final override;
    void Free() final override;
    const std::string& Type() const final override;
};

} // end namespace yais

#include "tensorrt/playground/impl/memory.h"