#!/bin/python3

import requests

url = 'http://localhost:8000/NES?AuthKey=MyVerySecureToken'
myobj = {'somekey': 'somevalue'}

x = requests.post(url, json = myobj)

print(x.text)
