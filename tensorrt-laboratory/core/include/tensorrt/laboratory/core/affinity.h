/* Copyright (c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
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
#include <map>

#include "cpuaff/cpuaff.hpp"

namespace trtlab {

struct CpuSet : public cpuaff::cpu_set
{
  public:
    using cpuaff::cpu_set::cpu_set;

    CpuSet Intersection(const CpuSet& other) const;
    CpuSet Union(const CpuSet& other) const;
    CpuSet Difference(const CpuSet& other) const;

    auto GetAllocator() const -> cpuaff::round_robin_allocator
    {
        return cpuaff::round_robin_allocator(*this);
    }

    std::string GetCpuString() const;
    std::string CoresDescription() const;

    std::map<cpuaff::socket_type, CpuSet> SocketMap();
};

struct Affinity
{
    static CpuSet GetAffinity();
    static void SetAffinity(const CpuSet& cpus);

    static CpuSet GetCpusByNuma(int numa_id);
    static CpuSet GetCpusBySocket(int socket_id);
    static CpuSet GetCpusByCore(int core_id);
    static CpuSet GetCpusByProcessingUnit(int thread_id);

    static CpuSet GetCpusFromString(const std::string& ids);
    static cpuaff::cpu GetCpuFromId(int);

    static int GetSocketCount();
};

} // namespace trtlab
