#include <gtest/gtest.h>
#include "ImgFactory.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(factory, basic)
{
    const auto& tag = cuda_image::Nv12Img{}.getTag();
    auto* imgElement = cuda_image::ImgFactory::CreateSrcImgFromTag(tag);
    std::cout << imgElement->getVirtualTag() << "\n";

    const auto& dstTag = cuda_image::BGRImg{}.getTag();
    auto* dstVisitor = cuda_image::ImgFactory::CreateDstImgFromTag(dstTag);
    std::cout << dstVisitor->getVirtualTag() << "\n";

    imgElement->accept(*dstVisitor);
}
