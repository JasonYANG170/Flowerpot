<div align="center">
	  <img src="https://github.com/JasonYANG170/Flowerpot/assets/39414350/fb4f4e7e-ed8a-4745-9fe7-4c52a4537b50">
    <h1> Flowerpot交互花盆</h1>
    <img src="https://img.shields.io/github/license/JasonYANG170/Flowerpot?style=for-the-badge">
    <img src="https://img.shields.io/github/commit-activity/w/JasonYANG170/Flowerpot?style=for-the-badge">
	<img src="https://img.shields.io/github/languages/count/JasonYANG170/Flowerpot?logo=cplusplus&style=for-the-badge">
	<br>
    	<a href="https://discord.com/invite/az3ceRmgVe"><img alt="Discord" src="https://img.shields.io/discord/978108215499816980?style=social&logo=discord&label=echosec"></a>
  <br>

  <br>
这是一项基于C++语言，以ESP8266为核心处理器开发的Arduino物联网项目
  
<br>
本项目开源版本为Core版本，请遵守GPL2.0开源许可协议
</div>

## 演示视频
[![Flowerpot](https://res.cloudinary.com/marcomontalbano/image/upload/v1719167915/video_to_markdown/images/youtube--T8O9qbLbfRQ-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/T8O9qbLbfRQ "Flowerpot")


[无法播放请点此使用BiliBili](https://www.bilibili.com/video/BV1LCTFeuEKt/)
## 本项目接入万物互联IOT
电脑端：https://github.com/JasonYANG170/IOTConnect-PC


手机端：https://github.com/JasonYANG170/IOTConnect-Android


Web端：https://github.com/JasonYANG170/IOTConnect-Web

## 功能
- [x] 拟人表情
- [x] ChatGPT文字交互模型
- [x] OpenCV深度视觉识别算法
- [x] 客户端远控Android/Windows
- [x] 土壤加温
- [x] 无线充电
- [x] 蓄电池充电
- [x] 电量显示
- [x] 自动浇水
- [x] 支持mqtt服务器收发
- [x] 支持OTA技术
- [ ] 每日报告
- [ ] 模块插件

## 系统
#### FlowerpotOS2.0
基于Arduino平台开发，主要以第三方库完成功能实现

#### FlowerpotOS3.1
基于PlatformIO平台开发，剔除了第三方库，改为自行开发  
极大的缩小程序空间占用，剔除冗余功能与第三方库  
支持FS文件系统，支持基于Bootstorp5开发的WebServer服务  

## 开源协议
本项目遵循GPL2.0开源协议，使用本程序时请注明出处并进行版权声明  
本项目仅供学习研究与学习，严禁非授权的商业获利，
如果您有更好的建议，欢迎PR

## 荣誉
本项目取得中国大学生挑战杯广东省科技发明类省赛三等奖  
FlowerpotOS嵌入式系统软件著作权

## 迭代说明
**2023年3月**，打造了我们第一个产品Flowerpot节水净水花盆，该花盆主要面向于年轻用户群体，搭载了我们首个特点“拟人表情显示技术”，通过采集植物生长数据结合云端AI模型生成拟人表情。同时我们购入3D打印设备制作初代产品的外壳。  

**5月**，我们认为初代产品存在电路结构混乱，外观设计笨重，美观度较差等问题，于是我们重新对其外观进行设计，以及使用PCB设计软件完成了我们产品的PCB集成电路布局。  

**7月**，我们通过个人渠道发布了产品问卷调查，调查反馈市场兴趣较大，于是我们对产品的源码进行重构，并为初代产品源码颁发GPL2.0开源许可协议，同时在GitHub开源平台开放初代产品源码。  

**10月**，我们对该产品迭代升级，新增特点MQTT云端控制技术及OTA功能，为我们的物联网数字生活类产品打造了分别基于C++、Kotlin的Windows、Android客户端软件，用户可在手机或电脑远程控制我们的产品以及通过云端完成软件升级。  

**11月**，我们再次对产品迭代，改名为Flowerpot交互花盆，并添加了AI交互大模型及深度视觉识别学习算法特点，用户可以通过客户端软件连接AI交互大模型实现与植物的拟人交互，研究认为有助于缓解用户压力。同时用户可以通过拍摄植物生长过程的图片并上传给电脑客户端，基于深度视觉识别学习算法可以得出植物未来生长趋势，生长速率等信息。  

**2024年1月**，通过用户使用反馈，认为原有花盆结构存在烂根，排水能力较弱问题，我们对于花盆整体结构重新升级，并引入新的特点“模块化设计”思路，用户可以根据喜好选择搭载模块化部件，可以添加蓄电池模块，太阳能模块，LED红蓝光模块，自动浇水模块，笔筒组件等等。  





