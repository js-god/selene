if(NOT IS_DIRECTORY ${SELENE_DIR})
    message(FATAL_ERROR "Please build using the outermost CMakeLists.txt file.")
endif()

#------------------------------------------------------------------------------

add_library(selene_base "")
add_library(selene::selene_base ALIAS selene_base)

target_sources(selene_base PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/base/Allocators.cpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Allocators.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Assert.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Bitcount.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Kernel.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/MemoryBlock.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/MessageLog.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Promote.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Round.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Types.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/Utils.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/_impl/ExplicitType.hpp
        )

target_compile_options(selene_base PRIVATE ${SELENE_COMPILER_OPTIONS})

target_compile_definitions(selene_base PRIVATE ${SELENE_COMPILER_DEFINITIONS})

if (SELENE_USE_DEFAULT_SINGLE_PRECISION)
    target_compile_definitions(selene_base PUBLIC SELENE_DEFAULT_SCALAR_SINGLE_PRECISION)
endif()

target_include_directories(selene_base PUBLIC
        $<BUILD_INTERFACE:${SELENE_DIR}>
        $<INSTALL_INTERFACE:include>)

#------------------------------------------------------------------------------

add_library(selene_base_io "")
add_library(selene::selene_base_io ALIAS selene_base_io)

target_sources(selene_base_io PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/base/io/FileReader.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/FileUtils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/FileUtils.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/FileWriter.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/MemoryReader.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/MemoryRegion.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/MemoryWriter.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/VectorReader.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/VectorWriter.hpp
        ${CMAKE_CURRENT_LIST_DIR}/base/io/WriterMode.hpp
        )

target_compile_options(selene_base_io PRIVATE ${SELENE_COMPILER_OPTIONS})

target_compile_definitions(selene_base_io PRIVATE ${SELENE_COMPILER_DEFINITIONS})

target_include_directories(selene_base_io PUBLIC
        $<BUILD_INTERFACE:${SELENE_DIR}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(selene_base_io PUBLIC selene_base)

#------------------------------------------------------------------------------

add_library(selene_img "")
add_library(selene::selene_img ALIAS selene_img)

target_sources(selene_img PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/img/common/BoundingBox.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/common/DataPtr.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/common/PixelFormat.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img/common/PixelFormat.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/common/Types.hpp

        ${CMAKE_CURRENT_LIST_DIR}/img/pixel/Pixel.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/pixel/PixelTraits.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/pixel/PixelTypeAliases.hpp

        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/DynImage.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/DynImageIterators.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/DynImageView.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/UntypedLayout.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/_impl/RuntimeChecks.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/_impl/StaticChecks.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/dynamic/_impl/Utils.hpp

        ${CMAKE_CURRENT_LIST_DIR}/img/typed/Image.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/ImageBase.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/ImageIterators.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/ImageTypeAliases.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/ImageView.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/ImageViewTypeAliases.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/TypedLayout.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/Utilities.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/_impl/ImageBaseTraits.hpp

        ${CMAKE_CURRENT_LIST_DIR}/img/typed/access/BorderAccessors.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/access/GetPixel.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/access/Interpolators.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/typed/access/RelativeAccessor.hpp

        ${CMAKE_CURRENT_LIST_DIR}/img/interop/DynImageToImage.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/interop/ImageToDynImage.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img/interop/OpenCV.hpp
        )

target_compile_options(selene_img PRIVATE ${SELENE_COMPILER_OPTIONS} ${SELENE_IMG_COMPILER_OPTIONS})

target_compile_definitions(selene_img PRIVATE ${SELENE_COMPILER_DEFINITIONS})

target_include_directories(selene_img PUBLIC
        $<BUILD_INTERFACE:${SELENE_DIR}>
        $<INSTALL_INTERFACE:include>)

if (OPENCV_CORE_FOUND)
    target_compile_definitions(selene_img PUBLIC SELENE_WITH_OPENCV)
    # no need to link against it, as it's only used in a header
endif()

target_link_libraries(selene_img PUBLIC selene_base)

#------------------------------------------------------------------------------

add_library(selene_img_io "")
add_library(selene::selene_img_io ALIAS selene_img_io)

target_sources(selene_img_io PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/img_io/IO.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/IO.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/JPEGCommon.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/JPEGRead.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/JPEGRead.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/JPEGWrite.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/JPEGWrite.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/PNGRead.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/PNGRead.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/PNGWrite.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/PNGWrite.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/_impl/JPEGCommon.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/_impl/JPEGDetail.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/_impl/JPEGDetail.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/_impl/PNGDetail.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/_impl/PNGDetail.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_io/_impl/Util.hpp
        )

target_compile_options(selene_img_io PRIVATE ${SELENE_COMPILER_OPTIONS} ${SELENE_IMG_COMPILER_OPTIONS})

target_compile_definitions(selene_img_io PRIVATE ${SELENE_COMPILER_DEFINITIONS})

target_include_directories(selene_img_io PUBLIC
        $<BUILD_INTERFACE:${SELENE_DIR}>
        $<INSTALL_INTERFACE:include>)

if (JPEG_FOUND)
    target_compile_definitions(selene_img_io PUBLIC SELENE_WITH_LIBJPEG)
    if (JPEG_PARTIAL_DECODING)
        target_compile_definitions(selene_img_io PUBLIC SELENE_LIBJPEG_PARTIAL_DECODING)
    endif()
    if (JPEG_EXTENDED_COLORSPACES)
        target_compile_definitions(selene_img_io PUBLIC SELENE_LIBJPEG_EXTENDED_COLORSPACES)
    endif()
    target_include_directories(selene_img_io PUBLIC ${JPEG_INCLUDE_DIR})
    target_link_libraries(selene_img_io PUBLIC ${JPEG_LIBRARIES})
    # Imported target support upcoming in CMake (commit 87f2cf3b1ce0ef7b8e71eae0f44cf9753660d423)
    #target_link_libraries(selene_img_io PUBLIC JPEG::JPEG)
endif()

if (PNG_FOUND)
    target_compile_definitions(selene_img_io PUBLIC SELENE_WITH_LIBPNG)
    target_link_libraries(selene_img_io PUBLIC PNG::PNG)
endif()

target_link_libraries(selene_img_io PUBLIC selene_base_io selene_img)

#------------------------------------------------------------------------------

add_library(selene_img_ops "")
add_library(selene::selene_img_ops ALIAS selene_img_ops)

target_sources(selene_img_ops PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Algorithms.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Allocate.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/ChannelOperations.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Clone.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Convolution.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Crop.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Fill.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/ImageConversions.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/PixelConversions.cpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/PixelConversions.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Resample.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/Transformations.hpp
        ${CMAKE_CURRENT_LIST_DIR}/img_ops/View.hpp
        )

target_compile_options(selene_img_ops PRIVATE ${SELENE_COMPILER_OPTIONS} ${SELENE_IMG_COMPILER_OPTIONS})

target_compile_definitions(selene_img_ops PRIVATE ${SELENE_COMPILER_DEFINITIONS})

target_include_directories(selene_img_ops PUBLIC
        $<BUILD_INTERFACE:${SELENE_DIR}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(selene_img_ops PUBLIC selene_img)

#------------------------------------------------------------------------------

add_library(selene INTERFACE)
add_library(selene::selene ALIAS selene)

target_include_directories(selene INTERFACE
        $<BUILD_INTERFACE:${SELENE_DIR}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(selene INTERFACE
        selene_base
        selene_base_io
        selene_img
        selene_img_io
        selene_img_ops)

#------------------------------------------------------------------------------

# Installation

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(SELENE_INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/selene)

# Create selene-config-version.cmake
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/selene-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

# Create selene-config.cmake
configure_package_config_file(
        ${SELENE_DIR}/cmake/selene-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/selene-config.cmake
        INSTALL_DESTINATION ${SELENE_INSTALL_CONFIGDIR})

# Install library files
install(TARGETS selene_base selene_base_io selene_img selene_img_io selene_img_ops selene
        EXPORT selene-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

# Install exports
install(EXPORT selene-targets
        FILE selene-targets.cmake
        NAMESPACE selene::
        DESTINATION ${SELENE_INSTALL_CONFIGDIR})

# Install config and config-version files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/selene-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/selene-config-version.cmake
        DESTINATION ${SELENE_INSTALL_CONFIGDIR})

#------------------------------------------------------------------------------

# Exporting from the build tree

export(EXPORT selene-targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/selene-targets.cmake
        NAMESPACE selene::)

export(PACKAGE selene)
