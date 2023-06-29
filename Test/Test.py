import requests
import json


NumSims = 10
NumShapes = 50

NumIters = 1000



for _ in range(NumIters):

    # Create Sim
    for x in range(10):
        r = requests.get("http://localhost:8000/NES/Simulation/Create?AuthKey=MyVerySecureToken&SimulationName=mySim")
        print(r.content)

    # Test create sphere
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID=0&Radius_nm=5.2&Center_nm={MYLIST}")
        print(r.content)

    # Test create cylinder
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Geometry/Shape/Cylinder/Create?AuthKey=MyVerySecureToken&SimulationID=0&Point1Radius_nm=5.2&Point1Position_nm={MYLIST}&Point2Radius_nm=8.2&Point2Position_nm={MYLIST}")
        print(r.content)

    # Test create Box
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Geometry/Shape/Box/Create?AuthKey=MyVerySecureToken&SimulationID=0&CenterPosition_nm={MYLIST}&Dimensions_nm={MYLIST}&Rotation_rad={MYLIST}")
        print(r.content)

    # Test create BS Compartment
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Compartment/BS/Create?AuthKey=MyVerySecureToken&SimulationID=0&ShapeID={x}&MembranePotential_mV=0.0&SpikeThreshold_mV=0.0&DecayTime_ms=0.0")
        print(r.content)

    # Test create Staple
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Connection/Staple/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&DestinationCompartmentID={x}")
        print(r.content)


    # Test create Receptor
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Connection/Receptor/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&DestinationCompartmentID={x}&Conductance_nS=0.0&TimeConstant_ns=0.0&ReceptorLocation_nm={MYLIST}")
        print(r.content)

    # Test create PatchClampDAC
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"http://localhost:8000/NES/Tool/PatchClampDAC/Create?AuthKey=MyVerySecureToken&SimulationID=0&DestinationCompartmentID={x}&ClampLocation_nm={MYLIST}")
        print(r.content)

