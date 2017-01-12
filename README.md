bug1: 

Shaders/colorShading.vert : no such file or directory fail to open Shaders/colorShading.vert

Solutions:

properties->configuration properties->Debugging and change the working directory to $(SolutionDir)

bug2:

Once,in tutorial9, the window displays all blue color, it seems have no graphic effect.

Solution:

PDB文件的问题。
1、点 调试 2、然后 选项和设置 3、右边勾上 启用源服务器支持 4、左边点 符号 5、把微软符号服务器勾 6、运行的时候等一下  7、问题解决