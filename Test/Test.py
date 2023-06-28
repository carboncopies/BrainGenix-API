import requests
import json




# Create Sim
for x in range(10):
    r = requests.get("http://localhost:8000/NES/Simulation/Create?AuthKey=MyVerySecureToken&SimulationName=mySim")
    print(r.content)

# Test create sphere
PyList = [1,2,3]
MYLIST = json.dumps(PyList)
for x in range(50):
    r = requests.get(f"http://localhost:8000/NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID=0&Radius_nm=5.2&Center_nm={MYLIST}")
    print(r.content)

# Test create cylinder
PyList = [1,2,3]
MYLIST = json.dumps(PyList)
for x in range(50):
    r = requests.get(f"http://localhost:8000/NES/Geometry/Shape/Cylinder/Create?AuthKey=MyVerySecureToken&SimulationID=0&Point1Radius_nm=5.2&Point1Position_nm={MYLIST}&Point2Radius_nm=8.2&Point2Position_nm={MYLIST}")
    print(r.content)