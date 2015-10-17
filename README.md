### [DMET 502 - Computer Graphics](http://met.guc.edu.eg/Courses/CourseEdition.aspx?crsEdId=606) 2D Assignment

This is a 2D graphics assignment done for computer graphics course in my university, it's a simplified (modified) clone of [King Of Thieves](http://www.kingofthieves.com/) game.

The assignment is written using c++ with openGL and [glut](https://www.opengl.org/resources/libraries/glut/) .

You can find two different source files, one is ready to run on Visual Studio with GLUT API and the other is ready to be run with [freeglut](http://freeglut.sourceforge.net/index.php).

### Installation :

#### Windows 

Just create a Win32 C++ console application in visual studio, link it with openGL and add visual studio [source files][visual-studio-files] to your project .

#### Linux with freeglut 

Install freeglut on your system, if you are using apt you can follow this [guide][linux-install]

compile the source code and make sure you link openGL and glut while compiling by using :

```
g++ -lglut -lGL -lGLU source.cpp -o execuatable
```

Then run the executable

```
./executable.out
```


[visual-studio-files]: https://github.com/abdelrahman-elkady/king-of-thieves-guc/tree/master/visual-studio
[linux-install]: http://kiwwito.com/installing-opengl-glut-libraries-in-ubuntu/
