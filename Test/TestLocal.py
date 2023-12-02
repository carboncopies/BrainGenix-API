import requests
import json
import random
import argparse


# Handle Arguments for Host, Port, etc
Parser = argparse.ArgumentParser(description="BrainGenix-API Simple Python Test Script")
Parser.add_argument("-Host", default="localhost", type=str, help="Host to connect to")
Parser.add_argument("-Port", default=8000, type=int, help="Port number to connect to")
Parser.add_argument("--NumSims", default=1, type=int, help="Number of simulations to create")
Parser.add_argument("--NumShapes", default=1, type=int, help="Number of shapes to create")
Parser.add_argument("--NumIters", default=1, type=int, help="Number of iterations to generate")
Args = Parser.parse_args()



# BaseURI = "http://api.braingenix.org/"
BaseURI = f"http://{Args.Host}:{Args.Port}/"

NumSims = Args.NumSims
NumShapes = Args.NumShapes
NumIters = Args.NumIters


# Test Simulation Create
for x in range(NumSims):
    r = requests.get(f"{BaseURI}NES/Simulation/Create?AuthKey=MyVerySecureToken&SimulationName=mySim")
    print("Sim/Create", r.content)


for _ in range(NumIters):

    # Test Simulation Reset
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/Simulation/Reset?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("Sim/Reset", r.content)

    # Test Simulation RunFor
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/Simulation/RunFor?AuthKey=MyVerySecureToken&SimulationID={x}&Runtime_ms={x}.0")
        print("Sim/RunFor", r.content)

    # Test Simulation RecordAll
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/Simulation/RecordAll?AuthKey=MyVerySecureToken&SimulationID={x}&MaxRecordTime_ms={x}.0")
        print("Sim/RecordAll", r.content)

    # Test Simulation GetRecording
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/Simulation/GetRecording?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("Sim/GetRecording", r.content)

    # Test Simulation GetStatus
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/Simulation/GetStatus?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("Sim/GetStatus", r.content)

    # Test create sphere
    for x in range(NumShapes):
        # PyList = [random.randint(5,5)/4, random.randint(5,5)/4, random.randint(5,5)/4]
        PyList = [0,0,4]
        MYLIST = json.dumps(PyList)
        r = requests.get(f"{BaseURI}NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID=0&Radius_um=1.5&Center_um={MYLIST}")
        print("Shape/Sphere/Create", r.content)

    # Test create Box
    for x in range(NumShapes):
        # PyList = [random.randint(-40,40)/4, random.randint(-40,40)/4, random.randint(-40,40)/4]
        PyList = [0, 0, 0]
        MYLIST = json.dumps(PyList)
        Rotation = json.dumps([0.1,0,0])
        Dimensions = json.dumps([1, 1, 1])
        r = requests.get(f"{BaseURI}NES/Geometry/Shape/Box/Create?AuthKey=MyVerySecureToken&SimulationID=0&CenterPosition_um={MYLIST}&Dimensions_um={Dimensions}&Rotation_rad={Rotation}")
        print("Shape/Box/Create",r.content)

    # # Test create cylinder
    # PyList = [1,2,3]
    # MYLIST = json.dumps(PyList)
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Geometry/Shape/Cylinder/Create?AuthKey=MyVerySecureToken&SimulationID=0&Point1Radius_um=5.2&Point1Position_um={MYLIST}&Point2Radius_um=8.2&Point2Position_um={MYLIST}")
    #     print("Shape/Cylinder/Create",r.content)



    # Test create BS Compartment
    for x in range(NumShapes*2):
        r = requests.get(f"{BaseURI}NES/Compartment/BS/Create?AuthKey=MyVerySecureToken&SimulationID=0&ShapeID={x}&MembranePotential_mV=0.0&SpikeThreshold_mV=0.0&DecayTime_ms=0.0")
        print("Compartment/BS/Create",r.content)

    # # Test create Staple
    # PyList = [1,2,3]
    # MYLIST = json.dumps(PyList)
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Connection/Staple/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&DestinationCompartmentID={x}")
    #     print("Connection/Staple/Create",r.content)


    # # Test create Receptor
    # PyList = [1,2,3]
    # MYLIST = json.dumps(PyList)
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Connection/Receptor/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&DestinationCompartmentID={x}&Conductance_nS=0.0&TimeConstant_ns=0.0&ReceptorLocation_um={MYLIST}")
    #     print("Connection/Receptor/Create",r.content)

    # # Test create PatchClampDAC
    # PyList = [1,2,3]
    # MYLIST = json.dumps(PyList)
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Tool/PatchClampDAC/Create?AuthKey=MyVerySecureToken&SimulationID=0&DestinationCompartmentID={x}&ClampLocation_um={MYLIST}")
    #     print("Tool/PatchClampDAC/Create", r.content)

    # # Test PatchClampDAC SetOutputList
    # PyList = list(range(10))
    # MYLIST = json.dumps(PyList)
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Tool/PatchClampDAC/SetOutputList?AuthKey=MyVerySecureToken&SimulationID=0&TargetDAC={x}&Timestep_ms=0.1&DACVoltages_mV={MYLIST}")
    #     print("Tool/PatchClampDAC/SetOutputList", r.content)

    # # Test create PatchClampADC
    # PyList = [1,2,3]
    # MYLIST = json.dumps(PyList)
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Tool/PatchClampADC/Create?AuthKey=MyVerySecureToken&SimulationID=0&SourceCompartmentID={x}&ClampLocation_um={MYLIST}")
    #     print("Tool/PatchClampADC/Create", r.content)

    # # Test PatchClampADC SetSampleRate
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Tool/PatchClampADC/SetSampleRate?AuthKey=MyVerySecureToken&SimulationID=0&TargetADC={x}&Timestep_ms={x}.0")
    #     print("Tool/PatchClampADC/SetSampleRate", r.content)

    # # Test PatchClampADC GetRecordedData
    # for x in range(NumShapes):
    #     r = requests.get(f"{BaseURI}NES/Tool/PatchClampADC/GetRecordedData?AuthKey=MyVerySecureToken&SimulationID=0&TargetADC={x}")
    #     print("Tool/PatchClampADC/GetRecordedData", r.content)

    # # Test Simulation BuildMesh
    # for x in range(NumSims):
    #     r = requests.get(f"{BaseURI}NES/Simulation/BuildMesh?AuthKey=MyVerySecureToken&SimulationID={x}")
    #     print("Sim/BuildMesh", r.content)


    JSONParams = json.dumps(0.1)
    r = requests.get(f"{BaseURI}NES/Debug?AuthKey=MyVerySecureToken&Query={JSONParams}")
    print("Sim/Debug", r.content)

    # # Test VSDA EM Initialize
    # for x in range(NumSims):
    #     r = requests.get(f"{BaseURI}NES/VSDA/EM/Initialize?AuthKey=MyVerySecureToken&SimulationID={x}")
    #     print("VSDA/EM/Initialize", r.content)

    # # Test VSDA EM SetupMicroscope
    # for x in range(NumSims):
    #     r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={x}&PixelResolution_nm=3&ImageWidth_px=512&ImageHeight_px=512&SliceThickness_nm=20&ScanRegionOverlap_percent=10")
    #     print("VSDA/EM/SetupMicroscope", r.content)

    # # Test VSDA EM GetImageStack
    # for x in range(NumSims):
    #     r = requests.get(f"{BaseURI}NES/VSDA/EM/GetImageStack?AuthKey=MyVerySecureToken&SimulationID={x}&ScanRegionID=2")
    #     print("VSDA/EM/GetImageStack", r.content)

    # # Test VSDA EM SetupMicroscope
    # for x in range(NumSims):
    #     r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={x}&PixelResolution_nm=3&ImageWidth_px=512&ImageHeight_px=512&SliceThickness_nm=20&ScanRegionOverlap_percent=10")
    #     print("VSDA/EM/SetupMicroscope", r.content)
       
    # # Test VSDA EM GetRenderStatus
    # for x in range(NumSims):
    #     r = requests.get(f"{BaseURI}NES/VSDA/EM/GetRenderStatus?AuthKey=MyVerySecureToken&SimulationID={x}&ScanRegionID=1")
    #     print("VSDA/EM/SetupMicroscope", r.content)
