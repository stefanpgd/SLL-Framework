# SLL ( Sweet Little Lines ), A Ray Tracing Framework
**SLL or '_Sweet Little Lines_' is a multi-threaded, CPU only, ray tracer framework written in C++**.

SLL is a 'realtime' ray tracer that produces around 30 to 300 million rays per second using the CPU. 
The back-end/engine that is being used is '[Template](https://github.com/jbikker/tmpl8)', made by [Jacco Bikker](https://github.com/jbikker)

NOTE: By default path-tracing is turned on but in `Color.cpp` the define symbol `DiffuseReflection` can be flipped to false to enable Whitted-style ray tracing.

How can I run it?
1. Download/Clone the project
2. Open the solution
3. Set the project to the desired configuration ( x64 Release runs the fastest )
4. Run it!

![Scene 1 and 7](https://user-images.githubusercontent.com/47507160/162332296-dc3750eb-6e36-45a4-98e0-f424758cc6df.png)
