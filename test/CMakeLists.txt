if(NOT IS_DIRECTORY ${SELENE_DIR})
    message(FATAL_ERROR "Please build using the outermost CMakeLists.txt file.")
endif()

#------------------------------------------------------------------------------

add_executable(selene_tests "")

target_sources(selene_tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/selene/Catch.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/Utils.hpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/base/Allocators.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/base/Bitcount.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/base/Kernel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/base/Round.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/base/_Utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/base/io/IO.cpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img/common/BoundingBox.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/common/PixelFormat.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/common/RowPointers.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/common/Types.cpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img/pixel/Pixel.cpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img/dynamic/DynImage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/dynamic/DynImageIterators.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/dynamic/UntypedLayout.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/dynamic/Utils.hpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/Image.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/ImageIterators.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/TypedLayout.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/Utilities.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/_Utils.hpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/access/BorderAccessors.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/access/GetPixel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/access/Interpolators.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/typed/access/RelativeAccessor.cpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img/interop/DynImageToImage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/interop/ImageToDynImage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img/interop/OpenCV.cpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img_io/IO.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_io/IO_JPEG.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_io/IO_PNG.cpp

        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Algorithms.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Allocate.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/ChannelOperations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Clone.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Convolution.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Crop.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Fill.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/ImageConversions.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/PixelConversions.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Resample.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/Transformations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/selene/img_ops/View.cpp
        )

target_compile_options(selene_tests PRIVATE
        ${SELENE_COMPILER_OPTIONS})

target_compile_definitions(selene_tests PRIVATE
        ${SELENE_COMPILER_DEFINITIONS})

target_include_directories(selene_tests PRIVATE
        ${SELENE_DIR}
        ${SELENE_DIR}/external/Catch2/single_include)

target_link_libraries(selene_tests
        selene
        selene_wrapper_fs)

if (OPENCV_CORE_FOUND)
    target_link_libraries(selene_tests opencv_core)
endif()
