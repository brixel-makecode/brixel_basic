# -*- coding:utf-8 -*-
'''!
  @file        gravity_TDS_example.py
  @brief       运行demogravity_TDS_example例程能够获取水中TDS数据并打印出来
  @copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT License (MIT)
  @author      TangJie(jie.tang@dfrobot.com)
  @version     V1.0.1
  @date        2021-12-21
  @url         https://github.com/DFRobot/DFRobot_GravityTDS
'''
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))

from dfrobot_gravity_tds import *

tds = DFRobot_GravityTDS()

tds.set_pin(tds.A1)
tds.set_adc_range(4096.0)
tds.set_aref(3.3)
while True:
  tds.update()
  print("%.2f" % tds.get_tds_value())
  time.sleep(1)