import requests
import json
import random
import argparse
import time
import base64


# Handle Arguments for Host, Port, etc
Parser = argparse.ArgumentParser(description="BrainGenix-API Simple Python Test Script")
Parser.add_argument("-Host", default="localhost", type=str, help="Host to connect to")
Parser.add_argument("-Port", default=8000, type=int, help="Port number to connect to")
Parser.add_argument("--NumSims", default=1, type=int, help="Number of simulations to create")
Parser.add_argument("--NumShapes", default=1, type=int, help="Number of shapes to create")
Parser.add_argument("--NumIters", default=1, type=int, help="Number of iterations to generate")
Args = Parser.parse_args()



#BaseURI = "http://api.braingenix.org/"
BaseURI = f"http://{Args.Host}:{Args.Port}/"

NumSims = Args.NumSims
NumShapes = Args.NumShapes
NumIters = Args.NumIters


def activity_simulations_tests():
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

def create_spheres(SimID:int, DoneShapes:int)->tuple:
    # Test create sphere
    DoneShapes = 0
    Offset = 0
    for x in range(NumShapes):
        # PyList = [random.randint(5,5)/4, random.randint(5,5)/4, random.randint(5,5)/4]
        Positions = [[0,0,4], [0,6,3], [1,2,1], [4,1,2], [3,3,2], [5,1,3], [7,6,2], [6,5,2], [7,6,4], [6,4,3]]
        for i in range(30):
             Positions.append([random.randint(0,8), random.randint(0,8), random.randint(0,8)])
        for i in range(len(Positions)):
            DoneShapes+=1
            PyList = Positions[i]
            MYLIST = json.dumps(PyList)
            r = requests.get(f"{BaseURI}NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&Radius_um=1.5&Center_um={MYLIST}")
            print("Shape/Sphere/Create", r.content)

            if i == 0:
                Offset = r.json()["ShapeID"]
    return DoneShapes, Offset

def create_boxes(SimID:int, DoneShapes:int)->int:
    # Test create Box
    for x in range(NumShapes):
        # PyList = [random.randint(-40,40)/4, random.randint(-40,40)/4, random.randint(-40,40)/4]
        DoneShapes+=1
        PyList = [0, 0, 0]
        MYLIST = json.dumps(PyList)
        Rotation = json.dumps([0,0,0.3])
        Dimensions = json.dumps([2, 3, 2])
        r = requests.get(f"{BaseURI}NES/Geometry/Shape/Box/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&CenterPosition_um={MYLIST}&Dimensions_um={Dimensions}&Rotation_rad={Rotation}")
        print("Shape/Box/Create",r.content)
    return DoneShapes

def create_cylinders(SimID:int, DoneShapes:int)->int:
    # Test create cylinder
    MYLIST1 = json.dumps([0,0,0])
    MYLIST2 = json.dumps([5,5,0])
    radius1 = 0.4
    radius2 = 0.2

    # Let's put some cylinders at the ends to ensure the locations are correct.
    r = requests.get(f"{BaseURI}NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&Radius_um=1.5&Center_um={MYLIST1}")
    print("Shape/Sphere/Create", r.content)
    DoneShapes+=1

    r = requests.get(f"{BaseURI}NES/Geometry/Shape/Sphere/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&Radius_um=1.5&Center_um={MYLIST2}")
    print("Shape/Sphere/Create", r.content)
    DoneShapes+=1

    #for x in range(NumShapes):
    getstr = f"{BaseURI}NES/Geometry/Shape/Cylinder/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&Point1Radius_um={radius1}&Point1Position_um={MYLIST1}&Point2Radius_um={radius2}&Point2Position_um={MYLIST2}"
    print("The get string: "+getstr)
    r = requests.get(getstr)
    print("Shape/Cylinder/Create",r.content)
    DoneShapes+=1

    return DoneShapes

def create_BS_compartments(SimID:int, Offset:int, DoneShapes:int):
    # Test create BS Compartment
    for x in range(DoneShapes):
        r = requests.get(f"{BaseURI}NES/Compartment/BS/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&ShapeID={x + Offset}&MembranePotential_mV=0.0&SpikeThreshold_mV=0.0&DecayTime_ms=0.0")
        print("Compartment/BS/Create",r.content)

def create_receptor_types():
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

def create_recording_devices():
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

# def show_model(NumSims:int):
#     # Test Simulation BuildMesh
#     for x in range(NumSims):
#         r = requests.get(f"{BaseURI}NES/Simulation/BuildMesh?AuthKey=MyVerySecureToken&SimulationID={x}")
#         print("Sim/BuildMesh", r.content)

def init_VSDA(SimID:int):
    #--- Test Simulation VSDA Routes ---# 
    r = requests.get(f"{BaseURI}NES/VSDA/EM/Initialize?AuthKey=MyVerySecureToken&SimulationID={SimID}")
    print("Sim/VSDA/EM/Initialize", r.content)

def scan_EM_1(NumSims:int):
    # Test VSDA EM SetupMicroscope
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={x}&PixelResolution_nm=3&ImageWidth_px=512&ImageHeight_px=512&SliceThickness_nm=20&ScanRegionOverlap_percent=10")
        print("VSDA/EM/SetupMicroscope", r.content)

    # Test VSDA EM DefineScanRegion
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/DefineScanRegion?AuthKey=MyVerySecureToken&SimulationID={x}&Point1_um=(0,0,0)&Point2_um=(0,0,0)")
        print("VSDA/EM/DefineScanRegion", r.content)

    # Test VSDA EM SetupMicroscope
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/QueueRenderOperation?AuthKey=MyVerySecureToken&SimulationID={x}&ScanRegionID=0")

def scan_EM_2(SimID:int):
    '''
    - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
    - (float) `PixelResolution_nm` Number of nanometers of resolution for each pixel.  
    - (int) `ImageWidth_px` Set the width of the image in pixels.  
    - (int) `ImageHeight_px` Set the height of the image in pixels.  
    - (float) `SliceThickness_nm` Set the thickness of each slice in nanometers.  
    - (float) `ScanRegionOverlap_percent` Set the overlap for the resulting image stacks.  
    '''
    r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={SimID}&PixelResolution_nm=0.1&ImageWidth_px=4096&ImageHeight_px=4096&SliceThickness_nm=100&ScanRegionOverlap_percent=10")
    print("Sim/VSDA/EM/SetupMicroscope", r.content)


    '''
    - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
    - (vec3) `Point1_um` (X,Y,Z) World space location of one corner of the rectangular prism enclosing the target scan region.  
    - (vec3) `Point2_um` (X,Y,Z) World space location of the other corner of the rectangular prism enclosing the target scan region.  
    '''
    Point1 = json.dumps([-1,-1,-1])
    Point2 = json.dumps([9,9,5])
    r = requests.get(f"{BaseURI}NES/VSDA/EM/DefineScanRegion?AuthKey=MyVerySecureToken&SimulationID={SimID}&Point1_um={Point1}&Point2_um={Point2}")
    print("Sim/VSDA/EM/DefineScanRegion", r.content)
    ScanRegionID = r.json()["ScanRegionID"]


    '''
    - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
    - (bgScanRegionID) `ScanRegionID` ID of the scan region to be rendered.  
    '''
    r = requests.get(f"{BaseURI}NES/VSDA/EM/QueueRenderOperation?AuthKey=MyVerySecureToken&SimulationID={SimID}&ScanRegionID={ScanRegionID}")
    print("Sim/VSDA/EM/QueueRenderOperation", r.content)

    '''
    - (bgSimulationID) `SimulationID` ID of simulation to get the status for.  
    '''
    StatusCode = 0
    while (StatusCode != 5):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/GetRenderStatus?AuthKey=MyVerySecureToken&SimulationID={SimID}")
        print("Sim/VSDA/EM/GetRenderStatus", r.content)
        StatusCode = r.json()["RenderStatus"]
        time.sleep(2)

    '''
    - (bgSimulationID) `SimulationID` ID of simulation to get the ImageStack for.  
    '''
    r = requests.get(f"{BaseURI}NES/VSDA/EM/GetImageStack?AuthKey=MyVerySecureToken&SimulationID={SimID}&ScanRegionID={ScanRegionID}")
    print("Sim/VSDA/EM/GetImageStack", r.content)
    ImageHandles = r.json()["RenderedImages"]


    for ImageHandle in ImageHandles:
        '''
        - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
        - (string) `ImageHandle` String containing the image handle that needs to be grabbed from. 
        '''
        r = requests.get(f"{BaseURI}NES/VSDA/EM/GetImage?AuthKey=MyVerySecureToken&SimulationID={SimID}&ImageHandle={ImageHandle}", timeout=999)
        print("Sim/VSDA/EM/GetImage (JSON omitted due to size)")
        ImageData = bytes(r.json()["ImageData"], 'utf-8')
        print(f"Saving Image As: '{ImageHandle.split('/')[1]}'")

        with open(ImageHandle.split("/")[1],"wb") as FileHandler:
            FileHandler.write(base64.decodebytes(ImageData))



# def run_debug():
#     JSONParams = json.dumps(0.1)
#     r = requests.get(f"{BaseURI}NES/Debug?AuthKey=MyVerySecureToken&Query={JSONParams}")
#     print("Sim/Debug", r.content)

def scan_EM_3(NumSims:int):
    # Test VSDA EM Initialize
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/Initialize?AuthKey=MyVerySecureToken&SimulationID={x}")
        print("VSDA/EM/Initialize", r.content)

    # Test VSDA EM SetupMicroscope
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={x}&PixelResolution_nm=3&ImageWidth_px=512&ImageHeight_px=512&SliceThickness_nm=20&ScanRegionOverlap_percent=10")
        print("VSDA/EM/SetupMicroscope", r.content)

    # Test VSDA EM GetImageStack
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/GetImageStack?AuthKey=MyVerySecureToken&SimulationID={x}&ScanRegionID=2")
        print("VSDA/EM/GetImageStack", r.content)

    # Test VSDA EM SetupMicroscope
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={x}&PixelResolution_nm=3&ImageWidth_px=512&ImageHeight_px=512&SliceThickness_nm=20&ScanRegionOverlap_percent=10")
        print("VSDA/EM/SetupMicroscope", r.content)
       
    # Test VSDA EM GetRenderStatus
    for x in range(NumSims):
        r = requests.get(f"{BaseURI}NES/VSDA/EM/GetRenderStatus?AuthKey=MyVerySecureToken&SimulationID={x}&ScanRegionID=1")
        print("VSDA/EM/SetupMicroscope", r.content)

# -----------------------------------------------------------------------------------

# Test Simulation Create
SimID = 0
for x in range(NumSims):
    r = requests.get(f"{BaseURI}NES/Simulation/Create?AuthKey=MyVerySecureToken&SimulationName=mySim")
    print("Sim/Create", r.content)
    SimID = r.json()["SimulationID"]


for _ in range(NumIters):

    #activity_simulations_tests()

    Offset=0
    DoneShapes=0

    #DoneShapes, Offset=create_spheres(SimID, DoneShapes)

    #DoneShapes=create_boxes(SimID, DoneShapes)

    DoneShapes=create_cylinders(SimID, DoneShapes)

    create_BS_compartments(SimID, Offset, DoneShapes)

    #create_receptor_types()

    #create_recording_devices()

    #show_model(NumSims)

    init_VSDA(SimID)

    #scan_EM_1(NumSims)

    scan_EM_2(SimID)

    #run_debug()

    #scan_EM_3(NumSims)
