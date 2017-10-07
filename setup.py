from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
ext_modules = [
    Extension("dist",
              ["cython_dist.pyx"],
              library_dirs=['.'],
              libraries=["cdist"])  # Unix-like specific
]
setup(
    name="Demos",
    cmdclass={"build_ext": build_ext},
    ext_modules=ext_modules
)
