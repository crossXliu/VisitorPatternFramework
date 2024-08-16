#include "ImgTypes.hpp"
#include "ImgBase.hpp"
#include "ImgVisitor.hpp"
#include <functional>
#include <iostream>

namespace cuda_image {

using std::function;

namespace detail {
/// just loop all types in ImgTypesAll tuple
template <typename ImgTpl>
class ImgTypesLoop;

/// these looper will instantiate all ImageTypes with Element pattern or Visitor
/// pattern
using ImgElementLooper = ImgTypesLoop<decltype(ImgTypesTpl<ImgElement>::value)>;
using ImgVisitorLooper = ImgTypesLoop<decltype(ImgTypesTpl<ImgVisitor>::value)>;

template <typename ImgTpl>
class ImgTypesLoop {
public:
  static constexpr inline auto ImgTypesCount =
      std::tuple_size_v<ImgTpl>;

  template <typename F>
  static void loop(F&& f, TagType const &tag) {
    return loopInteral<F, ImgTpl, 0>(std::forward<F>(f), tag);
  }

private:
  template <typename F, typename Tpl, std::size_t Idx,
            std::enable_if_t<(Idx >= ImgTypesCount), bool> = false>
  static void loopInteral(F&& func, TagType const &tag) {
    return;
  }

  template <typename F, typename Tpl, std::size_t Idx,
            std::enable_if_t<(Idx < ImgTypesCount), bool> = false>
  static void loopInteral(F&& func, TagType const &tag) {
    ImgTpl tpl{};
    constexpr auto trivialElement = decltype(std::get<Idx>(tpl)){};

    if (trivialElement.getTag() == tag) {
      func(trivialElement);
      return;
    } else {
      return loopInteral<F, Tpl, Idx + 1>(std::forward<F>(func), tag);
    }
  }
};
} // namespace detail

class ImgFactory {
public:
  static IImgElement *CreateSrcImgFromTag(TagType const &tag) {
    IImgElement* ptr{};
    /// expand the tuple
    auto creatFunc = [&ptr] (auto&& args) {
        std::cout << "actual type is " << args.getTag() << "\n";
        using ConcreteImgType = std::decay_t<decltype(args)>;
        auto* oriPtr = new ConcreteImgType();
        ptr = static_cast<IImgElement*>(oriPtr);
    };

    detail::ImgElementLooper::loop(creatFunc, tag);

    {
      std::array<char, sizeof("0xffffffffffffffff")> logStr{};
      snprintf(logStr.data(), logStr.max_size(), "%p\n", ptr);

      std::cout << logStr.data();
    }
    return ptr;
  }

  static IImgVistor *CreateDstImgFromTag(TagType const &tag) {
    IImgVistor* ptr{};
    /// expand the tuple
    auto creatFunc = [&ptr] (auto&& args) {
        std::cout << "actual type is " << args.getTag() << "\n";
        using ConcreteImgType = std::decay_t<decltype(args)>;
        auto* oriPtr = new ConcreteImgType();
        ptr = static_cast<IImgVistor*>(oriPtr);
    };

    detail::ImgVisitorLooper::loop(creatFunc, tag);

    {
      std::array<char, sizeof("0xffffffffffffffff")> logStr{};
      snprintf(logStr.data(), logStr.max_size(), "%p\n", ptr);

      std::cout << logStr.data();
    }
    return ptr;
  }
};
} // namespace cuda_image
