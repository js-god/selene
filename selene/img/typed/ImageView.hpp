// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_IMG_TYPED_IMAGE_VIEW_HPP
#define SELENE_IMG_TYPED_IMAGE_VIEW_HPP

/// @file

#include <selene/img/common/DataPtr.hpp>

#include <selene/img/typed/ImageBase.hpp>
#include <selene/img/typed/ImageIterators.hpp>
#include <selene/img/typed/TypedLayout.hpp>

#include <selene/img/pixel/PixelTraits.hpp>

#include <cstring>

namespace sln {

/** \brief Statically typed image view class, i.e. non-owning.
 *
 * An instance of `ImageView<PixelType, modifiability>` represents a statically typed image view with pixel elements of
 * type `PixelType`.
 * Since the number of channels is determined by the pixel type (e.g. `Pixel<U, N>`), the storage of multiple
 * channels/samples is always interleaved, as opposed to planar.
 * Images are stored row-wise contiguous, with additional space after each row due to a custom stride in bytes.
 *
 * The memory of an `ImageView<PixelType>` instance is never owned by the instance.
 * To express an owning relation to the underlying data, use an `Image<PixelType>`.
 *
 * A view can either be created to point to constant data (`ImageModifiability::Constant`), or to modifiable (mutable)
 * data (`ImageModifiability::Mutable`); this is determined by the respective non-type template parameter.
 *
 * @tparam PixelType_ The pixel type. Usually of type `Pixel<>`.
 * @tparam modifiability_ Expresses whether the view contents can be modified (`ImageModifiability::Mutable`) or not
 *                        (`ImageModifiability::Constant`).
 */
template <typename PixelType_, ImageModifiability modifiability_>
class ImageView
    : public ImageBase<ImageView<PixelType_, modifiability_>>
{
public:
  using PixelType = PixelType_;
  using DataPtrType = typename DataPtr<modifiability_>::Type;
  using PixelTypePtr = std::conditional_t<modifiability_ == ImageModifiability::Constant, const PixelType*, PixelType*>;

  using iterator = ImageRowIterator<PixelType, modifiability_>;  ///< The iterator type.
  using const_iterator = ConstImageRowIterator<PixelType, modifiability_>;  ///< The const_iterator type.

  constexpr static bool is_view = impl::ImageBaseTraits<ImageView<PixelType_, modifiability_>>::is_view;
  constexpr static bool is_modifiable = impl::ImageBaseTraits<ImageView<PixelType_, modifiability_>>::is_modifiable;

  constexpr static ImageModifiability modifiability()
  {
    return impl::ImageBaseTraits<ImageView<PixelType_, modifiability_>>::modifiability();
  }

  ImageView() = default;
  ImageView(DataPtr<modifiability_> ptr, TypedLayout layout);

  const TypedLayout& layout() const noexcept;

  PixelLength width() const noexcept;
  PixelLength height() const noexcept;
  Stride stride_bytes() const noexcept;
  std::ptrdiff_t row_bytes() const noexcept;
  std::ptrdiff_t total_bytes() const noexcept;

  bool is_packed() const noexcept;
  bool is_empty() const noexcept;
  bool is_valid() const noexcept;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  DataPtrType byte_ptr() const noexcept;
  DataPtrType byte_ptr(PixelIndex y) const noexcept;
  DataPtrType byte_ptr(PixelIndex x, PixelIndex y) const noexcept;

  PixelTypePtr data() const noexcept;
  PixelTypePtr data(PixelIndex y) const noexcept;
  PixelTypePtr data_row_end(PixelIndex y) const noexcept;
  PixelTypePtr data(PixelIndex x, PixelIndex y) const noexcept;

  auto operator()(PixelIndex x, PixelIndex y) noexcept
      -> std::conditional_t<modifiability_ == ImageModifiability::Mutable, PixelType&, const PixelType&>;
  const PixelType& operator()(PixelIndex x, PixelIndex y) const noexcept;

  ImageView<PixelType, modifiability_>& view() noexcept;
  decltype(auto) view() const noexcept;
  decltype(auto) constant_view() const noexcept;

  void clear();

private:
  static_assert(std::is_trivial<PixelType>::value, "Pixel type is not trivial");
  static_assert(std::is_standard_layout<PixelType>::value, "Pixel type is not standard layout");

  DataPtr<modifiability_> ptr_;
  TypedLayout layout_;

  sln::Bytes compute_data_offset(PixelIndex y) const noexcept;
  sln::Bytes compute_data_offset(PixelIndex x, PixelIndex y) const noexcept;
};

template <typename PixelType> using MutableImageView = ImageView<PixelType, ImageModifiability::Mutable>;  ///< An image view pointing to mutable data.
template <typename PixelType> using ConstantImageView = ImageView<PixelType, ImageModifiability::Constant>;  ///< An image view pointing to constant data.

template <typename PixelType0, ImageModifiability modifiability_0,
    typename PixelType1, ImageModifiability modifiability_1>
bool equal(const ImageView<PixelType0, modifiability_0>& img_0, const ImageView<PixelType1, modifiability_1>& img_1);

// ----------
// Implementation:

/** \brief Constructs an image view onto the specified memory region, given the specified layout.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param ptr A pointer to the memory block representing the image data
 * @param layout The image layout.
 */
template <typename PixelType_, ImageModifiability modifiability_>
ImageView<PixelType_, modifiability_>::ImageView(DataPtr<modifiability_> ptr, TypedLayout layout)
    : ptr_(ptr), layout_(layout)
{
  // adjust stride_bytes (may have been set to 0 in TypedLayout constructor)
  layout_.stride_bytes = std::max(layout_.stride_bytes, Stride(PixelTraits<PixelType>::nr_bytes * layout_.width));
}

/** \brief Returns the image view layout.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return The typed image view layout.
 */
template <typename PixelType_, ImageModifiability modifiability_>
const TypedLayout& ImageView<PixelType_, modifiability_>::layout() const noexcept
{
  return layout_;
}

/** \brief Returns the image view width.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return The image view width.
 */
template <typename PixelType_, ImageModifiability modifiability_>
PixelLength ImageView<PixelType_, modifiability_>::width() const noexcept
{
  return layout_.width;
}

/** \brief Returns the image view height.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return The image view height.
 */
template <typename PixelType_, ImageModifiability modifiability_>
PixelLength ImageView<PixelType_, modifiability_>::height() const noexcept
{
  return layout_.height;
}

/** \brief Returns the row stride of the image view in bytes.
 *
 * The row stride is the number of bytes that a row occupies in memory.
 * It has to be greater or equal to the width times the size of a pixel element:
 * `(stride_bytes() >= width() * PixelTraits::nr_bytes)`.
 * If it is equal, then `is_packed()` returns `true`, otherwise `is_packed()` returns `false`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return The row stride of the image view in bytes.
 */
template <typename PixelType_, ImageModifiability modifiability_>
Stride ImageView<PixelType_, modifiability_>::stride_bytes() const noexcept
{
  return layout_.stride_bytes;
}

/** \brief Returns the number of data bytes occupied by each image row.
 *
 * The value returned is equal to `(width() * PixelTraits::nr_bytes)`.
 * It follows that `stride_bytes() >= row_bytes()`, since `stride_bytes()` may include additional padding bytes.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return The number of data bytes occupied by each image row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
std::ptrdiff_t ImageView<PixelType_, modifiability_>::row_bytes() const noexcept
{
  return layout_.row_bytes<PixelType>();
}

/** \brief Returns the total number of bytes occupied by the image data in memory.
 *
 * The value returned is equal to `(stride_bytes() * height())`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return The total number of bytes occupied by the image data in memory.
 */
template <typename PixelType_, ImageModifiability modifiability_>
std::ptrdiff_t ImageView<PixelType_, modifiability_>::total_bytes() const noexcept
{
  return layout_.total_bytes<PixelType>();
}

/** \brief Returns whether the image view is stored packed in memory.
 *
 * Returns the boolean expression `(stride_bytes() == width() * PixelTraits::nr_bytes)`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return True, if the image view data is stored packed; false otherwise.
 */
template <typename PixelType_, ImageModifiability modifiability_>
bool ImageView<PixelType_, modifiability_>::is_packed() const noexcept
{
  return layout_.is_packed<PixelType>();
}

/** \brief Returns whether the image view is empty.
 *
 * An image [view] is considered empty if its internal data pointer points to `nullptr`, `width() == 0`,
 * `height() == 0`, or any combination of these.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return True, if the image view is empty; false if it is non-empty.
 */
template <typename PixelType_, ImageModifiability modifiability_>
bool ImageView<PixelType_, modifiability_>::is_empty() const noexcept
{
  return ptr_.data() == nullptr || layout_.width == 0 || layout_.height == 0;
}

/** \brief Returns whether the image view is valid.
 *
 * Semantically equal to `!is_empty()`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return True, if the image view is valid; false otherwise.
 */
template <typename PixelType_, ImageModifiability modifiability_>
bool ImageView<PixelType_, modifiability_>::is_valid() const noexcept
{
  return !is_empty();
}

/** \brief Returns an iterator to the first row.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Iterator to the first image row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::begin() noexcept -> iterator
{
  return ImageRowIterator<PixelType, modifiability_>(ImageRow<PixelType, modifiability_>(this, 0_idx));
}

/** \brief Returns a constant iterator to the first row.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Constant iterator to the first image row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::begin() const noexcept -> const_iterator
{
  return ConstImageRowIterator<PixelType, modifiability_>(ConstImageRow<PixelType, modifiability_>(this, 0_idx));
}

/** \brief Returns a constant iterator to the first row.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Constant iterator to the first image row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::cbegin() const noexcept -> const_iterator
{
  return ConstImageRowIterator<PixelType, modifiability_>(ConstImageRow<PixelType, modifiability_>(this, 0_idx));
}

/** \brief Returns an iterator to the row after the last row of the image.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Iterator to the image row after the last row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::end() noexcept -> iterator
{
  return ImageRowIterator<PixelType, modifiability_>(
      ImageRow<PixelType, modifiability_>(this, PixelIndex{this->height()}));
}

/** \brief Returns a constant iterator to the row after the last row of the image.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Constant iterator to the image row after the last row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::end() const noexcept -> const_iterator
{
  return ConstImageRowIterator<PixelType, modifiability_>(
      ConstImageRow<PixelType, modifiability_>(this, PixelIndex{this->height()}));
}

/** \brief Returns a constant iterator to the row after the last row of the image.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Constant iterator to the image row after the last row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::cend() const noexcept -> const_iterator
{
  return ConstImageRowIterator<PixelType, modifiability_>(
      ConstImageRow<PixelType, modifiability_>(this, PixelIndex{this->height()}));
}

/** \brief Returns a pointer to the first byte storing image data (in row 0).
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Pointer to the first image data byte.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::byte_ptr() const noexcept -> DataPtrType
{
  return ptr_.data();
}

/** \brief Returns a pointer to the first byte storing image data in row `y`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param y Row index.
 * @return Pointer to the first image data byte of row `y`.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::byte_ptr(PixelIndex y) const noexcept -> DataPtrType
{
  return ptr_.data() + this->compute_data_offset(y);
}

/** \brief Returns a pointer to the first byte of the pixel element at location `(x, y)`, i.e. row `y`, column `x`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param x Column index.
 * @param y Row index.
 * @return Pointer to the first byte of the pixel element at location `(x, y)`.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::byte_ptr(PixelIndex x, PixelIndex y) const noexcept -> DataPtrType
{
  return ptr_.data() + this->compute_data_offset(x, y);
}

/** \brief Returns a pointer to the first pixel element (i.e. at row 0, column 0).
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Pointer to the first pixel element.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::data() const noexcept -> PixelTypePtr
{
  using VoidPtrType = std::conditional_t<modifiability_ == ImageModifiability::Constant, const void*, void*>;
  return static_cast<PixelTypePtr>(static_cast<VoidPtrType>(this->byte_ptr()));
}

/** \brief Returns a pointer to the first pixel element of the y-th row (i.e. at row y, column 0).
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param y Row index.
 * @return Pointer to the first pixel element of the y-th row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::data(PixelIndex y) const noexcept -> PixelTypePtr
{
  using VoidPtrType = std::conditional_t<modifiability_ == ImageModifiability::Constant, const void*, void*>;
  return static_cast<PixelTypePtr>(static_cast<VoidPtrType>(this->byte_ptr(y)));
}

/** \brief Returns a pointer to the one-past-the-last pixel element of the y-th row (i.e. at row y, column `width()`).
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param y Row index.
 * @return Pointer to the one-past-the-last pixel element of the y-th row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::data_row_end(PixelIndex y) const noexcept -> PixelTypePtr
{
  using VoidPtrType = std::conditional_t<modifiability_ == ImageModifiability::Constant, const void*, void*>;
  return static_cast<PixelTypePtr>(static_cast<VoidPtrType>(
      this->byte_ptr(y) + PixelTraits<PixelType>::nr_bytes * layout_.width));
}

/** \brief Returns a pointer to the x-th pixel element of the y-th row (i.e. at row y, column x).
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param x Column index.
 * @param y Row index.
 * @return Pointer to the x-th pixel element of the y-th row.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::data(PixelIndex x, PixelIndex y) const noexcept -> PixelTypePtr
{
  using VoidPtrType = std::conditional_t<modifiability_ == ImageModifiability::Constant, const void*, void*>;
  return static_cast<PixelTypePtr>(static_cast<VoidPtrType>(this->byte_ptr(x, y)));
}

/** \brief Returns a reference to the pixel element at location `(x, y)`, i.e. row `y`, column `x`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param x Column index.
 * @param y Row index.
 * @return Reference to the pixel element at location `(x, y)`.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::operator()(PixelIndex x, PixelIndex y) noexcept
    -> std::conditional_t<modifiability_ == ImageModifiability::Mutable, PixelType&, const PixelType&>
{
  return *this->data(x, y);
}

/** \brief Returns a constant reference to the pixel element at location `(x, y)`, i.e. row `y`, column `x`.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @param x Column index.
 * @param y Row index.
 * @return Constant reference to the pixel element at location `(x, y)`.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::operator()(PixelIndex x, PixelIndex y) const noexcept -> const PixelType&
{
  return *this->data(x, y);
}

/** \brief Returns the image view.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return Returns the image view itself; i.e. a reference to `this`.
 */
template <typename PixelType_, ImageModifiability modifiability_>
auto ImageView<PixelType_, modifiability_>::view() noexcept
    -> ImageView<PixelType, modifiability_>&
{
  return *this;
}

/** \brief Returns a constant image view.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return A constant image view.
 */
template <typename PixelType_, ImageModifiability modifiability_>
decltype(auto) ImageView<PixelType_, modifiability_>::view() const noexcept
{
  return constant_view();
}

/** \brief Returns a constant image view.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 * @return A constant image view.
 */
template <typename PixelType_, ImageModifiability modifiability_>
decltype(auto) ImageView<PixelType_, modifiability_>::constant_view() const noexcept
{
  if constexpr(is_modifiable)
  {
    return ImageView<PixelType, ImageModifiability::Constant>{this->byte_ptr(), this->layout()};
  }
  else
  {
    return static_cast<ImageView<PixelType, ImageModifiability::Constant>&>(*this);
  }
}

/** \brief Clears the image view; i.e. resets the internal state to the image view state after default construction.
 *
 * @tparam PixelType_ The pixel type.
 * @tparam modifiability_ Determines whether image contents are constant or mutable.
 */
template <typename PixelType_, ImageModifiability modifiability_>
void ImageView<PixelType_, modifiability_>::clear()
{
  ptr_ = DataPtr<modifiability_>{};
  layout_ = TypedLayout{};
}

template <typename PixelType_, ImageModifiability modifiability_>
sln::Bytes ImageView<PixelType_, modifiability_>::compute_data_offset(PixelIndex y) const noexcept
{
  return sln::Bytes{layout_.stride_bytes * y};
}

template <typename PixelType_, ImageModifiability modifiability_>
sln::Bytes ImageView<PixelType_, modifiability_>::compute_data_offset(PixelIndex x, PixelIndex y) const noexcept
{
  return sln::Bytes{layout_.stride_bytes * y + PixelTraits<PixelType>::nr_bytes * x};
}

template <typename PixelType0, ImageModifiability modifiability_0,
    typename PixelType1, ImageModifiability modifiability_1>
bool equal(const ImageView<PixelType0, modifiability_0>& img_0, const ImageView<PixelType1, modifiability_1>& img_1)
{
  // Underlying element type and nr of channels both have to match; the pixel format has to match at least in the
  // nr of channels, or be PixelFormat::Unknown in either source or target.
  static_assert(
      std::is_same<typename PixelTraits<PixelType0>::Element,
          typename PixelTraits<PixelType1>::Element>::value,
      "Incompatible pixel types for equality comparison");
  static_assert(
      PixelTraits<PixelType0>::nr_channels == PixelTraits<PixelType1>::nr_channels,
      "Incompatible pixel types for equality comparison");
  static_assert(
      get_nr_channels(PixelTraits<PixelType0>::pixel_format)
      == get_nr_channels(PixelTraits<PixelType1>::pixel_format)
      || PixelTraits<PixelType0>::pixel_format == PixelFormat::Unknown
      || PixelTraits<PixelType1>::pixel_format == PixelFormat::Unknown,
      "Incompatible pixel types for equality comparison");

  // The sizes should then also be the same
  static_assert(sizeof(PixelType0) == sizeof(PixelType1), "Incompatible pixel types for equality comparison");

  // Special case: if both images have a zero-length side, the shall be considered equal (both are invalid)
  if ((img_0.width() == 0 || img_0.height() == 0) && (img_1.width() == 0 || img_1.height() == 0))
  {
    return true;
  }

  if (img_0.width() != img_1.width() || img_0.height() != img_1.height())
  {
    return false;
  }

  for (auto y = 0_idx; y < img_0.height(); ++y)
  {
    const auto end0 = img_0.data_row_end(y);
    const auto begin0 = img_0.data(y);
    const auto begin1 = img_1.data(y);

    // std::equal may not be optimized to std::memcmp, even though we're dealing with a POD-type here...
    // const bool equal_row = std::equal(begin0, end0, begin1);
    // ...so let's just call std::memcmp directly:
    const auto nr_bytes = static_cast<std::size_t>(std::distance(begin0, end0)) * sizeof(PixelType0);
    const bool equal_row = (std::memcmp(begin0, begin1, nr_bytes) == 0);

    if (!equal_row)
    {
      return false;
    }
  }

  return true;
}

}  // namespace sln

#endif  // SELENE_IMG_TYPED_IMAGE_VIEW_HPP
