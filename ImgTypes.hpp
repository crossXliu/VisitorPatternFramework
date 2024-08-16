#pragma once
#include <tuple>

namespace cuda_image {

/// @TODO tag could be stringView type
using TagType = const char*;
using GaImage = void*;

/// all the image types supported by this framework should be put here
enum ImageTypes {
  NV12,
  YUV422,
  BGR,
  BGRA,
  MAX,
};

/// a helper the get tuples with pattern(element or visitor) of all image types
/// which has been registered by BIND_IMG_TRAITS macro
/// use ImgTypesTpl_v to get the tuple
template <template <typename> class Pattern>
struct ImgTypesTpl;

static_assert(
    ImageTypes::MAX == 4,
    "you may modify the image types enum, please make sure that every enum "
    "value has its corresponding element and visitor type, and change this assert\n");

template <ImageTypes Type>
struct ImgEnumBinding {};

#define BIND_IMG_TRAITS(EnumValue, Class)                                      \
  template <> struct ImgEnumBinding<EnumValue> {                               \
    using ImgCudaClass = Class;                                                \
  };

/// we don't care about the operations of GPU, so there is no need to implement
/// the member functions
struct Nv12Img {
  constexpr explicit Nv12Img() = default;
  constexpr TagType getTag() const { return "NV12"; }
};
BIND_IMG_TRAITS(NV12, Nv12Img);

struct Yuv422Img {
  constexpr explicit Yuv422Img() = default;
  constexpr TagType getTag() const { return "YUV422"; }
};
BIND_IMG_TRAITS(YUV422, Yuv422Img);

struct BGRImg {
  constexpr explicit BGRImg() = default;
  constexpr TagType getTag() const { return "BGR"; }
};
BIND_IMG_TRAITS(BGR, BGRImg);
struct BGRAImg {
  constexpr explicit BGRAImg() = default;
  constexpr TagType getTag() const { return "BGR"; }
};
BIND_IMG_TRAITS(BGRA, BGRAImg);

namespace internal {
/// edge condition for recursive
template <template <typename> class Pattern, ImageTypes Idx, std::enable_if_t<(Idx == ImageTypes::MAX - 1), bool> = false>
constexpr auto GetGeneralTupleFromEnum() {
    using PatternType = Pattern<typename ImgEnumBinding<Idx>::ImgCudaClass>;
    return std::make_tuple(PatternType{});
}

template <template <typename> class Pattern, ImageTypes Idx, std::enable_if_t<(Idx < ImageTypes::MAX - 1), bool> = false>
constexpr auto GetGeneralTupleFromEnum() {
    using PatternType = Pattern<typename ImgEnumBinding<Idx>::ImgCudaClass>;
    constexpr auto nextIdx = static_cast<ImageTypes>(Idx + 1);
    return std::tuple_cat(std::make_tuple(PatternType{}),
            GetGeneralTupleFromEnum<Pattern, nextIdx>());
}
}

template <template <typename> class Pattern> struct ImgTypesTpl {
  inline static constexpr auto value =
      internal::GetGeneralTupleFromEnum<Pattern, static_cast<ImageTypes>(0)>();
};
template <template <typename> class Pattern>
constexpr inline auto ImgTypesTpl_v = ImgTypesTpl<Pattern>::value;
} // namespace cuda_image
