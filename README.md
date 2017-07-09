# Target of pxScreenLiveStreaming
A record and live streaming server for windows screen.

用于采集计算机本机的视频和音频，支持保存为flv文件，支持rtsp/http-flv/hls/rtmp输出。

# AlreadyDone
- 采集计算机本机的画面，并将其编码为.264格式
- 分析视频各阶段耗时
比如采集bmp耗时多少
比如bmp转换为yuv耗时多少
比如yuv编码为264耗时多超
![结合DebugView++分析视频各阶段耗时](https://github.com/ZhaoliangGuo/Resources/blob/master/images/time_analyze.jpg)

# ToDoList
- 梳理软件的架构

- 采集本机的音频的数据 编码为aac数据

- 将264视频数据和aac音频数据保存为flv文件

- 支持选择帧率 暂定比如10帧/s 20帧/s 25帧/s 

- 支持rtsp输出
- 支持http-flv输出
- 支持hls输出
- 支持rtmp输出
