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



# BaseURI = "http://api.braingenix.org/"
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


def CreateCylinder(SimID:int, DoneShapes:int, Point1:list, Point2:list, Radius1:float = 0.1, Radius2:float = 0.1):

    Point1JSON = json.dumps(Point1)
    Point2JSON = json.dumps(Point2)

    getstr = f"{BaseURI}NES/Geometry/Shape/Cylinder/Create?AuthKey=MyVerySecureToken&SimulationID={SimID}&Point1Radius_um={Radius1}&Point1Position_um={Point1JSON}&Point2Radius_um={Radius2}&Point2Position_um={Point2JSON}"
    r = requests.get(getstr)
    print("Shape/Cylinder/Create",r.content)

    DoneShapes+=1
    return DoneShapes



def create_cylinders(SimID:int, DoneShapes:int)->int:

    
    

    Sections = [
        [[0.25, 6.5, 0.0], [0.25, 8.5, 0.0]], [[1.25, 6.5, 0.0], [1.25, 8.5, 0.0]], [[0.25, 7.5, 0.0], [1.25, 7.5, 0.0]], # Letter H
        [[2.0, 6.5, 0.0], [2.5, 8.5, 0.0]],   [[2.5, 8.5, 0.0], [3.0, 6.5, 0.0]],   [[2.25, 7.5, 0.0], [2.75, 7.5, 0.0]], # Letter A
        [[3.75,6.5,0.0], [3.75,8.5,0.0]],     [[3.75,8.5,0.0],[4.75,7.75,0.0]],     [[4.75,7.75,0.0],[3.75,7.0,0.0]],     # Letter P
        [[5.25,6.5,0.0], [5.25,8.5,0.0]],     [[5.25,8.5,0.0],[6.25,7.75,0.0]],     [[6.25,7.75,0.0],[5.25,7.0,0.0]],     # Letter P
        [[6.75,8.5,0.0],[7.25,7.75,0.0]],     [[7.25,7.75,0.0],[7.75,8.5,0.0]],     [[7.25,6.5,0.0],[7.25,7.75,0.0]],     # Letter Y 
    #---------------------------------------------------------------------------------------------------------------------------------
        [[-0.75, 3.5, 0.0], [-0.75, 5.5, 0.0]], [[-0.25, 3.5, 0.0], [-0.25, 5.5, 0.0]], [[-0.75, 4.5, 0.0], [-0.25, 4.5, 0.0]], # Letter H
        [[0.25,3.5,0.0],    [0.75,3.5,0.0]],    [[0.25,5.5,0.0],[0.75,5.5,0.0]],        [[0.25,3.5,0.0],[0.25,5.5,0.0]],  [[0.75,3.5,0.0],[0.75,5.5,0.0]], #Letter O 
        [[1.25, 3.5,0.0],   [1.25,5.5,0.0]],  [[1.25,3.5,0.0],[1.75,3.5,0.0]],                                            #Letter L
        [[2.25, 3.41,0.0],[2.25,5.5,0.0]],                                                                                #Letter I 
        [[2.75,3.5,0.0],[2.75,5.5,0.0]], [[2.75,5.5,0.0],[3.25,4.5,0.0]], [[2.75,3.41,0.0],[3.25,4.5,0.0]],               #Letter D
        [[3.50, 3.5, 0.0], [4.00, 5.5, 0.0]],  [[4.00, 5.5, 0.0], [4.5, 3.5, 0.0]],  [[3.75, 4.5, 0.0],[4.25, 4.5, 0.0]],   #Letter A
        [[5.0,5.5,0.0],[5.25,4.5,0.0]], [[5.25,4.5,0.0],[5.5,5.5,0.0]], [[5.25,3.5,0.0],[5.25,4.5,0.0]],                  #Letter Y 
        [[6.25,5.5,0.0],[6.75,5.5,0.0]], [[6.25,5.5,0.0],[6.25,4.5,0.0]],[[6.25,4.5,0.0],[6.75,4.5,0.0]], [[6.75,4.5,0.0],[6.75,3.5,0.0]],[[6.25,3.5,0.0],[6.75,3.5,0.0]], #Letter S 

    ]

    print(Sections)

    for Section in Sections:

        Point1 = Section[0]
        Point2 = Section[1]

        DoneShapes = CreateCylinder(SimID, DoneShapes, Point1, Point2)


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


def init_VSDA(SimID:int):
    #--- Test Simulation VSDA Routes ---# 
    r = requests.get(f"{BaseURI}NES/VSDA/EM/Initialize?AuthKey=MyVerySecureToken&SimulationID={SimID}")
    print("Sim/VSDA/EM/Initialize", r.content)

def scan_EM_2(SimID:int):
    '''
    - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
    - (float) `PixelResolution_nm` Number of nanometers of resolution for each pixel.  
    - (int) `ImageWidth_px` Set the width of the image in pixels.  
    - (int) `ImageHeight_px` Set the height of the image in pixels.  
    - (float) `SliceThickness_nm` Set the thickness of each slice in nanometers.  
    - (float) `ScanRegionOverlap_percent` Set the overlap for the resulting image stacks.  
    '''
    r = requests.get(f"{BaseURI}NES/VSDA/EM/SetupMicroscope?AuthKey=MyVerySecureToken&SimulationID={SimID}&PixelResolution_nm=0.04&ImageWidth_px=512&ImageHeight_px=512&SliceThickness_nm=100&ScanRegionOverlap_percent=10")
    print("Sim/VSDA/EM/SetupMicroscope", r.content)


    '''
    - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
    - (vec3) `Point1_um` (X,Y,Z) World space location of one corner of the rectangular prism enclosing the target scan region.  
    - (vec3) `Point2_um` (X,Y,Z) World space location of the other corner of the rectangular prism enclosing the target scan region.  
    '''
    Point1 = json.dumps([-1,-1,-1])
    Point2 = json.dumps([9,9,0.5])
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
        r = requests.get(f"{BaseURI}NES/VSDA/EM/GetImage?AuthKey=MyVerySecureToken&SimulationID={SimID}&ImageHandle={ImageHandle}")
        print("Sim/VSDA/EM/GetImage (JSON omitted due to size)")
        ImageData = bytes(r.json()["ImageData"], 'utf-8')
        print(f"Saving Image As: '{ImageHandle.split('/')[1]}'")

        with open(ImageHandle.split("/")[1],"wb") as FileHandler:
            FileHandler.write(base64.decodebytes(ImageData))


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
