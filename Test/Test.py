import requests
import json


BaseURI = "http://api.braingenix.org/"
# BaseURI = "http://localhost:8000/"

NumSims = 10
NumShapes = 50

NumIters = 10000


# Test Simulation Create
for x in range(10):
    r = requests.get(f"{BaseURI}NES/Simulation/Create?AuthKey=MyVerySecureToken&SimulationName=mySim")
    print("Sim/Create", r.content)


for _ in range(NumIters):

    # Test Simulation Reset
    for x in range(10):
        r = requests.get(f"{BaseURI}NES/Simulation/Reset?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("Sim/Reset", r.content)

    # Test Simulation RunFor
    for x in range(10):
        r = requests.get(f"{BaseURI}NES/Simulation/RunFor?AuthKey=MyVerySecureToken&SimulationID={x}&Runtime_ms={x}.0")
        print("Sim/RunFor", r.content)

    # Test Simulation RecordAll
    for x in range(10):
        r = requests.get(f"{BaseURI}NES/Simulation/RecordAll?AuthKey=MyVerySecureToken&SimulationID={x}&MaxRecordTime_ms={x}.0")
        print("Sim/RecordAll", r.content)

    # Test Simulation GetRecording
    for x in range(10):
        r = requests.get(f"{BaseURI}NES/Simulation/GetRecording?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("Sim/GetRecording", r.content)

    # Test Simulation GetStatus
    for x in range(10):
        r = requests.get(f"{BaseURI}NES/Simulation/GetStatus?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("Sim/GetStatus", r.content)

    # Test create sphere
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID=0&Radius_um=5.2&Center_um={MYLIST}")
        print("Shape/Sphere/Create", r.content)

    # Test create cylinder
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Geometry/Shape/Cylinder/Create?AuthKey=MyVerySecureToken&SimulationID=0&Point1Radius_um=5.2&Point1Position_um={MYLIST}&Point2Radius_um=8.2&Point2Position_um={MYLIST}")
        print("Shape/Cylinder/Create",r.content)

    # Test create Box
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Geometry/Shape/Box/Create?AuthKey=MyVerySecureToken&SimulationID=0&CenterPosition_um={MYLIST}&Dimensions_um={MYLIST}&Rotation_rad={MYLIST}")
        print("Shape/Box/Create",r.content)

    # Test create BS Compartment
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Compartment/BS/Create?AuthKey=MyVerySecureToken&SimulationID=0&ShapeID={x}&MembranePotential_mV=0.0&SpikeThreshold_mV=0.0&DecayTime_ms=0.0")
        print("Compartment/BS/Create",r.content)

    # Test create Staple
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Connection/Staple/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&DestinationCompartmentID={x}")
        print("Connection/Staple/Create",r.content)


    # Test create Receptor
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Connection/Receptor/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&DestinationCompartmentID={x}&Conductance_nS=0.0&TimeConstant_ns=0.0&ReceptorLocation_um={MYLIST}")
        print("Connection/Receptor/Create",r.content)

    # Test create PatchClampDAC
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Tool/PatchClampDAC/Create?AuthKey=MyVerySecureToken&SimulationID=0&DestinationCompartmentID={x}&ClampLocation_um={MYLIST}")
        print("Tool/PatchClampDAC/Create", r.content)

    # Test PatchClampDAC SetOutputList
    PyList = list(range(10))
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Tool/PatchClampDAC/SetOutputList?AuthKey=MyVerySecureToken&SimulationID=0&TargetDAC={x}&Timestep_ms=0.1&DACVoltages_mV={MYLIST}")
        print("Tool/PatchClampDAC/SetOutputList", r.content)

    # Test create PatchClampADC
    PyList = [1,2,3]
    MYLIST = json.dumps(PyList)
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Tool/PatchClampADC/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&ClampLocation_um={MYLIST}")
        print("Tool/PatchClampADC/Create", r.content)

    # Test PatchClampADC SetSampleRate
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Tool/PatchClampADC/SetSampleRate?AuthKey=MyVerySecureToken&SimulationID=0&TargetADC={x}&Timestep_ms={x}.0")
        print("Tool/PatchClampADC/SetSampleRate", r.content)

    # Test PatchClampADC GetRecordedData
    for x in range(NumShapes):
        r = requests.get(f"{BaseURI}NES/Tool/PatchClampADC/GetRecordedData?AuthKey=MyVerySecureToken&SimulationID=0&TargetADC={x}")
        print("Tool/PatchClampADC/GetRecordedData", r.content)