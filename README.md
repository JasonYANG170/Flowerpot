# Flowerpot花盆
这是一项基于C++语言，以ESP8266为核心处理器开发的Arduino物联网项目

本项目开源版本为核心版本，遵循GPL2.0开源许可协议
# 迭代说明
  **3月**，打造了我们第一个产品Flowerpot节水净水花盆，该花盆主要面向于年轻用户群体，搭载了我们首个主要特点“拟人表情显示技术”，通过采集植物生长数据结合云端AI模型生成拟人表情。同时我们使用3D打印技术制作初代产品的外壳。  **5月**，我们认为初代产品存在电路结构混乱，外观设计笨重，美观度较差等问题，于是我们重新对其外观进行设计，以及使用PCB设计软件完成了我们产品的PCB集成电路布局。

 **7月**，我们通过个人渠道发布了产品问卷调查，调查反馈市场兴趣较大，于是我们对产品的源码进行重构，并为初代产品源码颁发GPL2.0开源许可协议，同时在GitHub开源平台开放初代产品源码。

 **9月**，我们对该产品迭代升级，为Flowerpot节水净水花盆新增MQTT云端控制技术及OTA功能，为我们的物联网数字生活类产品打造了分别基于C++、Kotlin的Windows、Android客户端软件，用户可在手机或电脑远程控制我们的产品以及通过云端完成软件升级。

  **10月**，我们再次对产品迭代，并添加了AI交互大模型及深度视觉识别学习算法，用户可以通过客户端软件连接AI交互大模型实现与植物的拟人交互，研究认为有助于缓解用户压力。同时用户可以通过拍摄植物生长过程的图片并上传给电脑客户端，深度视觉识别学习算法可以得出植物生长趋势，生长速率等信息。

  **12月**，通过我们的种植认为原有花盆结构存在烂根，排水能力较弱问题，我们对于花盆整体结构重新升级，并引入新的“模块化设计”思路，用户可以根据喜好选择搭载模块化部件，可以添加蓄电池模块，太阳能模块，LED蓝光模块，自动浇水模块，笔筒组件等等。
# 功能
1.获取温湿度传感器数据，并根据温湿度传感器数据控制电机完成自动浇水。

2.支持mqtt服务器收发

3，支持OTA技术


