DFRobot_GravityTDS
===========================

* [English Version](./README.md)

SEN0244 即插即用，使用简单方便。3.3~5.5V的宽电压供电，0~2.3V的模拟信号输出，使得这款产品兼容5V、3.3V控制系统，能非常方便的接到现成的控制系统中使用。测量用的激励源采用交流信号，可有效防止探头极化，延长探头寿命的同时，也增加了输出信号的稳定性。TDS探头为防水探头，可长期浸入水中测量。 该产品可应用于生活用水、水培等领域的水质检测。有了这个传感器，就可轻松DIY一套TDS检测仪了，轻松检测水的洁净程度，为你的水质把好关。

![产品效果图](../../resources/images/SEN0244.png)

## 产品链接（https://www.dfrobot.com）

    SKU：SEN0244
  
## 目录

  * [概述](#概述)
  * [库安装](#库安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)

## 概述

这是一个TDS传感器得库，使用该库能够很方便的获取TDS数据，即插即用，使用方便，宽电压供电.


## 库安装

要使用这个库，需要下载依赖库：https://github.com/DFRobot/DFRobot_RaspberryPi_Expansion_Board，在将库下载到Raspberry Pi，然后打开例程文件夹。要执行一个例程demox.py，请在命令行中输入python demox.py。例如，要执行control_led.py例程，你需要输入:

```python
python control_led.py
```

## 方法

```python
'''!
  @brief 设置温度补偿数据
  @param temperature 温度补偿数据
'''
def set_temperature(self, temperature)
    
'''!
  @brief 设置模拟数据采集引脚
  @param pin 模拟数据采集引脚
'''
def set_pin(self, pin)

'''!
  @brief 设置ADC参考电压
  @param value ADC参考电压
'''
def set_aref(self, value)

'''!
  @brief 设置ADC范围，10bit为1024，12bit为4096
  @param ADC范围
'''  
def set_adc_range(self, range)

'''!
  @brief 设置TDS K 值
  @param value K值数据
'''
def set_k_value(self, value)

'''!
  @brief 获取TDS数据
  @return 返回TDS数据
'''  
def get_tds_value(self)

'''!
  @brief 获取ec数据
  @return 返回ec数据
'''  
def get_ec_value(self)

'''!
  @brief 获取ADC数据并计算
''' 
def update(self)
```

## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |     √     |            |          |         |
| RaspberryPi4 |           |            |     √    |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## History

- 2018-01-03 - 1.0.0 版本
- 2021-12-21 - 1.0.1 版本

## Credits

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))