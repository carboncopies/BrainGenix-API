# API Design Spec

Note: Every API call below, except the Simulation/Create call, must also receive
the parameters:

- (bgSimulationID) `SimulationID=` Simulation ID number.
- (AuthKey) `AuthKey=` Authorization key.

Note: On a nonsuccess status code, other parameters are *not* guarenteed to be provided. Always check the status code first.


# Enums

## bgStatus  
 - `0` Success
 - `1` General Failure
 - `2` Invalid Parameters Passed
 - `3` Upstream Gateway Unavailable
 - `4` Unauthorized/invaid token/no token provided

## bgServiceStatus
 - `0` Healthy
 - `1` Degraded
 - `2` Failed
 - `3` Not Configured


# Implementation Status
 - [x] Diagnostic Hello
 - [x] Diagnostic Version
 - [x] Diagnostic Status

 - [x] Authentication GetToken
 
 - [x] NES Simulation Create
 - [x] NES Simulation Reset
 - [x] NES Simulation RunFor
 - [x] NES Simulation RecordAll
 - [x] NES Simulation GetRecording
 - [x] NES Simulation GetStatus
 - [x] NES Shapes SphereCreate
 - [x] NES Shapes CylinderCreate
 - [x] NES Shapes BoxCreate
 - [x] NES Compartments BSCreate
 - [x] NES Connections StapleCreate
 - [x] NES Connections ReceptorCreate
 - [x] NES Tools DACCreate
 - [x] NES Tools DACSetOutputList
 - [x] NES Tools ADCCreate
 - [x] NES Tools ADCSetSampleRate
 - [x] NES Tools ADCGetRecordedData


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



## Compartments

### BS - Create

**URI**: `/NES/Compartment/BS/Create?`  
**Request**:  
*Required Params*:  
- (bgShapeID) `ShapeID=` ID of an NES Shape.   
- (float) `MembranePotential_mV=` Membrane potential of the compartment in millivolts.  
- (float) `SpikeThreshold_mV=` Action potential spike threshold of the compartment in millivolts.  
- (float) `DecayTime_ms=` (tau_ahp) After hyperpolarization time constant in milliseconds.  
- (float) `RestingPotential_mV=` To be documented.     - needs implementation!  
- (float) `AfterHyperpolarizationAmplitude_mV=` to be documented.       - needs implementation!  

*Optional Params*:  
- (string) `Name=` Optional name of the compartment.  

**Response**:   

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgCompartmentID) `CompartmentID=` ID of the resulting compartment created here (if status indicates success, otherwise this is -1).



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
- (float) `TimeConstant_ms=` Postsynaptic potential time constant in milliseconds.
- (vec3) `ReceptorLocation_um=` (X,Y,Z) World space location of the receptor's base (where it intersects the compartment).

*Optional Params*:  

- (string) `Name=` Optional name of the connection.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgConnectionID) `RecptorID=` ID of the resulting receptor connection created here (if status indicates success, otherwise this is -1).



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


### VSDA - EM - GetImageStack

**URI** `/NES/VSDA/EM/GetImageStack?`  
**Request**:  
*Required Params*:  

- (bgSimulationID) `SimulationID=` ID of simulation to setup the microscope for.  
- (bgScanRegionID) `ScanRegionID=` ID of the scan region to get the image stack for. Note: The stack must have finished being rendered.  

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.  
- (Base64String) `ImageData=` String containing base64 encoded image data.    