from setuptools import setup, find_packages

setup(
    name="MotoCANlib",
    version="1.0.0",
    packages=find_packages(),
    include_package_data=True,
    package_data={"generator": ["templates/*.j2"]},
    install_requires=[
        "jinja2",
        "pyyaml",
        "MotoParseCAN"
    ],
    entry_points={
        "console_scripts": [
            "generate-can-code = generator.main:main"
        ]
    },
)