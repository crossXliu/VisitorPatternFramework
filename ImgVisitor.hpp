#pragma once
#include <type_traits>
#include "ImgTypes.hpp"
#include "CudaKernelInvoker.hpp"

namespace cuda_image {
/// visitor in visitor design pattern
class IImgVistor;

/// concrete img visitor type
template <typename T>
class ImgVisitor;


/// all concrete Image types should overload the visit function
class IImgVistor {
public:
  virtual void visit(Nv12Img const& src) noexcept = 0;
  virtual void visit(Yuv422Img const& src) noexcept = 0;
  virtual void visit(BGRImg const& src) noexcept = 0;
  virtual void visit(BGRAImg const& src) noexcept = 0;
  virtual TagType getVirtualTag() noexcept = 0;
};

template <typename T> class ImgVisitor : public IImgVistor {
public:
  void visit(Nv12Img const &src) noexcept override {
    using SrcType = std::decay_t<decltype(src)>;
    CudaKernelInvoker<SrcType, T> functor;
    return functor(src, m_dst, m_conf);
  }

  void visit(Yuv422Img const &src) noexcept override {
    using SrcType = std::decay_t<decltype(src)>;
    CudaKernelInvoker<SrcType, T> functor;
    return functor(src, m_dst, m_conf);
  }

  void visit(BGRImg const &src) noexcept override {
    using SrcType = std::decay_t<decltype(src)>;
    CudaKernelInvoker<SrcType, T> functor;
    return functor(src, m_dst, m_conf);
  }

  void visit(BGRAImg const &src) noexcept override {
    using SrcType = std::decay_t<decltype(src)>;
    CudaKernelInvoker<SrcType, T> functor;
    return functor(src, m_dst, m_conf);
  }

  constexpr auto getTag() const noexcept { return T{}.getTag(); }

  TagType getVirtualTag() noexcept override { return getTag(); }

private:
  T m_dst;
  ExtConf m_conf;
};

namespace internal {
template<typename, typename T>
class has_visit_xxximg_function;

template<typename C, typename Ret, typename... Args>
class has_visit_xxximg_function<C, Ret(Args...)> {
private:
  template <typename T>
  static constexpr auto check(std::nullptr_t) ->
      typename std::is_same<
          decltype(std::declval<C>().visit(std::declval<Args>()...)), void>;

  template <typename> static constexpr std::false_type check(...);

public:
    using RetType = Ret;
    using value_type = decltype(check<C>(nullptr));
};

/// a static checker for examining if the concrete visitor overload `visit`
/// function with all ImageTypes as its parameters
template <typename T> struct ImgVisitorStaticChecker {
  using TImg = std::decay_t<T>;
  using TcheckerImpl = has_visit_xxximg_function<IImgVistor, void(TImg const &)>;
  static_assert(TcheckerImpl::value_type::value == true,
                "\033[31m" "[fatal][!!!!!!!!]some ImageTypes has not been supported by "
                "IImgVistor and corresponding Derived implementation, please "
                "add it if you have append the ImageTypes enum before\n" "\033[0m");
};

/// a pesudo assert to trigger above static checker
static_assert(std::tuple_size_v<decltype(ImgTypesTpl<ImgVisitorStaticChecker>::value)> > 0);
}
} // namespace cuda_image
