# -*- coding: utf-8 -*-
"""
Created on Tue Nov 22 15:45:11 2022

@author: Patryk
"""

import requests
from bs4 import BeautifulSoup

def download():
    my_url = "http://192.168.0.52/"
    my_html = requests.get(my_url)
    my_soup = BeautifulSoup(my_html.text, 'html.parser')
    my_temp = str(my_soup.find_all("span",id = "temp"))
    my_hum = str(my_soup.find_all("span",id = "hum"))
    my_pres = str(my_soup.find_all("span",id = "pres"))
    my_gas = str(my_soup.find_all("span",id = "gas"))
    my_sealevel = str(my_soup.find_all("span",id = "SEALEVEL"))

    temperature = my_temp[17:22:1]
    humidity = my_hum[16:21:1]
    pressure = my_pres[17:23:1]
    gas = my_gas[16:21:1]
    sealevel = my_sealevel[21:27:1]
    return {'temp':temperature,'hum':humidity,'pres':pressure,'gas':gas,'sealvl':sealevel}
