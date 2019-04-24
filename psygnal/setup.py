from distutils.core import setup, Extension
import os

ext_src = [os.path.join('psygnal', 'psygnalmodule.c')]
ext = Extension('psygnal._psygnal', ext_src)

setup(name='psygnal',
      version='1.0',
      description='Provides full access to psiginfo',
      url='https://github.com/jasonmp85/psygnal',
      author='Jason Petersen',
      author_email='Jason.Petersen@microsoft.com',
      package_dir={'psygnal': 'lib'},
      packages=['psygnal'],
      ext_modules=[ext])
