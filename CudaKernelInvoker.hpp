#pragma once

/// a fake invoker here for gtest in x86
/// @TODO implement the real invoker in cuda file
struct ExtConf {
};

template <typename TSrc, typename TDst>
struct CudaKernelInvoker {
    void operator() (TSrc const& src, TDst& dst, ExtConf conf) noexcept
    {
        
    }
};
