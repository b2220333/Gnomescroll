AR = '/usr/bin/ar'
ARCH_ST = ['-arch']
ARFLAGS = 'rcs'
BINDIR = '/usr/local/bin'
CC = ['/usr/bin/gcc']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'gcc'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('4', '2', '1')
CFLAGS = ['-g', '-Wall']
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_PYEMBED = ['-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fwrapv', '-arch', 'x86_64', '-arch', 'x86_64', '-fno-strict-aliasing']
CFLAGS_PYEXT = ['-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fwrapv', '-arch', 'x86_64', '-arch', 'x86_64', '-fno-strict-aliasing']
CFLAGS_cshlib = ['-fPIC', '-compatibility_version', '1', '-current_version', '1']
COMPILER_CC = 'gcc'
CPPPATH_ST = '-I%s'
CXXFLAGS_PYEMBED = ['-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fwrapv', '-arch', 'x86_64', '-arch', 'x86_64']
CXXFLAGS_PYEXT = ['-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fwrapv', '-arch', 'x86_64', '-arch', 'x86_64']
CYTHON = '/usr/local/bin/cython'
DEFINES = ['PYTHONDIR="/usr/local/lib/python2.7/site-packages"', 'PYTHONARCHDIR="/usr/local/lib/python2.7/site-packages"', 'HAVE_PYTHON_H=1', 'HAVE_GL_GLEW_H=1', 'HAVE_SDL=1', 'HAVE_SDL_H=1', 'HAVE_SDL_IMAGE_H=1', 'HAVE_FMOD_H=1']
DEFINES_PYEMBED = ['ENABLE_DTRACE', 'MACOSX', 'NDEBUG', 'NDEBUG', 'ENABLE_DTRACE']
DEFINES_PYEXT = ['ENABLE_DTRACE', 'MACOSX', 'NDEBUG', 'NDEBUG', 'ENABLE_DTRACE']
DEFINES_SDL = ['_GNU_SOURCE=1', '_THREAD_SAFE']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'mac-o'
DEST_CPU = 'x86_64'
DEST_OS = 'darwin'
FRAMEWORKPATH_ST = '-F%s'
FRAMEWORK_GL = ['Cocoa', 'OpenGL']
FRAMEWORK_ST = ['-framework']
HAVE_FMOD_H = 1
HAVE_GL_GLEW_H = 1
HAVE_PYTHON_H = 1
HAVE_SDL = 1
HAVE_SDL_H = 1
HAVE_SDL_IMAGE_H = 1
INCLUDES = ['c_lib', 'SDL', 'cube_lib', 'vox_lib', 'sound', 'libraries/lin/fmod/inc']
INCLUDES_FMOD = ['../../libraries/lin/fmod/inc']
INCLUDES_FMOD.H = ['../../libraries/lin/fmod/inc']
INCLUDES_PYEMBED = ['/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7']
INCLUDES_PYEXT = ['/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7']
INCLUDES_SDL = ['/usr/local/include/SDL']
LIBDIR = '/usr/local/lib'
LIBPATH_FMOD = ['../../../libraries/lin/fmod/lib']
LIBPATH_PYEMBED = ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib']
LIBPATH_PYEXT = ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib']
LIBPATH_PYTHON2.7 = ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib']
LIBPATH_SDL = ['/usr/local/lib']
LIBPATH_ST = '-L%s'
LIB_FMOD = ['fmodex']
LIB_GL = ['GLEW']
LIB_PYEMBED = ['python2.7']
LIB_PYEXT = ['python2.7']
LIB_PYTHON2.7 = ['python2.7']
LIB_SDL = ['SDLmain', 'SDL']
LIB_SDL_image = ['SDL_image']
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_PYEMBED = ['-Wl,-F.', '-arch', 'x86_64', '-arch', 'x86_64', '-Wl,-F.', '-Wl,-F.', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64']
LINKFLAGS_PYEXT = ['-Wl,-F.', '-arch', 'x86_64', '-arch', 'x86_64', '-Wl,-F.', '-Wl,-F.', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64', '-arch', 'x86_64']
LINKFLAGS_SDL = ['-Wl,-framework,Cocoa']
LINKFLAGS_cshlib = ['-dynamiclib']
LINKFLAGS_cstlib = []
LINK_CC = ['/usr/bin/gcc']
MACOSX_DEPLOYMENT_TARGET = '10.7'
PREFIX = '/usr/local'
PYC = 1
PYCMD = '"import sys, py_compile;py_compile.compile(sys.argv[1], sys.argv[2])"'
PYFLAGS = ''
PYFLAGS_OPT = '-O'
PYO = 1
PYTHON = ['/usr/bin/python']
PYTHONARCHDIR = '/usr/local/lib/python2.7/site-packages'
PYTHONDIR = '/usr/local/lib/python2.7/site-packages'
PYTHON_CONFIG = '/usr/bin/python2.7-config'
PYTHON_VERSION = '2.7'
RPATH = ['.', '..', './ext/', 'libraries/lin/fmod/lib']
RPATH_ST = '-Wl,-rpath,%s'
SHLIB_MARKER = []
SONAME_ST = []
STLIBPATH_ST = '-L%s'
STLIB_MARKER = []
STLIB_ST = '-l%s'
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.dylib'
cstlib_PATTERN = 'lib%s.a'
define_key = ['PYTHONDIR', 'PYTHONARCHDIR', 'HAVE_PYTHON_H', 'HAVE_GL_GLEW_H', 'HAVE_SDL', 'HAVE_SDL_H', 'HAVE_SDL_IMAGE_H', 'HAVE_FMOD_H']
macbundle_PATTERN = '%s.bundle'
os = 'Darwin'
pyext_PATTERN = '%s.so'
