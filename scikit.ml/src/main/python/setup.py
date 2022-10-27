import setuptools

dependencies = [
      'scikit-learn',
      'NumPy >= 1.8.2',
      'SciPy >= 0.13.3',
      'tensorflow'
]

setuptools.setup(name='SEDE-scikit.ml',
      version='0.1',
      description='SEDE service scikitlearn and tensorflow',
      url='http://github.com/fmohr/SEDE',
      author='Amin Faez',
      author_email='aminfaez@mail.upb.de',
      license='GNU',
      packages=setuptools.find_packages(),
      install_requires=dependencies)