#! /usr/bin/env python

import sys, os.path, glob
from setuptools import setup, Extension
from python.lib.moresetuptools import build_ext, CleanGenerated
# function to generate gen/*.{c,h}
from this_version import get_aubio_version, get_aubio_pyversion

__version__ = get_aubio_pyversion()
__aubio_version__ = get_aubio_version()

include_dirs = []
library_dirs = []
define_macros = [('AUBIO_VERSION', '%s' % __aubio_version__)]
extra_link_args = []

include_dirs += [ 'python/ext' ]
try:
    import numpy
    include_dirs += [ numpy.get_include() ]
except ImportError:
    pass

if sys.platform.startswith('darwin'):
    extra_link_args += ['-framework','CoreFoundation', '-framework','AudioToolbox']

sources = sorted(glob.glob(os.path.join('python', 'ext', '*.c')))

aubio_extension = Extension("aubio._aubio",
    sources,
    include_dirs = include_dirs,
    library_dirs = library_dirs,
    extra_link_args = extra_link_args,
    define_macros = define_macros)

if os.path.isfile('src/aubio.h'):
    if not os.path.isdir(os.path.join('build','src')):
        pass
        #__version__ += 'a2' # python only version

classifiers = [
    'Development Status :: 4 - Beta',
    'Environment :: Console',
    'Intended Audience :: Science/Research',
    'Topic :: Software Development :: Libraries',
    'Topic :: Multimedia :: Sound/Audio :: Analysis',
    'Topic :: Multimedia :: Sound/Audio :: Sound Synthesis',
    'Operating System :: POSIX',
    'Operating System :: MacOS :: MacOS X',
    'Operating System :: Microsoft :: Windows',
    'Programming Language :: C',
    'Programming Language :: Python',
    'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',
    ]

distrib = setup(name='aubio',
    version = __version__,
    packages = ['aubio'],
    package_dir = {'aubio':'python/lib/aubio'},
    ext_modules = [aubio_extension],
    description = 'a collection of tools for music analysis',
    long_description = 'a collection of tools for music analysis',
    license = 'GNU/GPL version 3',
    author = 'Paul Brossier',
    author_email = 'piem@aubio.org',
    maintainer = 'Paul Brossier',
    maintainer_email = 'piem@aubio.org',
    url = 'https://aubio.org/',
    platforms = 'any',
    classifiers = classifiers,
    install_requires = ['numpy'],
    setup_requires = ['numpy'],
    cmdclass = {
        'clean': CleanGenerated,
        'build_ext': build_ext,
        },
    entry_points = {
        'console_scripts': [
            'aubio = aubio.cmd:main',
            'aubiocut = aubio.cut:main',
        ],
    },
    test_suite = 'nose2.collector.collector',
    extras_require = {
        'tests': ['numpy'],
        },
    )
