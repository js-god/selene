// This file is part of the `Selene` library.
// Copyright 2017 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#include <selene/img/PixelFormat.hpp>

namespace selene {
namespace img {

std::ostream& operator <<(std::ostream& os, PixelFormat pixel_format)
{
  switch (pixel_format)
  {
    case PixelFormat::Y: os << "PixelFormat::Y"; break;
    case PixelFormat::X: os << "PixelFormat::X"; break;

    case PixelFormat::YA: os << "PixelFormat::YA"; break;
    case PixelFormat::XX: os << "PixelFormat::XX"; break;

    case PixelFormat::RGB: os << "PixelFormat::RGB"; break;
    case PixelFormat::BGR: os << "PixelFormat::BGR"; break;
    case PixelFormat::YCbCr: os << "PixelFormat::YCbCr"; break;
    case PixelFormat::CIELab: os << "PixelFormat::CIELab"; break;
    case PixelFormat::ICCLab: os << "PixelFormat::ICCLab"; break;
    case PixelFormat::XXX: os << "PixelFormat::XXX"; break;

    case PixelFormat::RGBA: os << "PixelFormat::RGBA"; break;
    case PixelFormat::BGRA: os << "PixelFormat::BGRA"; break;
    case PixelFormat::ARGB: os << "PixelFormat::ARGB"; break;
    case PixelFormat::ABGR: os << "PixelFormat::ABGR"; break;
    case PixelFormat::CMYK: os << "PixelFormat::CMYK"; break;
    case PixelFormat::YCCK: os << "PixelFormat::YCCK"; break;
    case PixelFormat::XXXX: os << "PixelFormat::XXXX"; break;

    case PixelFormat::Unknown: os << "PixelFormat::Unknown"; break;
  }

  return os;
}

std::ostream& operator <<(std::ostream& os, SampleType sample_type)
{
  switch (sample_type)
  {
    case SampleType::UnsignedInteger: os << "SampleType::UnsignedInteger"; break;
    case SampleType::SignedInteger: os << "SampleType::SignedInteger"; break;
    case SampleType::FloatingPoint: os << "SampleType::FloatingPoint"; break;
    case SampleType::Unknown: os << "SampleType::Unknown"; break;
  }

  return os;
}

} // namespace img
} // namespace selene