# API Design Spec

Note: Every API call below, except the Simulation/Create call, must also receive
the parameters:

- (bgSimulationID
) `SimulationID=` Simulation ID number.
- (AuthKey) `AuthKey=` Authorization key.

Note: On a nonsuccess status code, other parameters are *not* guarenteed to be provided. Always check the status code first.


# Enums

## bgStatus  
 - `0` Success
 - `1` General Failure
 - `2` Invalid Parameters Passed
 - `3` Upstream Gateway Unavailable
 - `4` Unauthorized/invaid token/no token provided
 - `5` Simulation Busy

## bgServiceStatus
 - `0` Healthy
 - `1` Degraded
 - `2` Failed
 - `3` Not Configured

## bgRenderStatus
 - `0` Ready
 - `1` Compressing
 - `2` Rendering
 - `3` Not Yet Rendered

# Routes  

## Diagnostic  

### Hello  

**URI**: `/Hello`  
**Request**:  
*Required Params*:  

**Response**:  
 - (string) `https://www.youtube.com/watch?v=dQw4w9WgXcQ` It's a rickroll lul.  


### Version  

**URI**: `/Diagnostic/Version`  
**Request**:  
*Required Params*:  

**Response**:  
 - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
 - (string) `Version=` Returns string containing the version of the API server.  


### Status

**URI**: `/Diagnostic/Status`  
**Request**:  
*Required Params*:  

**Response**:  
 - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
 - (string) `SystemState=` String indicating the current state of the service. Can be "Healthy", "Degraded", "Failed". 
 - (bgServiceStatus) `ServiceStateAPI=` Enum indicating the state of the API Service.  
 - (bgServiceStatus) `ServiceStateAPIDB=` Enum indicating the state of the API Database Service.  
 - (bgServiceStatus) `ServiceStateNES=` Enum indicating the state of the NES Service.  
 - (bgServiceStatus) `ServiceStateERS=` Enum indicating the state of the ERS Service.  
 - (bgServiceStatus) `ServiceStateSTS=` Enum indicating the state of the STS Service.  




## Authentication

### GetToken

**URI**: `/Auth/GetToken`  
**Request**:  
*Required Params*:  
 - (string) `Username=` Username for your account.  
 - (string) `Password=` Password for your account.  

**Response**:  
 - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
 - (string) `AuthKey=` If your username/password are valid, then the token will be returned.  



## Shapes

### Sphere - Create

**URI**: `/NES/Geometry/Shape/Sphere/Create?`  
**Request**:  
*Required Params*:  

- (float) `Radius_um=` The radius of the sphere in micrometers.
- (vec3) `Center_um=` Position of the center of the sphere in micrometers. Given as a json formatted list containing the three x,y,z coordinates.

*Optional Params*:  

- (string) `Name=` Optional name of the sphere.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).


### Sphere - BulkCreate

**URI**: `/NES/Geometry/Shape/Sphere/BulkCreate?`  
**Request**:  
*Required Params*:  

- (float list) `RadiusList_um=` List of radiuses of the spheres.  
- (float list) `CenterXList_um=` List of center X coordinates.  
- (float list) `CenterYList_um=` List of center Y coordinates.  
- (float list) `CenterZList_um=` List of center Z coordinates.  
- (string list) `NameList=` List of names of the spheres.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID list) `ShapeIDs=` List of returned shape IDs, will be empty on failure.


### Cylinder - Create

**URI**: `/NES/Geometry/Shape/Cylinder/Create?`  
**Request**:  
*Required Params*:  

- (float) `Point1Radius_um=` The radius at the cylinder's first point.
- (vec3) `Point1Position_um=` First endpoint of the cylinder's axis.
- (float) `Point2Radius_um=` The radius at the cylinder's second point.
- (vec3) `Point2Position_um=` Second endpoint of the cylinder's axis.

*Optional Params*:  

- (string) `Name=` Optional name of the cylinder.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).


### Cylinder - BulkCreate

**URI**: `/NES/Geometry/Shape/Cylinder/BulkCreate?`  
**Request**:  
*Required Params*:  

- (float list) `Point1RadiusList_um=` List of radiuses at the first point.  
- (float list) `Point2RadiusList_um=` List of radiuses at the first point.  
- (float list) `Point1XList_um=` List of point 1 X coordinates.  
- (float list) `Point1YList_um=` List of point 1 Y coordinates.  
- (float list) `Point1ZList_um=` List of point 1 Z coordinates.  
- (float list) `Point2XList_um=` List of point 2 X coordinates.  
- (float list) `Point2YList_um=` List of point 2 Y coordinates.  
- (float list) `Point2ZList_um=` List of point 2 Z coordinates.  
- (string list) `NameList=` List of names of the cylinders.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID list) `ShapeIDs=` List of returned shape IDs, will be empty on failure.


### Box - Create

**URI**: `/NES/Geometry/Shape/Box/Create?`  
**Request**:  
*Required Params*:  

- (vec3) `CenterPosition_um=` Position of the center of the box in world space coordinates.
- (vec3) `Dimensions_um=` (X,Y,Z) dimensions of the box in nm.
- (vec3) `Rotation_rad=` (X,Y,Z) rotation of the box in radians.

*Optional Params*:  

- (string) `Name=` Optional name of the box.

**Response**:    

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).


### Box - BulkCreate

**URI**: `/NES/Geometry/Shape/Box/BulkCreate?`  
**Request**:  
*Required Params*:  

- (float list) `CenterXList_um=` List of center X coordinates.  
- (float list) `CenterYList_um=` List of center Y coordinates.  
- (float list) `CenterZList_um=` List of center Z coordinates.  
- (float list) `DimensionsXList_um=` List of X dimensions for the boxes.  
- (float list) `DimensionsYList_um=` List of Y dimensions for the boxes.  
- (float list) `DimensionsZList_um=` List of Z dimensions for the boxes.  
- (float list) `RotationXList_rad=` List of X rotations in radians.  
- (float list) `RotationYList_rad=` List of Y rotations in radians.  
- (float list) `RotationZList_rad=` List of Z rotations in radians.  
- (string list) `NameList=` List of names of the boxes.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID list) `ShapeIDs=` List of returned shape IDs, will be empty on failure.


## Compartments

### BS - Create

**URI**: `/NES/Compartment/BS/Create?`  
**Request**:  
*Required Params*:  
- (bgShapeID) `ShapeID=` ID of an NES Shape.   
- (float) `MembranePotential_mV=` Membrane potential of the compartment in millivolts.  
- (float) `SpikeThreshold_mV=` Action potential spike threshold of the compartment in millivolts.  
- (float) `DecayTime_ms=` (tau_ahp) After hyperpolarization time constant in milliseconds.  
- (float) `RestingPotential_mV=` To be documented.  
- (float) `AfterHyperpolarizationAmplitude_mV=` to be documented.  

*Optional Params*:  
- (string) `Name=` Optional name of the compartment.  

**Response**:   

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgCompartmentID) `CompartmentID=` ID of the resulting compartment created here (if status indicates success, otherwise this is -1).


### BS - BulkCreate

**URI**: `/NES/Compartment/BS/BulkCreate?`  
**Request**:  
*Required Params*:  

- (bgShapeID list) `ShapeIDList=` List of ID of an NES Shapes.   
- (float list) `MembranePotentialList_mV=` List of Membrane potential of the compartment in millivolts.  
- (float list) `SpikeThresholdList_mV=` List of Action potential spike threshold of the compartment in millivolts.  
- (float list) `DecayTimeList_ms=` List of (tau_ahp) After hyperpolarization time constant in milliseconds.  
- (float list) `RestingPotentialList_mV=` To be documented.  
- (float list) `AfterHyperpolarizationAmplitudeList_mV=` to be documented.  
- (string list) `NameList=` List of names of the boxes.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgCompartmentID list) `CompartmentIDs=` List of returned compartment IDs, will be empty on failure.


## Connections

### Staple - Create

**URI** `/NES/Connection/Staple/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `SourceCompartmentID=` ID of the compartment whos data will be copied to the destination.
- (bgCompartmentID) `DestinationCompartmentID=` ID of the compartment whos data will be overwritten with the source.

*Optional Params*:  

- (string) `Name=` Optional name of the staple (clippy?).

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgConnectionID) `StapleID=` ID of the resulting staple connection created here (if status indicates success, otherwise this is -1).


### Receptor - Create

**URI** `/NES/Connection/Receptor/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `SourceCompartmentID=` ID of the compartment with presynaptic spike.
- (bgCompartmentID) `DestinationCompartmentID=` ID of the compartment receiving postsynaptic activity.
- (float) `Conductance_nS=` Conductance from source to destination in nanoSiemens.
- (float) `TimeConstantRise_ms=` Postsynaptic potential rise time constant in milliseconds.
- (float) `TimeConstantDecay_ms=` Postsynaptic potential decay time constant in milliseconds.
- (vec3) `ReceptorLocation_um=` (X,Y,Z) World space location of the receptor's base (where it intersects the compartment).

*Optional Params*:  

- (string) `Name=` Optional name of the connection.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgConnectionID) `ReceptorID=` ID of the resulting receptor connection created here (if status indicates success, otherwise this is -1).


### Neuron - Create

**URI** `/NES/Neuron/BSNeuron/Create?` 
**Request**:  
*Required Params*:  

- (bgShapeID) `SomaID=` ID of the shape object for the soma of the neuron (e.g. a sphere).
- (bgShapeID) `AxonID=` ID of the shape object for the axon of the neuron (e.g. a cylinder).
- (float) `MembranePotential_mV=` Membrane potential set at time of construction.
- (float) `RestingPotential_mV=` Membrane potential at rest.
- (float) `SpikeThreshold_mV=` Membrane potential at which the neuron will fire an action potential.
- (float) `DecayTime_ms=` Time constant for the decay of the after-hyperpolarization of the neuron.
- (float) `AfterHyperpolarizationAmplitude_mV=` Amplitude of the after-hyperpolarization of the neuron.
- (float) `PostsynapticPotentialRiseTime_ms=` Time constant of the rise-time of a post-synaptic potential arriving at the neuron.
- (float) `PostsynapticPotentialDecayTime_ms=` Time constant of the decay-time of a post-synaptic potential arriving at the neuron.
- (float) `PostsynapticPotentialAmplitude_mV=` Amplitude of a post-synaptic potential arriving at the neuron.

*Optional Params*:  

- (string) `Name=` Optional name of the neuron.
- (str) `NeuronTag=` String which can be used to group this neuron along with others. ** NEW **  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgNeuronID) `NeuronID=` ID of the resulting neuron (if status indicates success, otherwise this is -1).


Proposed Python client example (as shown in Python prototype code `BG_API.py`):

```
    Cfg = NES.Models.Neurons.BSNeuron.Configuration()
    Cfg.Name = name
    Cfg.SomaID = SomaID,
    Cfg.AxonID = AxonID
    Cfg.MembranePotential_mV = MembranePotential_mV
    Cfg.RestingPotential_mV = RestingPotential_mV
    Cfg.SpikeThreshold_mV = SpikeThreshold_mV
    Cfg.DecayTime_ms = DecayTime_ms
    Cfg.AfterHyperpolarizationAmplitude_mV = AfterHyperpolarizationAmplitude_mV
    Cfg.PostsynapticPotentialRiseTime_ms = PostsynapticPotentialRiseTime_ms
    Cfg.PostsynapticPotentialDecayTime_ms = PostsynapticPotentialDecayTime_ms
    Cfg.PostsynapticPotentialAmplitude_mV = PostsynapticPotentialAmplitude_mV
    neuron = Simulation.Sim.AddBSNeuron(Cfg)
```

### NeuralCircuit - Create      **NEW**

**URI** `/NES/NeuralCircuit/BSAlignedNC/Create?`    
**Request**:  

*Required Params*: 

- (int) `GeometryCollectionID?=` Number of cells in the neural circuit.

*Optional Params*:  

- (int) `NumCells=` Number of cells in the neural circuit.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.

## Tools

### DAC - Create

**URI** `/NES/Tool/PatchClampDAC/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `DestinationCompartmentID=` ID of the compartment receiving DAC output.
- (vec3) `ClampLocation_um=` (X,Y,Z) World space location of the DAC's connection in micrometers.

*Optional Params*:  

- (string) `Name=` Optional name of the DAC.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgPatchClampDACID) `PatchClampDACID=` ID of the resulting PatchClampDAC created here (if status indicates success, otherwise this is -1).


### DAC - Set Output List

**URI** `/NES/Tool/PatchClampDAC/SetOutputList?`  
**Request**:  
*Required Params*:  

- (bgPatchClampDACID) `TargetDAC=` ID of the DAC being configured.
- (float list) `DACVoltages_mV=` JSON formatted list of voltages to be played back by the DAC.
- (float) `Timestep_ms=` Simulation time for which each step in the DACVoltages is replayed. 

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.

### ADC - Create

**URI** `/NES/Tool/PatchClampADC/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `SourceCompartmentID=` ID of the compartment being read by the ADC output.
- (vec3) `ClampLocation_um=` (X,Y,Z) World space location of the ADC's connection in micrometers.

*Optional Params*:  

- (string) `Name=` Optional name of the ADC.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgPatchClampADCID) `PatchClampADCID=` ID of the resulting PatchClampADC created here (if status indicates success, otherwise this is -1).


### ADC - Set Sample Rate

**URI** `/NES/Tool/PatchClampADC/SetSampleRate?`  
**Request**:  
*Required Params*:  

- (bgPatchClampADCID) `TargetADC=` ID of the ADC being configured.
- (float) `Timestep_ms=` Sets the sample timestep rate for the ADC in milliseconds. (Note that this route implicitly clears the ADC, if it had any data that will be erased.)

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.


### ADC - Get Recorded Data

**URI** `/NES/Tool/PatchClampADC/GetRecordedData?`  
**Request**:  
*Required Params*:  

- (bgPatchClampADCID) `TargetADC=` ID of the ADC being read.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (float list) `RecordedData_mV=` JSON formatted list of voltages recorded by the ADC.
- (float) `Timestep_ms=` Gets the sample timestep rate for the ADC in milliseconds. 

## Simulation

### Simulation - Create

**URI** `/NES/Simulation/Create?`  
**Request**:  
*Required Params*:  

- (string) `SimulationName=` Set the name of your simulation. Probably bob. 

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgSimulationID) `SimulationID=` ID of the simulation created. Returns -1 on fail.


### Simulation - Save **NEW**

**URI** `/NES/Simulation/Save?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of the simulation to save.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (int) `SaveID=` Integer representing the save index. Use this to then load it later.  


### Simulation - Load **NEW**

**URI** `/NES/Simulation/Load?`  
**Request**:  
*Required Params*:  

- (int) `SaveID=` Integer representing the save index to load.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (bgSimulationID) `SimulationID=` ID of the simulation loaded.  


### Simulation - SaveErase **NEW**

**URI** `/NES/Simulation/SaveErase?`  
**Request**:  
*Required Params*:  

- (int) `SaveID=` Integer representing the save index to remove.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  



### Simulation - Reset

**URI** `/NES/Simulation/Reset?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of the simulation to reset. 

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.


### Simulation - Run For

**URI** `/NES/Simulation/RunFor?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of the simulation to run.
- (float) `Runtime_ms=` Number of milliseconds to simulate.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.


### Simulation - Record All

**URI** `/NES/Simulation/RecordAll?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to record.
- (float) `MaxRecordTime_ms=` Maximum number of milliseconds to record. -1 to record forever. 0 to disable/abort.

**Response**:  

- (bsStatus) `StatusCode=` Enum indicating the status of this APi call.


### Simulation - Get Recording

**URI** `/NES/Simulation/GetRecording?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to record.

**Response**:  

- (bsStatus) `StatusCode=` Enum indicating the status of this APi call.
- (JSON blob) `Recording=` JSON blob containing recorded data of all objects.


### Simulation - Get Status

**URI** `/NES/Simulation/GetStatus?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to get the run status of.

**Response**:  

- (bsStatus) `StatusCode=` Enum indicating the status of this APi call.
- (bool) `IsSimulating=` Is simulating true/false.
- (float) `RealWorldTimeRemaining_ms=` Float containing number of milliseconds estimated to be remaining in real-world time until current run call completes.
- (float) `RealWorldTimeElapsed_ms=` Float containing number of milliseconds elapsed since simulation started in real-world time measured since start of last run call.
- (float) `InSimulationTime_ms=` Float containing number of in-simulation milliseconds elapsed since last reset call.
- (float) `InSimulationTimeRemaining_ms=` Float containing number of milliseconds remaining in-simulation until run call is complete.
- (float) `PercentComplete=` Estimated percentage complete based on in-simulation time.


### Simulation - BuildMesh

**URI** `/NES/Simulation/BuildMesh?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to build a mesh from.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  

## VSDA

### VSDA - EM - Initialize

**URI** `/NES/VSDA/EM/Initialize?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the electron microscope renderer from.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  


### VSDA - EM - SetupMicroscope

**URI** `/NES/VSDA/EM/SetupMicroscope?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (float) `PixelResolution_nm=` Number of nanometers of resolution for each pixel.  
- (int) `ImageWidth_px=` Set the width of the image in pixels.  
- (int) `ImageHeight_px=` Set the height of the image in pixels.  
- (float) `SliceThickness_nm=` Set the thickness of each slice in nanometers.  
- (float) `ScanRegionOverlap_percent=` Set the overlap for the resulting image stacks.  
- (float) `MicroscopeFOV_deg=` Sets the FOV of the microscope in degrees. This does not have an affect on the overall scan size, as that is accounted for with automatic positioning.
- (int) `NumPixelsPerVoxel_px=` Sets the size that voxels are shown in images in pixels.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  


### VSDA - EM - DefineScanRegion

**URI** `/NES/VSDA/EM/DefineScanRegion?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (vec3) `Point1_um=` (X,Y,Z) World space location of one corner of the rectangular prism enclosing the target scan region.  
- (vec3) `Point2_um=` (X,Y,Z) World space location of the other corner of the rectangular prism enclosing the target scan region.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (bgScanRegionID) `ScanRegionID=` ID of the resulting scan region. Can be used to later get the image stack once generated.  


### VSDA - EM - QueueRenderOperation

**URI** `/NES/VSDA/EM/QueueRenderOperation?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `ScanRegionID=` ID of the scan region to be rendered.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  


### VSDA - EM - GetRenderStatus

**URI** `/NES/VSDA/EM/GetRenderStatus?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `ScanRegionID=` ID of the scan region to have it's status checked.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (bgRenderStatus) `RenderStatus=` Enum indicating status of the renderer.  
- (int) `CurrentSlice=` Int representing the current slice that the virtual microscope is on.
- (int) `TotalSlices=` Int representing the total number of slices.
- (int) `CurrentSliceImage=` Int representing the current image of this slice.
- (int) `TotalSliceImages=` Int representing the total images on this slice.


### VSDA - EM - GetImageStack

**URI** `/NES/VSDA/EM/GetImageStack?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `ScanRegionID=` ID of the scan region to get the image stack for. Note: The stack must have finished being rendered.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (list) `RenderedImages=` List of file paths that can be given to the VSDA EM GetImage Function one at a time to retrieve images.  


### VSDA - EM - GetImage

**URI** `/NES/VSDA/EM/GetImage?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (string) `ImageHandle=` String containing the image handle that needs to be grabbed from.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (base64String) `ImageData=` Base 64 encoded string containing the bytes of the file.  

### VSDA - Calcium - CreateIndicator

**URI** `/NES/VSDA/Calcium/CreateIndicator?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup calcium imaging for.  
- (str) `CalciumIndicatorName=` Name of type of indicator to be used, example: 'jGCaMP8'  
- (float) `IndicatorRiseTime_ms=` Rise time of the indicator in milliseconds.  
- (float) `IndicatorDecayTime_ms=` Decay time of the indicator in milliseconds.  
- (float) `IndicatorInterval_ms=` Interval time of the indicator in milliseconds.  
- (str list) `VisibleComponentsList=` List of what components are shown by this indicator, valid options are ['soma', 'axon', 'synapse']

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  

### VSDA - Calcium - Setup

**URI** `/NES/VSDA/Calcium/Setup?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup calcium imaging for.  
- (float) `PixelResolution_um=` Number of micrometers of resolution for each pixel.  
- (int) `ImageWidth_px=` Set the width of the image in pixels.  
- (int) `ImageHeight_px=` Set the height of the image in pixels.  
- (float) `SliceThickness_um=` Set the thickness of each slice in micrometers.  
- (float) `ScanRegionOverlap_percent=` Set the overlap for the resulting image stacks.  
- (float) `MicroscopeFOV_deg=` Sets the FOV of the microscope in degrees. This does not have an affect on the overall scan size, as that is accounted for with automatic positioning.  
- (int) `NumPixelsPerVoxel_px=` Sets the size that voxels are shown in images in pixels. (same as voxspace_side_px)  

- (str list) `FlourescingNeuronTags=` List of tags for neurons which will flouresce.  
- (int) `CalciumIndicatorID=` ID of a calcium indicator that you've defined.  
- (float) `ImagingInterval_ms=` How many milliseconds between captures.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  


### VSDA - Calcium - DefineScanRegion *NEW*

**URI** `/NES/VSDA/Calcium/DefineScanRegion?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the calcium imaging for.  
- (vec3) `Point1_um=` (X,Y,Z) World space location of one corner of the rectangular prism enclosing the target scan region.  
- (vec3) `Point2_um=` (X,Y,Z) World space location of the other corner of the rectangular prism enclosing the target scan region.  
- (bgStatus) `CalciumIndicatorID=` ID of the calcium indicator that you created.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (bgScanRegionID) `CalciumScanRegionID=` ID of the resulting scan region. Can be used to later get the image stack once generated.  


### VSDA - Calcium - QueueRenderOperation *NEW*

**URI** `/NES/VSDA/Calcium/QueueRenderOperation?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to render.  Note: this simulation must have already been simulated otherwise we cannot render it - (no data for functional recordings).
- (bgScanRegionID) `CalciumScanRegionID=` ID of the scan region to be rendered.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  


### VSDA - Calcium - GetRenderStatus *NEW*

**URI** `/NES/VSDA/Calcium/GetRenderStatus?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `CalciumScanRegionID=` ID of the scan region to have it's status checked.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (bgRenderStatus) `RenderStatus=` Enum indicating status of the renderer.  
- (int) `CurrentSlice=` Int representing the current slice that the virtual microscope is on.
- (int) `TotalSlices=` Int representing the total number of slices.
- (int) `CurrentSliceImage=` Int representing the current image of this slice.
- (int) `TotalSliceImages=` Int representing the total images on this slice.


### VSDA - Calcium - GetImageStack *NEW*

**URI** `/NES/VSDA/Calcium/GetImageStack?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `CalciumScanRegionID=` ID of the scan region to get the image stack for. Note: The stack must have finished being rendered.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (list) `RenderedImages=` List of file paths that can be given to the VSDA EM GetImage Function one at a time to retrieve images.  


### VSDA - Calcium - GetImage *NEW*

**URI** `/NES/VSDA/Calcium/GetImage?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (string) `ImageHandle=` String containing the image handle that needs to be grabbed from.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (base64String) `ImageData=` Base 64 encoded string containing the bytes of the file.  

## RecordingElectrode **NEW**

### RecordingElectrode - Initialize     **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/Initialize?` 
**Request**: 
*Required Params*:

- (bgSimulationID) `SimulationID=` ID of simulation to setup the electrode for. 

*Optional Params*:

- (vec3) `TipPosition_um=` Coordinates of the tip of the electrode in micrometers.
- (vec3) `EndPosition_um=` Coordinates of the tip of the electrode in micrometers.
- (vec3) `TipPosition_um=` Coordinates of the tip of the electrode in micrometers.
- (list[vec3]) `Sites=` Coordinates of all sites within the region from where data is being acquired.
- (float) `NoiseLevel=` Intensity of the synthetic noise.

**Response**:

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (int) `RecordingElectrodeID=` ID of the new RecordingElectrode.

### RecordingElectrode - AddNoise   **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/AddNoise?`
**Request**:
*Required Params*:

- (int) `SimulationID=` ID of the simulation.
- (int) `RecordingElectrodeID=` ID of the recording electrode.

**Response**:

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.

## RecordingElectrode - GetRecording    *NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/GetRecording?`
**Request**:
*Required Params*:

- (int) `SimulationID=` ID of the simulation.
- (int) `RecordingElectrodeID=` ID of the recording electrode.

**Response**:

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (dict) `RecordingDict=` Dict/Hashmap containing the data collected by the recording electrode.

## RecordingElectrode - SetElectricFieldPotential   **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/SetElectricFieldPotential?`
**Request**:
*Required Params*:

- (int) `SimulationID=` ID of the simulation.
- (int) `RecordingElectrodeID=` ID of the recording electrode.
- (int) `SiteId=` Integer index of the recording electrode site.

**Response**:

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (dict) `RecordingDict=` Dict/Hashmap containing the data collected by the recording electrode.



## Visualizer

## Visualizer - RenderImage

**URI** `/NES/Visualizer/RenderImage?`
**Request**:
*Required Params*:

- (int) `SimulationID=` ID of the simulation.  
- (vec3) `CameraPosition_um=` (X,Y,Z) location of the camera's position in microns from world origin.    
- (vec3) `CameraLookAtPosition_um=` (X,Y,Z) Location where the camera is looking at in microns from world origin.  
- (float) `CameraFOV_deg=` FOV of the camera in degrees.  

**Response**:

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (base64String) `ImageData=` Base 64 encoded string containing the bytes of the image.  