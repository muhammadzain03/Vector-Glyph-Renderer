# CMake SFML Project Template (with OpenGL)

Use this template repository if you would like to mix SFML Graphics with OpenGL. 

This template is based on the [original template repository](https://github.com/SFML/cmake-sfml-project) provided by SFML. The only changes made are to some compilation flags. In particular, building of network and audio components in disabled as we do not need these components in the course. This modified version should also compile without issues in the graphics lab (MS 239)

Additionally, OpenGL is set up via [GLAD](https://wikis.khronos.org/opengl/OpenGL_Loading_Library). For cross-platform compatibility, this repositroty uses an older version of OpenGL (2.1) as there is currently an [issue with mixing SFML Graphics with OpenGL on MacOS](https://www.sfml-dev.org/tutorials/3.0/window/opengl/). 

For questions or concerns, please get in touch with the instructor or one of the TAs.


