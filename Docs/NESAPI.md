# NES Internal API Version 2024.01.14

# About
This API spec dictates how the API gateway internally communicates with the NES subsystem. **This document does NOT describe the API that is front-facing and interacted with by users. This ONLY describes the internal API used to connect between the gateway and upstream service**  


Since the API gateway will be taking care of many aspects about handling security (such as authentication, input validation, etc.) we can simplify this communication spec much more than what is defined for the public facing API. So, with that said, we use RPC to implement this API.


Unless otherwise specified, all requests and responses will be in json.

`The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in this document are to be interpreted as described in RFC 2119.`



# Document Format
Each route will be described in this format:

## Name Of Route
**Route**: `RouteStringHere`  
**Request**:  
*Required Params*: <- These parameters *must* be included in the request (for this version)
 - (type) `Key Of Param` Description here.
*Optional Params*: <- These parameters *may* be included in the request (for this version) 
 - (type) `Key Of Param` Description here.


**Response**:  
 - (type) `Key Of Param` Description here.

**Notes**  
 - some notes about the route here, these *may* be included.  


# Enums

## StatusCode  
 - `0` Success
 - `1` Invalid Simulation ID
 - `2` Invalid Other ID (could be shape/dac/adc/staple/etc...)
 - `5` Simulation Busy
 - `999` General Failure

## bgRenderStatus
 - `0` Not Initialized
 - `1` Initialization Begin
 - `2` Initialization Done
 - `3` Render Requsted
 - `4` Render Ready
 - `5` Render Done




# Routes


## Diagnostic
 
### GetAPIVersion
**Route**: `GetAPIVersion`  
**Request**:  
*Required Params*:  

**Response**:  
 - (string) `Version` Version of the API that the NES service is running.

**Notes**  
 - The two versions *must* match, if they do not, the connection will be terminated with an error indicating a version mismatch. This route is usually called during intiailisation to ensure the client is talking to the server and they understand what's going on.


### Echo
**Route**: `Echo`  
**Request**:  
*Required Params*: 
 - (string) `Data` String to be echoed back in the response. Note that the server *may* truncate this at 512 bytes long.

**Response**:  
 - (string) `Data` Same string as the input, returned.

**Notes**  
 - This route is used for debugging and ensuring that the server is still happy and alive.



## Simulation  

### Simulation - Create
**Route**: `Simulation/Create`  
**Request**:  
*Required Params*: 
 - (std::string) `Name` Name of your simulation.

**Response**:  
 - (int) `SimulationID` ID is greater than 0 on success, otherwise -1 on fail.
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.

**Notes**  
 - None.


### Simulation - Reset
**Route**: `Simulation/Reset`  
**Request**:  
*Required Params*: 
- (int) `SimulationID` ID of the simulation being reset. 

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.

**Notes**  
 - None.


### Simulation - Run For
**Route**: `Simulation/RunFor`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation being run.  
- (float) `Runtime_ms` Number of milliseconds to simulate.

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.

**Notes**  
 - None.


### Simulation - RecordAll
**Route**: `Simulation/RecordAll`  
**Request**:  
*Required Params*:   
- (int) `SimulationID` ID of the simulation being run.  
- (float) `MaxRecordTime_ms` Maximum number of milliseconds to record. -1 to record forever. 0 to disable/abort.  

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.

**Notes**  
 - None.


### Simulation - GetRecording
**Route**: `Simulation/GetRecording`  
**Request**:  
*Required Params*: 
- (int) `SimulationID` ID of the simulation being run. 

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  
- (string) `Recording` JSON blob containing recorded data of all objects.  

**Notes**  
 - None.

### Simulation - GetStatus
**Route**: `Simulation/GetStatus`  
**Request**:  
*Required Params*: 
- (int) `SimulationID` ID of the simulation being run. 

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  
- (bool) `IsSimulating` Is simulating true/false.  
- (float) `RealWorldTimeRemaining_ms` Float containing number of milliseconds estimated to be remaining in real-world time until current run call completes.  
- (float) `RealWorldTimeElapsed_ms` Float containing number of milliseconds elapsed since simulation started in real-world time measured since start of last run call.  
- (float) `InSimulationTime_ms` Float containing number of in-simulation milliseconds elapsed since last reset call.  
- (float) `InSimulationTimeRemaining_ms` Float containing number of milliseconds remaining in-simulation until run call is complete.    
- (float) `PercentComplete` Estimated percentage complete based on in-simulation time.  

**Notes**  
 - None.


### Simulation - BuildMesh  

**URI** `Simulation/BuildMesh`  
**Request**:  
*Required Params*:  

- (int) `SimulationID` ID of simulation to build a mesh from  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  




## Shapes


### Sphere - Create
**Route**: `Geometry/Shape/Sphere/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this sphere is being created.  
- (float) `Radius_um` The radius of the sphere in micrometers.  
- (float) `CenterPosX_um` Position of the center's X coord of the sphere in micrometers.  
- (float) `CenterPosY_um` Position of the center's Y coord of the sphere in micrometers.  
- (float) `CenterPosZ_um` Position of the center's Z coord of the sphere in micrometers.  
- (string) `Name` Name of the sphere, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `ShapeID` ID of the resulting shape created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### Sphere - BulkCreate

**URI**: `/NES/Geometry/Shape/Sphere/BulkCreate`  
**Request**:  
*Required Params*:  

- (float list) `RadiusList_um` List of radiuses of the spheres.  
- (float list) `CenterXList_um` List of center X coordinates.  
- (float list) `CenterYList_um` List of center Y coordinates.  
- (float list) `CenterZList_um` List of center Z coordinates.  
- (string list) `NameList` List of names of the spheres.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (bgShapeID list) `ShapeIDs` List of returned shape IDs, will be empty on failure.


### Cylinder - Create
**Route**: `Geometry/Shape/Cylinder/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this cylinder is being created.  
- (float) `Point1Radius_um` The radius of the first point in micrometers.  
- (float) `Point1PosX_um` Position of the first point center's X coord of the sphere in micrometers.  
- (float) `Point1PosY_um` Position of the first point center's Y coord of the sphere in micrometers.  
- (float) `Point1PosZ_um` Position of the first point center's Z coord of the sphere in micrometers.  
- (float) `Point2Radius_um` The radius of the second point in micrometers.  
- (float) `Point2PosX_um` Position of the second point center's X coord of the sphere in micrometers.  
- (float) `Point2PosY_um` Position of the second point center's Y coord of the sphere in micrometers.  
- (float) `Point2PosZ_um` Position of the second point center's Z coord of the sphere in micrometers.  
- (string) `Name` Name of the sphere, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `ShapeID` ID of the resulting shape created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### Cylinder - BulkCreate

**URI**: `/NES/Geometry/Shape/Cylinder/BulkCreate`  
**Request**:  
*Required Params*:  

- (float list) `Point1RadiusList_um` List of radiuses at the first point.  
- (float list) `Point2RadiusList_um` List of radiuses at the first point.  
- (float list) `Point1XList_um` List of point 1 X coordinates.  
- (float list) `Point1YList_um` List of point 1 Y coordinates.  
- (float list) `Point1ZList_um` List of point 1 Z coordinates.  
- (float list) `Point2XList_um` List of point 2 X coordinates.  
- (float list) `Point2YList_um` List of point 2 Y coordinates.  
- (float list) `Point2ZList_um` List of point 2 Z coordinates.  
- (string list) `NameList` List of names of the cylinders.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (bgShapeID list) `ShapeIDs` List of returned shape IDs, will be empty on failure.


### Box - Create
**Route**: `Geometry/Shape/Box/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this box is being created.    
- (float) `CenterPosX_um` Position of the center x coord.  
- (float) `CenterPosY_um` Position of the center y coord.  
- (float) `CenterPosZ_um` Position of the center z coord.  
- (float) `ScaleX_um` Scale of the x axis of the box.  
- (float) `ScaleY_um` Scale of the y axis of the box.  
- (float) `ScaleZ_um` Scale of the z axis of the box.  
- (float) `RotationX_rad` Euler angle of the X axis in radians.  
- (float) `RotationY_rad` Euler angle of the Y axis in radians.  
- (float) `RotationZ_rad` Euler angle of the Z axis in radians.  
- (string) `Name` Name of the Box, defaults to 'undefined'.  

**Response**:  
- (bgShapeID) `ShapeID` ID of the resulting shape created here (-1 on fail).    
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  

## Compartments


### Box - BulkCreate

**URI**: `/NES/Geometry/Shape/Box/BulkCreate`  
**Request**:  
*Required Params*:  

- (float list) `CenterXList_um` List of center X coordinates.  
- (float list) `CenterYList_um` List of center Y coordinates.  
- (float list) `CenterZList_um` List of center Z coordinates.  
- (float list) `DimensionsXList_um` List of X dimensions for the boxes.  
- (float list) `DimensionsYList_um` List of Y dimensions for the boxes.  
- (float list) `DimensionsZList_um` List of Z dimensions for the boxes.  
- (float list) `RotationX_rad` List of X rotations in radians.  
- (float list) `RotationY_rad` List of Y rotations in radians.  
- (float list) `RotationZ_rad` List of Z rotations in radians.  
- (string list) `NameList` List of names of the boxes.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (bgShapeID list) `ShapeIDs` List of returned shape IDs, will be empty on failure.


### BS - Create
**Route**: `Compartment/BS/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.   
- (bgShapeID) `ShapeID` ID of the shape which sets the size of this shape.  
- (float) `MembranePotential_mV` Membrane potential of the compartment in millivolts.  
- (float) `SpikeThreshold_mV` Action potential spike threshold of the compartment in millivolts.  
- (float) `DecayTime_ms` (tau_ahp) After hyperpolarization time constant in milliseconds.  
- (float) `RestingPotential_mV` To be documented.  
- (float) `AfterHyperpolarizationAmplitude_mV` to be documented.  

- (string) `Name` Name of the BallStick compartment, defaults to 'undefined'.  

**Response**:   
- (bgCompartmentID) `CompartmentID` ID of the resulting compartment created here (-1 on fail).   
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  


### BS - BulkCreate

**URI**: `/NES/Compartment/BS/BulkCreate`  
**Request**:  
*Required Params*:  

- (bgShapeID list) `ShapeIDList` List of ID of an NES Shapes.   
- (float list) `MembranePotentialList_mV` List of Membrane potential of the compartment in millivolts.  
- (float list) `SpikeThresholdList_mV` List of Action potential spike threshold of the compartment in millivolts.  
- (float list) `DecayTimeList_ms` List of (tau_ahp) After hyperpolarization time constant in milliseconds.  
- (float list) `RestingPotentialList_mV` To be documented.  
- (float list) `AfterHyperpolarizationAmplitudeList_mV` to be documented.  
- (string list) `NameList` List of names of the boxes.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (bgCompartmentID list) `CompartmentIDs` List of returned compartment IDs, will be empty on failure.



### Neuron - Create

**URI** `/NES/Neuron/BSNeuron/Create` 
**Request**:  
*Required Params*:  

- (bgShapeID) `SomaID` ID of the shape object for the soma of the neuron (e.g. a sphere).
- (bgShapeID) `AxonID` ID of the shape object for the axon of the neuron (e.g. a cylinder).
- (float) `MembranePotential_mV` Membrane potential set at time of construction.
- (float) `RestingPotential_mV` Membrane potential at rest.
- (float) `SpikeThreshold_mV` Membrane potential at which the neuron will fire an action potential.
- (float) `DecayTime_ms` Time constant for the decay of the after-hyperpolarization of the neuron.
- (float) `AfterHyperpolarizationAmplitude_mV` Amplitude of the after-hyperpolarization of the neuron.
- (float) `PostsynapticPotentialRiseTime_ms` Time constant of the rise-time of a post-synaptic potential arriving at the neuron.
- (float) `PostsynapticPotentialDecayTime_ms` Time constant of the decay-time of a post-synaptic potential arriving at the neuron.
- (float) `PostsynapticPotentialAmplitude_mV` Amplitude of a post-synaptic potential arriving at the neuron.

*Optional Params*:  

- (string) `Name` Optional name of the neuron.
- (str) `NeuronTag` String which can be used to group this neuron along with others. ** NEW **  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (bgNeuronID) `NeuronID` ID of the resulting neuron (if status indicates success, otherwise this is -1).


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

**URI** `/NES/NeuralCircuit/BSAlignedNC/Create`    
**Request**:  

*Required Params*: 

- (int) `GeometryCollectionID` Number of cells in the neural circuit.

*Optional Params*:  

- (int) `NumCells` Number of cells in the neural circuit.

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.


## Connections


### Staple - Create
**Route**: `Connection/Staple/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.  
- (bgCompartmentID) `SourceCompartmentID` ID of the compartment whos data will be copied to the destination.  
- (bgCompartmentID) `DestinationCompartmentID` ID of the compartment whos data will be overwritten with the source.  
- (string) `Name` Name of the Staple, defaults to 'undefined'.  

**Response**:  
- (bgConnectionID) `StapleID` ID of the resulting staple connection created here (-1 on fail).   
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  


### Receptor - Create
**Route**: `Connection/Receptor/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.  
- (bgCompartmentID) `SourceCompartmentID` ID of the compartment whos data will be copied to the destination.  
- (bgCompartmentID) `DestinationCompartmentID` ID of the compartment whos data will be overwritten with the source.  
- (float) `Conductance_nS` Conductance from source to destination in nanoSiemens.  
- (float) `TimeConstantRise_ms` Postsynaptic potential rise time constant in milliseconds.
- (float) `TimeConstantDecay_ms` Postsynaptic potential decay time constant in milliseconds.
- (float) `ReceptorPosX_um` X world space coordinate in micrometers.  
- (float) `ReceptorPosY_um` Y world space coordinate in micrometers.   
- (float) `ReceptorPosZ_um` Z world space coordinate in micrometers.  
- (string) `Name` Name of the Receptor, defaults to 'undefined'.  

**Response**:  
- (bgConnectionID) `ReceptorID` ID of the resulting receptor connection created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


## Neurons

### BSNeuron - Create **NEW**

**Route** `Neuron/BSNeuron/Create`  
**Request**:  
*Required Params*:  

- (int) `SimulationID` ID of the simulation where this is being created.
- (bgShapeID) `SomaID` ID of the shape object for the soma of the neuron (e.g. a sphere).
- (bgShapeID) `AxonID` ID of the shape object for the axon of the neuron (e.g. a cylinder).
- (float) `MembranePotential_mV` Membrane potential set at time of construction.
- (float) `RestingPotential_mV` Membrane potential at rest.
- (float) `SpikeThreshold_mV` Membrane potential at which the neuron will fire an action potential.
- (float) `DecayTime_ms` Time constant for the decay of the after-hyperpolarization of the neuron.
- (float) `AfterHyperpolarizationAmplitude_mV` Amplitude of the after-hyperpolarization of the neuron.
- (float) `PostsynapticPotentialRiseTime_ms` Time constant of the rise-time of a post-synaptic potential arriving at the neuron.
- (float) `PostsynapticPotentialDecayTime_ms` Time constant of the decay-time of a post-synaptic potential arriving at the neuron.
- (float) `PostsynapticPotentialAmplitude_mV` Amplitude of a post-synaptic potential arriving at the neuron.
- (string) `Name` Name of the neuron.

**Response**:  

- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.
- (bgNeuronID) `NeuronID` ID of the resulting neuron (if status indicates success, otherwise this is -1).


## Tools

### DAC - Create
**Route**: `Tool/PatchClampDAC/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.   
- (bgCompartmentID) `DestinationCompartmentID` ID of the compartment receiving DAC output.  
- (float) `ClampPosX_um` X world space coordinate in micrometers.  
- (float) `ClampPosY_um` Y world space coordinate in micrometers.  
- (float) `ClampPosZ_um` Z world space coordinate in micrometers.  
- (string) `Name` Name of the DAC, defaults to 'undefined'.  

**Response**:  
- (bgPatchClampDACID) `PatchClampDACID` ID of the resulting DAC created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  


### DAC - Set Output List
**Route**: `Tool/PatchClampDAC/SetOutputList`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being modified.   
- (bgPatchClampDACID) `PatchClampDACID` ID of the target dac to be modified.  
- (float list) `DACVoltages_mV` JSON formatted list of voltages to be played back by the DAC.  
- (float) `Timestep_ms` Simulation time for which each step in the DACVoltages is replayed.   

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  


### ADC - Create
**Route**: `Tool/PatchClampADC/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.   
- (bgCompartmentID) `SourceCompartmentID` ID of the compartment being read by the ADC output.  
- (float) `ClampPosX_um` X world space coordinate in micrometers.  
- (float) `ClampPosY_um` Y world space coordinate in micrometers.    
- (float) `ClampPosZ_um` Z world space coordinate in micrometers.  
- (string) `Name` Name of the DAC, defaults to 'undefined'.  
    
**Response**:  
- (bgShapeID) `PatchClampADCID` ID of the resulting ADC created here (-1 on fail).    
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  


### ADC - Set Sample Rate
**Route**: `Tool/PatchClampADC/SetSampleRate`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being modified.   
- (bgPatchClampADCID) `PatchClampADCID` ID of the ADC being configured.  
- (float) `Timestep_ms` Sets the sample timestep rate for the ADC in milliseconds.   

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  


### ADC - Get Recorded Data
**Route**: `Tool/PatchClampADC/GetRecordedData`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being modified.   
- (bgPatchClampADCID) `PatchClampADCID` ID of the ADC being configured.  

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.  
- (string) `RecordedData_mV` JSON formatted list of voltages recorded by the ADC.  
- (float) `Timestep_ms` Gets the sample timestep rate for the ADC in milliseconds.   




## VSDA

### VSDA - EM - Initialize

**URI** `VSDA/EM/Initialize`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the electron microscope renderer from.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  


### VSDA - EM - SetupMicroscope

**URI** `VSDA/EM/SetupMicroscope`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (float) `PixelResolution_nm` Number of nanometers of resolution for each pixel.  
- (int) `ImageWidth_px` Set the width of the image in pixels.  
- (int) `ImageHeight_px` Set the height of the image in pixels.  
- (float) `SliceThickness_nm` Set the thickness of each slice in nanometers.  
- (float) `ScanRegionOverlap_percent` Set the overlap for the resulting image stacks.  
- (float) `MicroscopeFOV_deg` Sets the FOV of the microscope in degrees. This does not have an affect on the overall scan size, as that is accounted for with automatic positioning.
- (int) `NumPixelsPerVoxel_px` Sets the size that voxels are shown in images in pixels.

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  


### VSDA - EM - DefineScanRegion

**URI** `VSDA/EM/DefineScanRegion`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (vec3) `Point1_um` (X,Y,Z) World space location of one corner of the rectangular prism enclosing the target scan region.  
- (vec3) `Point2_um` (X,Y,Z) World space location of the other corner of the rectangular prism enclosing the target scan region.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (bgScanRegionID) `ScanRegionID` ID of the resulting scan region. Can be used to later get the image stack once generated.  


### VSDA - EM - QueueRenderOperation

**URI** `VSDA/EM/QueueRenderOperation`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `ScanRegionID` ID of the scan region to be rendered.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  


### VSDA - EM - GetRenderStatus

**URI** `VSDA/EM/GetRenderStatus`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (bgRenderStatus) `RenderStatus` Enum indicating status of the renderer.  
- (int) `CurrentSlice` Int representing the current slice that the virtual microscope is on.
- (int) `TotalSlices` Int representing the total number of slices.
- (int) `CurrentSliceImage` Int representing the current image of this slice.
- (int) `TotalSliceImages` Int representing the total images on this slice.

### VSDA - EM - GetImageStack

**URI** `VSDA/EM/GetImageStack`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `ScanRegionID` ID of the scan region to get the image stack for. Note: The stack must have finished being rendered.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (list) `RenderedImages` List of file paths that can be given to the VSDA EM GetImage Function one at a time to retrieve images.  


### VSDA - EM - GetImage

**URI** `/NES/VSDA/EM/GetImage`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (string) `ImageHandle` String containing the image handle that needs to be grabbed from.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (base64String) `ImageData` Base 64 encoded string containing the bytes of the file.  


### VSDA - Calcium - DefineScanRegion *NEW*

**URI** `/NES/VSDA/Calcium/DefineScanRegion`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the calcium imaging for.  
- (vec3) `Point1_um` (X,Y,Z) World space location of one corner of the rectangular prism enclosing the target scan region.  
- (vec3) `Point2_um` (X,Y,Z) World space location of the other corner of the rectangular prism enclosing the target scan region.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (bgScanRegionID) `CalciumScanRegionID` ID of the resulting scan region. Can be used to later get the image stack once generated.  


### VSDA - Calcium - QueueRenderOperation *NEW*

**URI** `/NES/VSDA/Calcium/QueueRenderOperation`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to render.  Note: this simulation must have already been simulated otherwise we cannot render it - (no data for functional recordings).
- (bgScanRegionID) `CalciumScanRegionID` ID of the scan region to be rendered.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  


### VSDA - Calcium - GetRenderStatus *NEW*

**URI** `/NES/VSDA/Calcium/GetRenderStatus`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `CalciumScanRegionID` ID of the scan region to have it's status checked.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (bgRenderStatus) `RenderStatus` Enum indicating status of the renderer.  
- (int) `CurrentSlice` Int representing the current slice that the virtual microscope is on.
- (int) `TotalSlices` Int representing the total number of slices.
- (int) `CurrentSliceImage` Int representing the current image of this slice.
- (int) `TotalSliceImages` Int representing the total images on this slice.


### VSDA - Calcium - GetImageStack *NEW*

**URI** `/NES/VSDA/Calcium/GetImageStack`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `CalciumScanRegionID` ID of the scan region to get the image stack for. Note: The stack must have finished being rendered.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (list) `RenderedImages` List of file paths that can be given to the VSDA EM GetImage Function one at a time to retrieve images.  


### VSDA - Calcium - GetImage *NEW*

**URI** `/NES/VSDA/Calcium/GetImage`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for.  
- (string) `ImageHandle` String containing the image handle that needs to be grabbed from.  

**Response**:  

- (bgStatus) `StatusCode` Enum indicating the status of this API call.  
- (base64String) `ImageData` Base 64 encoded string containing the bytes of the file.  

## RecordingElectrode

### RecordingElectrode - Initialize **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/Initialize` 
**Request**: 
*Required Params*:

- (bgSimulationID) `SimulationID` ID of simulation to setup the electrode for. 

*Optional Params*:

- (vec3) `TipPosition_um` Coordinates of the tip of the electrode in micrometers.
- (vec3) `EndPosition_um` Coordinates of the tip of the electrode in micrometers.
- (vec3) `TipPosition_um` Coordinates of the tip of the electrode in micrometers.
- (list[vec3]) `Sites` Coordinates of all sites within the region from where data is being acquired.
- (float) `NoiseLevel` Intensity of the synthetic noise.

**Response**:

- (bgStatus) `StatusCode` Enum indicating the status of this API call.

### RecordingElectrode - AddNoise   **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/AddNoise`
**Request**:
*Required Params*:

- None

**Response**:

- (bgStatus) `StatusCode` Enum indicating the status of this API call.

## RecordingElectrode - GetRecording **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/GetRecording?`
**Request**:
*Required Params*:

- None

**Response**:

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (dict) `RecordingDict` Dict/Hashmap containing the data collected by the recording electrode.

## RecordingElectrode - SetElectricFieldPotential **NEW**

**URI** `/NES/Simulator/Structs/RecordingElectrode/SetElectricFieldPotential?`
**Request**:
*Required Params*:

- (int) `SiteIdx` Integer index of the recording electrode site.

**Response**:

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (dict) `RecordingDict` Dict/Hashmap containing the data collected by the recording electrode.



## Visualizer

## Visualizer - RenderImage

**URI** `Visualizer/RenderImage`
**Request**:
*Required Params*:

- (int) `SimulationID` ID of the simulation.  
- (vec3) `CameraPosition_um` (X,Y,Z) location of the camera's position in microns from world origin.    
- (vec3) `CameraLookAtPosition_um` (X,Y,Z) Location where the camera is looking at in microns from world origin.  
- (float) `CameraFOV_deg` FOV of the camera in degrees.  

**Response**:

- (bgStatus) `StatusCode` Enum indicating the status of this API call.
- (base64String) `ImageData` Base 64 encoded string containing the bytes of the image.  