# games106
games106 现代图形绘制流水线原理与实践，作业框架。

作业框架实践部分，选择了使用了AMD的[Cauldron](https://github.com/GPUOpen-LibrariesAndSDKs/Cauldron)绘制框架。使用这个渲染库有考虑到两点：1、它有一些图形对象必要的封装（应该没有人会想要裸写Vulkan API的代码）。2、它的封装程度相对来说比较低，适合初学者。因为这个渲染库的选择，学习者最好使用Window。同时Sample使用了Win32 APP没有命令行终端，需要使用OutputDebugStringA函数，需要在Visual Studio的Output页面，选择Debug查看调试信息。

这里不推荐MacOS用户来使用作业框架或者学习Vulkan，因为没有官方自己提供的SDK，以及调试工具。

因为作者不太熟悉Linux，同时Cauldron也没有提供Linux的支持，学习者有兴趣，可以自己提供Linux的适配，提供贡献到Cauldron。可以参考Cauldron中的issue[Linux and GCC support by Zakhrov · Pull Request #3 · GPUOpen-LibrariesAndSDKs/Cauldron (github.com)](https://github.com/GPUOpen-LibrariesAndSDKs/Cauldron/pull/3)

# homework

## homework0

作业0 是一个hello triangle的代码。

期望得到的渲染结果是一个青色背景的三角形![homework0_0](.\img\homework0_0.png)

直接运行会得到一个黑色背景的三角形。

![homework0_1](.\img\homework0_1.png)

作业0就是修复这个渲染错误。可以使用下面推荐的调试工具以及方法。

# Debug & Profile

Vulkan的调试有两类

- Vulkan API调用的相关问题，使用```vkCreateDebugReportCallbackEXT```设置回调函数，这个需要VK_EXT_debug_report扩展支持。回调函数定义在在```ExtValidation.cpp```中的```MyDebugReportCallback```函数。这里使用了OutputDebugStringA，需要在Visual Studio的Output页面，选择Debug查看调试信息。
- Vulkan 图形渲染的相关问题，可以在Vulkan的[相关页面](https://www.vulkan.org/tools#profilers-and-debuggers)中寻找和学习者显卡厂商相关的工具，这里比较推荐[RenderDoc](https://renderdoc.org/)和[Nvidia Nsight Graphics](https://developer.nvidia.com/nsight-graphics)

Vulkan 的Profile 推荐使用显卡厂商做的工具，同样可以在Vulkan的[相关页面](https://www.vulkan.org/tools#profilers-and-debuggers)中寻找和学习者显卡厂商相关的工具。这里不推荐使用RenderDoc做性能分析，它给的结果不太准确。

# Build

## Prerequisites

来自Cauldron

- [CMake 3.24](https://cmake.org/download/)
- [Visual Studio 2019 or 2022](https://visualstudio.microsoft.com/downloads/)
- [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk) 在安装VS的时候，勾选上最新版本的SDK
- [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) 直接下载最新的Vulkan SDK，同时更新显卡驱动到最新的版本

## how to build

```powershell
cd games106
mkdir out
cd out
cmake ..
./games106.sln
```

