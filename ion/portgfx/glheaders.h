/**
Copyright 2016 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS-IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef ION_PORTGFX_GLHEADERS_H_
#define ION_PORTGFX_GLHEADERS_H_

// The GL header files in the opengl/ subdirectory may not be compatible with
// the system headers on the local host. Therefore, this file must be #included
// before anything that could bring in the system headers. We define a header
// guard that makes subsequent inclusions of those headers harmless.
#if defined(_GL_GL_H_)
#  error "ion/portgfx/glheaders.h must be included before system gl.h"
#else
#  define _GL_GL_H_
#endif

// Include OpenGL headers based on platform.

#if defined(ION_PLATFORM_MAC)
// Mac always has its own header files.
#  include <OpenGL/gl.h>
#  include <OpenGL/glext.h>

// From GL_ARB_debug_output
typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam);

#elif defined(ION_PLATFORM_IOS)
// IOS always has its own header files.
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>

// From GL_ARB_debug_output
typedef void (GL_APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id,
                                        GLenum severity, GLsizei length,
                                        const GLchar *message,
                                        const void *userParam);

#elif defined(ION_PLATFORM_ANDROID) || \
      defined(ION_PLATFORM_GENERIC_ARM) || \
      defined(ION_PLATFORM_ASMJS) || \
      defined(ION_PLATFORM_NACL) || \
      defined(ION_PLATFORM_QNX) || \
      (defined(ION_PLATFORM_LINUX) && defined(ION_GFX_OGLES20))
#  if !defined(ION_PLATFORM_NACL)
     // When using Linux with EGL, we don't want EGL to import X11 as it does by
     // default.  This can cause conflicts later on.
#    if (defined(ION_PLATFORM_LINUX) && defined(ION_GFX_OGLES20))
#      define MESA_EGL_NO_X11_HEADERS
#    endif

#    include <EGL/egl.h>  // NOLINT
#    include <EGL/eglext.h>  // NOLINT
#  endif

#  if (defined(ION_PLATFORM_LINUX) && defined(ION_GFX_OGLES20))
#    undef MESA_EGL_NO_X11_HEADERS
#  endif

#  if __ANDROID_API__ >= 18
#    include <GLES3/gl3.h>
#  else
#    include <GLES2/gl2.h>
#    include <GLES2/gl2ext.h>
#  endif

// From GL_ARB_debug_output
typedef void (GL_APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id,
                                        GLenum severity, GLsizei length,
                                        const GLchar *message,
                                        const void *userParam);

#else
#  if defined(ION_PLATFORM_WINDOWS)
// Use ANGLE since our build machines support only OpenGL 1.1, but have DirectX.
#    if defined(ION_ANGLE)
#      include <EGL/egl.h>  // NOLINT
#      include <EGL/eglext.h>  // NOLINT
#    endif
#    if defined(NOGDI)
#      undef NOGDI  // Need to get wgl functions from windows.h.
#    endif
#    include <windows.h>  // NOLINT
#  endif
#  if !defined(GL_GLEXT_PROTOTYPES)
#    define GL_GLEXT_PROTOTYPES  // For glGetString() to be defined.
#  endif
#  include "third_party/GL/gl/include/GL/glcorearb.h"
#  include "third_party/GL/gl/include/GL/glext.h"

// Prevent GLU from being included since it tries to redefine classes as
// structs.
#  define __glu_h__
#  define __gl_h_

#endif

#if defined(ION_PLATFORM_ASMJS) || defined(ION_GOOGLE_INTERNAL)
// Prevent some platforms from including their own gl.h headers.
#  define __gl_h_
#endif

#if defined(ION_PLATFORM_LINUX) && defined(ION_GFX_OGLES20)
  typedef double GLdouble;

#elif !defined(__glcorearb_h_) && \
      (!defined(ION_PLATFORM_ANDROID) || (__ANDROID_API__ <= 20))

// Define GLdouble, GLuint64, and GLsync which aren't in OpenGL ES 2.0 spec.
// ... but ARE defined in newer Android NDK platforms, notably the
// ones required for 64-bit Android (API 20).  It was added
// between API 18 and 20; we know 18 needs it and "L" needs it gone.
#  include <stdint.h>
  typedef double GLdouble;
  typedef int64_t GLint64;
  typedef uint64_t GLuint64;
  typedef struct __GLsync* GLsync;
#endif

// These constants are not always defined in OpenGL header files.

#ifndef GL_ALIASED_POINT_SIZE_RANGE
#  define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#endif
#ifndef GL_ALPHA_BITS
#  define GL_ALPHA_BITS 0x0D55
#endif
#ifndef GL_ALREADY_SIGNALED
#  define GL_ALREADY_SIGNALED 0x911A
#endif
#ifndef GL_BACK
#  define GL_BACK 0x0405
#endif
#ifndef GL_BACK_LEFT
#  define GL_BACK_LEFT 0x0402
#endif
#ifndef GL_BACK_RIGHT
#  define GL_BACK_RIGHT 0x0403
#endif
#ifndef GL_BLUE
#  define GL_BLUE 0x1905
#endif
#ifndef GL_BLUE_BITS
#  define GL_BLUE_BITS 0x0D54
#endif
#ifndef GL_BUFFER_MAP_POINTER
#define GL_BUFFER_MAP_POINTER 0x88BD
#endif
#ifndef GL_BUFFER_OBJECT
#  define GL_BUFFER_OBJECT 0x9151
#endif
#ifndef GL_COLOR_ATTACHMENT0
#  define GL_COLOR_ATTACHMENT0 0x8CE0
#endif
#ifndef GL_COMPARE_REF_TO_TEXTURE
#  define GL_COMPARE_REF_TO_TEXTURE 0x884E
#endif
#ifndef GL_COMPRESSED_R11_EAC
#  define GL_COMPRESSED_R11_EAC 0x9270
#endif
#ifndef GL_COMPRESSED_RG11_EAC
#  define GL_COMPRESSED_RG11_EAC 0x9272
#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#  define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#  define GL_COMPRESSED_RGB8_ETC2 0x9274
#endif
#ifndef GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2
#  define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8C01
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8C03
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#  define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#  define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#  define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#  define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#endif
#ifndef GL_COMPRESSED_SIGNED_R11_EAC
#  define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#endif
#ifndef GL_COMPRESSED_SIGNED_RG11_EAC
#  define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#endif
#ifndef GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC
#  define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#endif
#ifndef GL_COMPRESSED_SRGB8_ETC2
#  define GL_COMPRESSED_SRGB8_ETC2 0x9275
#endif
#ifndef GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2
#  define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#endif
#ifndef GL_CONDITION_SATISFED
#  define GL_CONDITION_SATISFIED 0x911C
#endif
#ifndef GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
#  define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x0002
#endif
#ifndef GL_CONTEXT_CORE_PROFILE_BIT
#  define GL_CONTEXT_CORE_PROFILE_BIT 0x0001
#endif
#ifndef GL_CONTEXT_PROFILE_MASK
#  define GL_CONTEXT_PROFILE_MASK 0x9126
#endif
#ifndef GL_COPY_READ_BUFFER
#  define GL_COPY_READ_BUFFER 0x8F36
#endif
#ifndef GL_COPY_WRITE_BUFFER
#  define GL_COPY_WRITE_BUFFER 0x8F37
#endif
#ifndef GL_CURRENT_QUERY_EXT
#  define GL_CURRENT_QUERY_EXT 0x8865
#endif
#ifndef GL_DRAW_BUFFER0
#  define GL_DRAW_BUFFER0 0x8825
#endif
#ifndef GL_DEBUG_CALLBACK_FUNCTION
#  define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#endif
#ifndef GL_DEBUG_CALLBACK_USER_PARAM
#  define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#endif
#ifndef GL_DEBUG_LOGGED_MESSAGES
#  define GL_DEBUG_LOGGED_MESSAGES 0x9145
#endif
#ifndef GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH
#  define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#endif
#ifndef GL_DEBUG_OUTPUT_SYNCHRONOUS
#  define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#endif
#ifndef GL_DEBUG_SEVERITY_LOW
#  define GL_DEBUG_SEVERITY_LOW 0x9148
#endif
#ifndef GL_DEBUG_SEVERITY_MEDIUM
#  define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#endif
#ifndef GL_DEBUG_SEVERITY_HIGH
#  define GL_DEBUG_SEVERITY_HIGH 0x9146
#endif
#ifndef GL_DEBUG_SOURCE_API
#  define GL_DEBUG_SOURCE_API 0x8246
#endif
#ifndef GL_DEBUG_SOURCE_APPLICATION
#  define GL_DEBUG_SOURCE_APPLICATION 0x824A
#endif
#ifndef GL_DEBUG_SOURCE_OTHER
#  define GL_DEBUG_SOURCE_OTHER 0x824B
#endif
#ifndef GL_DEBUG_SOURCE_SHADER_COMPILER
#  define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#endif
#ifndef GL_DEBUG_SOURCE_THIRD_PARTY
#  define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#endif
#ifndef GL_DEBUG_SOURCE_WINDOW_SYSTEM
#  define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#endif
#ifndef GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
#  define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#endif
#ifndef GL_DEBUG_TYPE_ERROR
#  define GL_DEBUG_TYPE_ERROR 0x824C
#endif
#ifndef GL_DEBUG_TYPE_OTHER
#  define GL_DEBUG_TYPE_OTHER 0x8251
#endif
#ifndef GL_DEBUG_TYPE_PERFORMANCE
#  define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#endif
#ifndef GL_DEBUG_TYPE_PORTABILITY
#  define GL_DEBUG_TYPE_PORTABILITY 0x824F
#endif
#ifndef GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
#  define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#endif
#ifndef GL_DEPTH_ATTACHMENT
#  define GL_DEPTH_ATTACHMENT 0x8D00
#endif
#ifndef GL_DEPTH24_STENCIL8
#  define GL_DEPTH24_STENCIL8 0x88F0
#endif
#ifndef GL_DEPTH32F_STENCIL8
#  define GL_DEPTH32F_STENCIL8 0x8CAD
#endif
#ifndef GL_DEPTH_BITS
#  define GL_DEPTH_BITS 0x0D56
#endif
#ifndef GL_DEPTH_BUFFER_BIT
#  define GL_DEPTH_BUFFER_BIT 0x00000100
#endif
#ifndef GL_DEPTH_COMPONENT24
#  define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT32F
#  define GL_DEPTH_COMPONENT32F 0x8CAC
#endif
#ifndef GL_DEPTH_STENCIL
#  define GL_DEPTH_STENCIL 0x84F9
#endif
#ifndef GL_DRAW_BUFFER
#  define GL_DRAW_BUFFER 0x0C01
#endif
#ifndef GL_DRAW_FRAMEBUFFER
#  define GL_DRAW_FRAMEBUFFER 0x8CA9
#endif
#ifndef GL_DRAW_FRAMEBUFFER_BINDING
#  define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#endif
#ifndef GL_ETC1_RGB8_OES
#  define GL_ETC1_RGB8_OES 0x8D64
#endif
#ifndef GL_FIXED
#  define GL_FIXED 0x140C
#endif
#ifndef GL_FLOAT_32_UNSIGNED_INT_24_8_REV
#  define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#endif
#ifndef GL_FRAMEBUFFER
#  define GL_FRAMEBUFFER 0x8D40
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE
#  define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME
#  define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER
#  define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL
#  define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE
#  define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#endif
#ifndef GL_FRAMEBUFFER_BINDING
#  define GL_FRAMEBUFFER_BINDING 0x8CA6
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#  define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#  define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
#  define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#endif
#ifndef GL_FRAMEBUFFER_UNSUPPORTED
#  define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#endif
#ifndef GL_FRONT
#  define GL_FRONT 0x0404
#endif
#ifndef GL_FRONT_AND_BACK
#  define GL_FRONT_AND_BACK 0x0408
#endif
#ifndef GL_FRONT_LEFT
#  define GL_FRONT_LEFT 0x0400
#endif
#ifndef GL_FRONT_RIGHT
#  define GL_FRONT_RIGHT 0x0401
#endif
#ifndef GL_GENERATE_MIPMAP_HINT
#  define GL_GENERATE_MIPMAP_HINT 0x8192
#endif
#ifndef GL_GPU_DISJOINT_EXT
#  define GL_GPU_DISJOINT_EXT 0x8FBB
#endif
#ifndef GL_GREEN
#  define GL_GREEN 0x1904
#endif
#ifndef GL_GREEN_BITS
#  define GL_GREEN_BITS 0x0D53
#endif
#ifndef GL_HALF_FLOAT
#  define GL_HALF_FLOAT 0x140B
#endif
#ifndef GL_HIGH_FLOAT
#  define GL_HIGH_FLOAT 0x8DF2
#endif
#ifndef GL_HIGH_INT
#  define GL_HIGH_INT 0x8DF5
#endif
#ifndef GL_IMPLEMENTATION_COLOR_READ_FORMAT
#  define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#endif
#ifndef GL_IMPLEMENTATION_COLOR_READ_TYPE
#  define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#endif
#ifndef GL_INT_SAMPLER_1D
#  define GL_INT_SAMPLER_1D 0x8DC9
#endif
#ifndef GL_INT_SAMPLER_2D
#  define GL_INT_SAMPLER_2D 0x8DCA
#endif
#ifndef GL_INT_SAMPLER_3D
#  define GL_INT_SAMPLER_3D 0x8DCB
#endif
#ifndef GL_INT_SAMPLER_CUBE
#  define GL_INT_SAMPLER_CUBE 0x8DCC
#endif
#ifndef GL_INT_SAMPLER_1D_ARRAY
#  define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#endif
#ifndef GL_INT_SAMPLER_2D_ARRAY
#  define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#endif
#ifndef GL_INT_SAMPLER_CUBE_MAP_ARRAY
#  define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#endif
#ifndef GL_INTERLEAVED_ATTRIBS
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#endif
#ifndef GL_INVALID_FRAMEBUFFER_OPERATION
#  define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#endif
#ifndef GL_LEFT
#  define GL_LEFT 0x0406
#endif
#ifndef GL_LOW_FLOAT
#  define GL_LOW_FLOAT 0x8DF0
#endif
#ifndef GL_LOW_INT
#  define GL_LOW_INT 0x8DF3
#endif
#ifndef GL_LUMINANCE
#  define GL_LUMINANCE 0x1909
#endif
#ifndef GL_LUMINANCE_ALPHA
#  define GL_LUMINANCE_ALPHA 0x190A
#endif
#ifndef GL_MAP_FLUSH_EXPLICIT_BIT
#  define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#endif
#ifndef GL_MAP_INVALIDATE_BUFFER_BIT
#  define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#endif
#ifndef GL_MAP_INVALIDATE_RANGE_BIT
#  define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#endif
#ifndef GL_MAP_READ_BIT
#  define GL_MAP_READ_BIT 0x0001
#endif
#ifndef GL_MAP_UNSYNCHRONIZED_BIT
#  define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
#endif
#ifndef GL_MAP_WRITE_BIT
#  define GL_MAP_WRITE_BIT 0x0002
#endif
#ifndef GL_MAX_3D_TEXTURE_SIZE
#  define GL_MAX_3D_TEXTURE_SIZE 0x8073
#endif
#ifndef GL_MAX_ARRAY_TEXTURE_LAYERS
#  define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#endif
#ifndef GL_MAX_COLOR_ATTACHMENTS
#  define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#endif
#ifndef GL_MAX_DEBUG_LOGGED_MESSAGES
#  define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#endif
#ifndef GL_MAX_DEBUG_MESSAGE_LENGTH
#  define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#endif
#ifndef GL_MAX_DRAW_BUFFERS
#  define GL_MAX_DRAW_BUFFERS 0x8824
#endif
#ifndef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
#  define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#endif
#ifndef GL_MAX_FRAGMENT_UNIFORM_VECTORS
#  define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD
#endif
#ifndef GL_MAX_RENDERBUFFER_SIZE
#  define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#endif
#ifndef GL_MAX_SAMPLE_MASK_WORDS
#  define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#endif
#ifndef GL_MAX_SAMPLES
#  define GL_MAX_SAMPLES 0x8D57
#endif
#ifndef GL_MAX_SERVER_WAIT_TIMEOUT
#  define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#endif
#ifndef GL_MAX_TEXTURE_BUFFER_SIZE
#  define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#  define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
#ifndef GL_MAX_TRANSFORM_FEEDBACK_BUFFERS
#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
#endif
#ifndef GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#endif
#ifndef GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#endif
#ifndef GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#endif
#ifndef GL_MAX_VARYING_VECTORS
#  define GL_MAX_VARYING_VECTORS 0x8DFC
#endif
#ifndef GL_MAX_VERTEX_UNIFORM_COMPONENTS
#  define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#endif
#ifndef GL_MAX_VERTEX_UNIFORM_VECTORS
#  define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#endif
#ifndef GL_MEDIUM_FLOAT
#  define GL_MEDIUM_FLOAT 0x8DF1
#endif
#ifndef GL_MEDIUM_INT
#  define GL_MEDIUM_INT 0x8DF4
#endif
#ifndef GL_MULTISAMPLE
#  define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_NUM_EXTENSIONS
#  define GL_NUM_EXTENSIONS 0x821D
#endif
#ifndef GL_NUM_SHADER_BINARY_FORMATS
#  define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#endif
#ifndef GL_OBJECT_TYPE
#  define GL_OBJECT_TYPE 0x9112
#endif
#ifndef GL_PALETTE4_R5_G6_B5_OES
#  define GL_PALETTE4_R5_G6_B5_OES 0x8B92
#endif
#ifndef GL_PALETTE4_RGB5_A1_OES
#  define GL_PALETTE4_RGB5_A1_OES 0x8B94
#endif
#ifndef GL_PALETTE4_RGB8_OES
#  define GL_PALETTE4_RGB8_OES 0x8B90
#endif
#ifndef GL_PALETTE4_RGBA4_OES
#  define GL_PALETTE4_RGBA4_OES 0x8B93
#endif
#ifndef GL_PALETTE4_RGBA8_OES
#  define GL_PALETTE4_RGBA8_OES 0x8B91
#endif
#ifndef GL_PALETTE8_R5_G6_B5_OES
#  define GL_PALETTE8_R5_G6_B5_OES 0x8B97
#endif
#ifndef GL_PALETTE8_RGB5_A1_OES
#  define GL_PALETTE8_RGB5_A1_OES 0x8B99
#endif
#ifndef GL_PALETTE8_RGB8_OES
#  define GL_PALETTE8_RGB8_OES 0x8B95
#endif
#ifndef GL_PALETTE8_RGBA4_OES
#  define GL_PALETTE8_RGBA4_OES 0x8B98
#endif
#ifndef GL_PALETTE8_RGBA8_OES
#  define GL_PALETTE8_RGBA8_OES 0x8B96
#endif
#ifndef GL_PIXEL_PACK_BUFFER
#  define GL_PIXEL_PACK_BUFFER 0x88EB
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER
#  define GL_PIXEL_UNPACK_BUFFER 0x88EC
#endif
#ifndef GL_PIXEL_PACK_BUFFER_BINDING
#  define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_BINDING
#  define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#endif
#ifndef GL_POINT_SIZE
#  define GL_POINT_SIZE 0x0B11
#endif
#ifndef GL_POINT_SIZE_RANGE
#define GL_POINT_SIZE_RANGE 0x0B12
#endif
#ifndef GL_POINT_SPRITE
#  define GL_POINT_SPRITE 0x8861
#endif
#ifndef GL_PRIMITIVES_GENERATED
#define GL_PRIMITIVES_GENERATED 0x8C87
#endif
#ifndef GL_PROGRAM_OBJECT
#  define GL_PROGRAM_OBJECT 0x8B40
#endif
#ifndef GL_PROGRAM_PIPELINE
#  define GL_PROGRAM_PIPELINE 0x82E4
#endif
#ifndef GL_PROGRAM_PIPELINE_OBJECT
#  define GL_PROGRAM_PIPELINE_OBJECT 0x8A4F
#endif
#ifndef GL_PROGRAM_POINT_SIZE
#  define GL_PROGRAM_POINT_SIZE 0x8642
#endif
#ifndef GL_QUERY
#  define GL_QUERY 0x82E3
#endif
#ifndef GL_QUERY_COUNTER_BITS_EXT
#  define GL_QUERY_COUNTER_BITS_EXT 0x8864
#endif
#ifndef GL_QUERY_OBJECT
#  define GL_QUERY_OBJECT 0x9153
#endif
#ifndef GL_QUERY_RESULT_AVAILABLE_EXT
#  define GL_QUERY_RESULT_AVAILABLE_EXT 0x8867
#endif
#ifndef GL_QUERY_RESULT_EXT
#  define GL_QUERY_RESULT_EXT 0x8866
#endif
#ifndef GL_R11F_G11F_B10F
#  define GL_R11F_G11F_B10F 0x8C3A
#endif
#ifndef GL_R16F
#  define GL_R16F 0x822D
#endif
#ifndef GL_R16I
#  define GL_R16I 0x8233
#endif
#ifndef GL_R16UI
#  define GL_R16UI 0x8234
#endif
#ifndef GL_R32F
#  define GL_R32F 0x822E
#endif
#ifndef GL_R32I
#  define GL_R32I 0x8235
#endif
#ifndef GL_R32UI
#  define GL_R32UI 0x8236
#endif
#ifndef GL_R8
#  define GL_R8 0x8229
#endif
#ifndef GL_R8I
#  define GL_R8I 0x8231
#endif
#ifndef GL_R8UI
#  define GL_R8UI 0x8232
#endif
#ifndef GL_R8_SNORM
#  define GL_R8_SNORM 0x8F94
#endif
#ifndef GL_RASTERIZER_DISCARD
#define GL_RASTERIZER_DISCARD 0x8C89
#endif
#ifndef GL_READ_BUFFER
#  define GL_READ_BUFFER 0x0C02
#endif
#ifndef GL_READ_FRAMEBUFFER
#  define GL_READ_FRAMEBUFFER 0x8CA8
#endif
#ifndef GL_READ_FRAMEBUFFER_BINDING
#  define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#endif
#ifndef GL_READ_ONLY
#  define GL_READ_ONLY 0x88B8
#endif
#ifndef GL_READ_WRITE
#  define GL_READ_WRITE 0x88BA
#endif
#ifndef GL_RED
#  define GL_RED 0x1903
#endif
#ifndef GL_RED_BITS
#  define GL_RED_BITS 0x0D52
#endif
#ifndef GL_RED_INTEGER
#  define GL_RED_INTEGER 0x8D94
#endif
#ifndef GL_RENDERBUFFER
#  define GL_RENDERBUFFER 0x8D41
#endif
#ifndef GL_RENDERBUFFER_ALPHA_SIZE
#  define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#endif
#ifndef GL_RENDERBUFFER_BINDING
#  define GL_RENDERBUFFER_BINDING 0x8CA7
#endif
#ifndef GL_RENDERBUFFER_BLUE_SIZE
#  define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#endif
#ifndef GL_RENDERBUFFER_DEPTH_SIZE
#  define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#endif
#ifndef GL_RENDERBUFFER_GREEN_SIZE
#  define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#endif
#ifndef GL_RENDERBUFFER_HEIGHT
#  define GL_RENDERBUFFER_HEIGHT 0x8D43
#endif
#ifndef GL_RENDERBUFFER_INTERNAL_FORMAT
#  define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#endif
#ifndef GL_RENDERBUFFER_RED_SIZE
#  define GL_RENDERBUFFER_RED_SIZE 0x8D50
#endif
#ifndef GL_RENDERBUFFER_STENCIL_SIZE
#  define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#endif
#ifndef GL_RENDERBUFFER_WIDTH
#  define GL_RENDERBUFFER_WIDTH 0x8D42
#endif
#ifndef GL_RG
#  define GL_RG 0x8227
#endif
#ifndef GL_RG16F
#  define GL_RG16F 0x822F
#endif
#ifndef GL_RG16I
#  define GL_RG16I 0x8239
#endif
#ifndef GL_RG16UI
#  define GL_RG16UI 0x823A
#endif
#ifndef GL_RG32F
#  define GL_RG32F 0x8230
#endif
#ifndef GL_RG32I
#  define GL_RG32I 0x823B
#endif
#ifndef GL_RG32UI
#  define GL_RG32UI 0x823C
#endif
#ifndef GL_RG8
#  define GL_RG8 0x822B
#endif
#ifndef GL_RG8I
#  define GL_RG8I 0x8237
#endif
#ifndef GL_RG8UI
#  define GL_RG8UI 0x8238
#endif
#ifndef GL_RG8_SNORM
#  define GL_RG8_SNORM 0x8F95
#endif
#ifndef GL_RGB10_A2
#  define GL_RGB10_A2 0x8059
#endif
#ifndef GL_RGB10_A2UI
#  define GL_RGB10_A2UI 0x906F
#endif
#ifndef GL_RGB16F
#  define GL_RGB16F 0x881B
#endif
#ifndef GL_RGB16I
#  define GL_RGB16I 0x8D89
#endif
#ifndef GL_RGB16UI
#  define GL_RGB16UI 0x8D77
#endif
#ifndef GL_RGB32F
#  define GL_RGB32F 0x8815
#endif
#ifndef GL_RGB32I
#  define GL_RGB32I 0x8D83
#endif
#ifndef GL_RGB32UI
#  define GL_RGB32UI 0x8D71
#endif
#ifndef GL_RGB8
#  define GL_RGB8 0x8051
#endif
#ifndef GL_RGB8I
#  define GL_RGB8I 0x8D8F
#endif
#ifndef GL_RGB8UI
#  define GL_RGB8UI 0x8D7D
#endif
#ifndef GL_RGB8_SNORM
#  define GL_RGB8_SNORM 0x8F96
#endif
#ifndef GL_RGB9_E5
#  define GL_RGB9_E5 0x8C3D
#endif
#ifndef GL_RGB565
#  define GL_RGB565 0x8D62
#endif
#ifndef GL_RGBA16F
#  define GL_RGBA16F 0x881A
#endif
#ifndef GL_RGBA16I
#  define GL_RGBA16I 0x8D88
#endif
#ifndef GL_RGBA16UI
#  define GL_RGBA16UI 0x8D76
#endif
#ifndef GL_RGBA32F
#  define GL_RGBA32F 0x8814
#endif
#ifndef GL_RGBA32I
#  define GL_RGBA32I 0x8D82
#endif
#ifndef GL_RGBA32UI
#  define GL_RGBA32UI 0x8D70
#endif
#ifndef GL_RGBA8
#  define GL_RGBA8 0x8058
#endif
#ifndef GL_RGBA8I
#  define GL_RGBA8I 0x8D8E
#endif
#ifndef GL_RGBA8UI
#  define GL_RGBA8UI 0x8D7C
#endif
#ifndef GL_RGBA8_SNORM
#  define GL_RGBA8_SNORM 0x8F97
#endif
#ifndef GL_RGBA_INTEGER
#  define GL_RGBA_INTEGER 0x8D99
#endif
#ifndef GL_RGB_INTEGER
#  define GL_RGB_INTEGER 0x8D98
#endif
#ifndef GL_RG_INTEGER
#  define GL_RG_INTEGER 0x8228
#endif
#ifndef GL_RIGHT
#  define GL_RIGHT 0x0407
#endif
#ifndef GL_SAMPLE_POSITION
#  define GL_SAMPLE_POSITION 0x8E50
#endif
#ifndef GL_SAMPLE_MASK_VALUE
#  define GL_SAMPLE_MASK_VALUE 0x8E52
#endif
#ifndef GL_SAMPLER
#  define GL_SAMPLER 0x82E6
#endif
#ifndef GL_SAMPLER_1D
#  define GL_SAMPLER_1D 0x8B5D
#endif
#ifndef GL_SAMPLER_1D_ARRAY
#  define GL_SAMPLER_1D_ARRAY 0x8DC0
#endif
#ifndef GL_SAMPLER_1D_ARRAY_SHADOW
#  define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#endif
#ifndef GL_SAMPLER_1D_SHADOW
#  define GL_SAMPLER_1D_SHADOW 0x8B61
#endif
#ifndef GL_SAMPLER_2D_ARRAY
#  define GL_SAMPLER_2D_ARRAY 0x8DC1
#endif
#ifndef GL_SAMPLER_2D_ARRAY_SHADOW
#  define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#endif
#ifndef GL_SAMPLER_2D_MULTISAMPLE
#  define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#endif
#ifndef GL_SAMPLER_2D_MULTISAMPLE_ARRAY
#  define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#endif
#ifndef GL_SAMPLER_2D_SHADOW
#  define GL_SAMPLER_2D_SHADOW 0x8B62
#endif
#ifndef GL_SAMPLER_3D
#  define GL_SAMPLER_3D 0x8B5F
#endif
#ifndef GL_SAMPLER_BINDING
#  define GL_SAMPLER_BINDING 0x8919
#endif
#ifndef GL_SAMPLER_CUBE_MAP_ARRAY
#  define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#endif
#ifndef GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW
#  define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#endif
#ifndef GL_SAMPLER_CUBE_SHADOW
#  define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#endif
#ifndef GL_SAMPLER_EXTERNAL_OES
#  define GL_SAMPLER_EXTERNAL_OES 0x8D66
#endif
#ifndef GL_SEPARATE_ATTRIBS
#define GL_SEPARATE_ATTRIBS 0x8C8D
#endif
#ifndef GL_SHADER_BINARY_FORMATS
#  define GL_SHADER_BINARY_FORMATS 0x8DF8
#endif
#ifndef GL_SHADER_COMPILER
#  define GL_SHADER_COMPILER 0x8DFA
#endif
#ifndef GL_SHADER_OBJECT
#  define GL_SHADER_OBJECT 0x8B48
#endif
#ifndef GL_SIGNALED
#  define GL_SIGNALED 0x9119
#endif
#ifndef GL_SRGB8
#  define GL_SRGB8 0x8C41
#endif
#ifndef GL_SRGB8_ALPHA8
#  define GL_SRGB8_ALPHA8 0x8C43
#endif
#ifndef GL_STENCIL
#  define GL_STENCIL 0x1802
#endif
#ifndef GL_STENCIL_ATTACHMENT
#  define GL_STENCIL_ATTACHMENT 0x8D20
#endif
#ifndef GL_STENCIL_BITS
#  define GL_STENCIL_BITS 0x0D57
#endif
#ifndef GL_STENCIL_BUFFER_BIT
#  define GL_STENCIL_BUFFER_BIT 0x00000400
#endif
#ifndef GL_STENCIL_INDEX8
#  define GL_STENCIL_INDEX8 0x8D48
#endif
#ifndef GL_SYNC_CONDITION
#  define GL_SYNC_CONDITION 0x9113
#endif
#ifndef GL_SYNC_FENCE
#  define GL_SYNC_FENCE 0x9116
#endif
#ifndef GL_SYNC_FLAGS
#  define GL_SYNC_FLAGS 0x9115
#endif
#ifndef GL_SYNC_FLUSH_COMMANDS_BIT
#  define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#endif
#ifndef GL_SYNC_GPU_COMMANDS_COMPLETE
#  define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#endif
#ifndef GL_SYNC_STATUS
#  define GL_SYNC_STATUS 0x9114
#endif
#ifndef GL_TEXTURE_1D_ARRAY
#  define GL_TEXTURE_1D_ARRAY 0x8C18
#endif
#ifndef GL_TEXTURE_2D_ARRAY
#  define GL_TEXTURE_2D_ARRAY 0x8C1A
#endif
#ifndef GL_TEXTURE_2D_MULTISAMPLE
#  define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#endif
#ifndef GL_TEXTURE_2D_MULTISAMPLE_ARRAY
#  define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#endif
#ifndef GL_TEXTURE_3D
#  define GL_TEXTURE_3D 0x806F
#endif
#ifndef GL_TEXTURE_BASE_LEVEL
#  define GL_TEXTURE_BASE_LEVEL 0x813C
#endif
#ifndef GL_TEXTURE_BINDING_1D_ARRAY
#  define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#endif
#ifndef GL_TEXTURE_BINDING_2D_ARRAY
#  define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#endif
#ifndef GL_TEXTURE_BINDING_2D_MULTISAMPLE
#  define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#endif
#ifndef GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
#  define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#endif
#ifndef GL_TEXTURE_BINDING_3D
#  define GL_TEXTURE_BINDING_3D 0x806A
#endif
#ifndef GL_TEXTURE_BINDING_BUFFER
#  define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#endif
#ifndef GL_TEXTURE_BINDING_CUBE_MAP_ARRAY
#  define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#endif
#ifndef GL_TEXTURE_BINDING_EXTERNAL_OES
#  define GL_TEXTURE_BINDING_EXTERNAL_OES 0x8D67
#endif
#ifndef GL_TEXTURE_BUFFER
#  define GL_TEXTURE_BUFFER 0x8C2A
#endif
#ifndef GL_TEXTURE_BUFFER_DATA_STORE_BINDING
#  define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#endif
#ifndef GL_TEXTURE_BUFFER_FORMAT
#  define GL_TEXTURE_BUFFER_FORMAT 0x8C2E
#endif
#ifndef GL_TEXTURE_COMPARE_FUNC
#  define GL_TEXTURE_COMPARE_FUNC 0x884D
#endif
#ifndef GL_TEXTURE_COMPARE_MODE
#  define GL_TEXTURE_COMPARE_MODE 0x884C
#endif
#ifndef GL_TEXTURE_CUBE_MAP_ARRAY
#  define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#endif
#ifndef GL_TEXTURE_EXTERNAL_OES
#  define GL_TEXTURE_EXTERNAL_OES 0x8D65
#endif
#ifndef GL_TEXTURE_FIXED_SAMPLE_LOCATIONS
#  define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#endif
#ifndef GL_TEXTURE_IMMUTABLE_FORMAT
#  define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F
#endif
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#  define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif
#ifndef GL_TEXTURE_MAX_LEVEL
#  define GL_TEXTURE_MAX_LEVEL 0x813D
#endif
#ifndef GL_TEXTURE_MAX_LOD
#  define GL_TEXTURE_MAX_LOD 0x813B
#endif
#ifndef GL_TEXTURE_MIN_LOD
#  define GL_TEXTURE_MIN_LOD 0x813A
#endif
#ifndef GL_TEXTURE_SAMPLES
#  define GL_TEXTURE_SAMPLES 0x9106
#endif
#ifndef GL_TEXTURE_SWIZZLE_R
#  define GL_TEXTURE_SWIZZLE_R 0x8E42
#endif
#ifndef GL_TEXTURE_SWIZZLE_G
#  define GL_TEXTURE_SWIZZLE_G 0x8E43
#endif
#ifndef GL_TEXTURE_SWIZZLE_B
#  define GL_TEXTURE_SWIZZLE_B 0x8E44
#endif
#ifndef GL_TEXTURE_SWIZZLE_A
#  define GL_TEXTURE_SWIZZLE_A 0x8E45
#endif
#ifndef GL_TEXTURE_WRAP_R
#  define GL_TEXTURE_WRAP_R 0x8072
#endif
#ifndef GL_TIMEOUT_EXPIRED
#  define GL_TIMEOUT_EXPIRED 0x911B
#endif
#ifndef GL_TIMEOUT_IGNORED
#  define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF
#endif
#ifndef GL_TIMESTAMP_EXT
#  define GL_TIMESTAMP_EXT 0x8E28
#endif
#ifndef GL_TIME_ELAPSED_EXT
#  define GL_TIME_ELAPSED_EXT 0x88BF
#endif
#ifndef GL_TRANSFORM_FEEDBACK
#  define GL_TRANSFORM_FEEDBACK 0x8E22
#endif
#ifndef GL_TRANSFORM_FEEDBACK_ACTIVE
#define GL_TRANSFORM_FEEDBACK_ACTIVE 0x8E24
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BINDING
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_BINDING
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_MODE
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_SIZE
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_START
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#endif
#ifndef GL_TRANSFORM_FEEDBACK_PAUSED
#define GL_TRANSFORM_FEEDBACK_PAUSED 0x8E23
#endif
#ifndef GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#endif
#ifndef GL_TRANSFORM_FEEDBACK_VARYINGS
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#endif
#ifndef GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#endif
#ifndef GL_UNSIGNALED
#  define GL_UNSIGNALED 0x9118
#endif
#ifndef GL_UNSIGNED_INT_10F_11F_11F_REV
#  define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#endif
#ifndef GL_UNSIGNED_INT_24_8
#  define GL_UNSIGNED_INT_24_8 0x84FA
#endif
#ifndef GL_UNSIGNED_INT_2_10_10_10_REV
#  define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#endif
#ifndef GL_UNSIGNED_INT_5_9_9_9_REV
#  define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_1D
#  define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_2D
#  define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_3D
#  define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_CUBE
#  define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
#  define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
#  define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#endif
#ifndef GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY
#  define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#endif
#ifndef GL_UNSIGNED_INT_VEC2
#  define GL_UNSIGNED_INT_VEC2 0x8DC6
#endif
#ifndef GL_UNSIGNED_INT_VEC3
#  define GL_UNSIGNED_INT_VEC3 0x8DC7
#endif
#ifndef GL_UNSIGNED_INT_VEC4
#  define GL_UNSIGNED_INT_VEC4 0x8DC8
#endif
#ifndef GL_VERTEX_ARRAY_BINDING
#  define GL_VERTEX_ARRAY_BINDING 0x85B5
#endif
#ifndef GL_VERTEX_ARRAY_OBJECT
#  define GL_VERTEX_ARRAY_OBJECT 0x9154
#endif
#ifndef GL_VERTEX_ATTRIB_ARRAY_DIVISOR
#  define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88fe
#endif
#ifndef GL_WAIT_FAILED
#  define GL_WAIT_FAILED 0x911D
#endif
#ifndef GL_WRITE_ONLY
#  define GL_WRITE_ONLY 0x88B9
#endif

#endif  // ION_PORTGFX_GLHEADERS_H_
