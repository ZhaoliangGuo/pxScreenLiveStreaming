# Target of pxScreenLiveStreaming
A record and live streaming server for windows screen.

用于采集计算机本机的视频和音频，支持保存为flv文件，支持rtsp/http-flv/hls/rtmp输出。

# 程序编译前的准备工作
- 安装nasm并将路径添加到环境变量。
```
C:\Program Files\NASM
```

nasm是汇编编译的工具。

因为rgb转换为yuv的过程中 调用了汇编代码 
需要用nasm来操作汇编代码

[nasm下载地址](http://www.nasm.us/pub/nasm/releasebuilds/2.13.01/)

如果未安装nasm，会出现如下现象:

![未安装nasm时的错误现象](https://github.com/ZhaoliangGuo/Resources/blob/master/error1.png)


# AlreadyDone
- 采集计算机本机的画面，并将其编码为.264格式 并可保存为flv文件
- 采集麦克风的数据并将其保存为.pcm数据

- 分析视频各阶段耗时
比如采集bmp耗时多少
比如bmp转换为yuv耗时多少
比如yuv编码为264耗时多超

# ToDoList
- 梳理软件的架构

- 采集本机的音频的数据 编码为aac数据

- 将264视频数据和aac音频数据保存为flv文件

- 支持选择帧率 暂定比如10帧/s 20帧/s 25帧/s 

- 支持rtsp输出
- 支持http-flv输出
- 支持hls输出
- 支持rtmp输出

# Debug
- 视频各阶段时间分析
```
#define DEBUG_VIDEO_TIME_ANALYZE 1
```
![结合DebugView++分析视频各阶段耗时](https://github.com/ZhaoliangGuo/Resources/blob/master/images/time_analyze.jpg)

- 音频采集麦克风开启、采集数据、关闭分析
```
#define DEBUG_AUDIO_CAPTURE_PCM 1
```
![音频采集麦克风开启、采集数据、关闭分析](https://github.com/ZhaoliangGuo/Resources/blob/master/audio_pcm_capture_analyze.jpg)
