#pragma once

#include <cstdint>
#include "ImgVisitor.hpp"

namespace cuda_image {

/// element in visitor design pattern
class IImgElement;

/// concrete img element type
template <typename T>
class ImgElement;

class IImgElement {
public:
  virtual void accept(IImgVistor& visitor) = 0;
  virtual TagType getVirtualTag() = 0;
  virtual bool isBufferAvaliable() = 0;
};

template <typename T> class ImgElement : public IImgElement {
public:
  constexpr explicit ImgElement<T>() = default;
  explicit ImgElement<T>(GaImage& img) {
      /// @TODO tr
  }
  bool isBufferAvaliable() override { return m_origin == nullptr; }
  void accept(IImgVistor &visitor) override { return visitor.visit(m_src); }

  constexpr auto getTag() const { return T{}.getTag(); }

  TagType getVirtualTag() override { return getTag(); }

private:
  T m_src;
  GaImage* m_origin{nullptr};
};
} // namespace cuda_image
