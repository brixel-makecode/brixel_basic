# -*- coding:utf-8 -*-
'''!
  @file        dfrobot_gravity_tds.py
  @brief       这是SEN0244的库，库中实现将模拟数据值转换为TDS值数据
  @copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT License (MIT)
  @author      TangJie(jie.tang@dfrobot.com)
  @version     V1.0.1
  @date        2021-12-21
  @url         https://github.com/DFRobot/DFRobot_GravityTDS
'''
import time
import sys
import os
sys.path.append("/home/pi/DFRobot_RaspberryPi_Expansion_Board/raspberry")

from DFRobot_RaspberryPi_Expansion_Board import DFRobot_Expansion_Board_IIC


class DFRobot_GravityTDS():

  A0 = 0X00
  A1 = 0X01
  A2 = 0X02
  A3 = 0X03
  
  
  def __init__(self):
    '''!
      @brief 定义DFRobot_GravityTDS基类，并初始换adc
    '''
    self._board = DFRobot_Expansion_Board_IIC(1,0x10)
    self._pin = self._board.A0
    self._temperature = 25.0
    self._aref = 5.0
    self._adc_range = 1024.0
    self._k_value = 1.0
    self._tds_value = 0.0
    self._ec_value25 = 0.0
    #self._board_detect()
    while self._board.begin() != self._board.STA_OK:
      print("board begin faild")
      time.sleep(1)
    print("board begin success")
    self._board.set_adc_enable()
    
  def set_temperature(self, temperature):
    '''!
      @brief 设置温度补偿数据
      @param temperature 温度补偿数据
    '''
    self._temperature = temperature
    
  def set_pin(self, pin):
    '''!
      @brief 设置模拟数据采集引脚
      @param pin 模拟数据采集引脚
    '''
    if pin == self.A0:
      self._pin = self._board.A0
    elif pin == self.A1:
      self._pin = self._board.A1
    elif pin == self.A2:
      self._pin = self._board.A2
    elif pin == self.A3:
      self._pin = self._board.A3
    
  def set_aref(self, value):
    '''!
      @brief 设置ADC参考电压
      @param value ADC参考电压
    '''
    self._aref = value
    
  def set_adc_range(self, range):
    '''!
      @brief 设置ADC范围，10bit为1024，12bit为4096
      @param ADC范围
    '''
    self._adc_range = range
    
  def set_k_value(self, value):
    '''!
      @brief 设置TDS K 值
      @param value K值数据
    '''
    self._k_value = value 
    
  def get_tds_value(self):
    '''!
      @brief 获取TDS数据
      @return 返回TDS数据
    '''
    return self._tds_value
    
  def get_ec_value(self):
    '''!
      @brief 获取ec数据
      @return 返回ec数据
    '''
    return self._ec_value25
    
  def update(self):
    '''!
      @brief 获取ADC数据并计算
    '''
    self._analog_value = self._board.get_adc_value(self._pin)
    self._voltage = self._analog_value / self._adc_range * self._aref
    self._ec_value = (133.42 * self._voltage * self._voltage * self._voltage - 255.86 * self._voltage * self._voltage + 857.39 * self._voltage) * self._k_value
    self._ec_value25 = self._ec_value / (1.0 + 0.02 * (self._temperature - 25.0))
    self._tds_value = self._ec_value25 * 0.5