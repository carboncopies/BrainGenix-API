# API Design Spec

Note: Every API call below, except the Simulation/Create call, must also receive
the parameters:

- (bgSimulationID) `SimulationID=` Simulation ID number.
- (AuthKey) `AuthKey=` Authorization key.


# Enums

## bgStatus  
 - `0` Success
 - `400` Invalid Parameters Passed
 - `500` Downstream Gateway Unavailable




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
 - (string) `Version=` Returns string containing the version of this server.  


### Status

**URI**: `/Diagnostic/Status`  
**Request**:  
*Required Params*:  

**Response**:  
 - (string) `SystemState=` String indicating the current state of the service. Can be "Healthy", "Degraded", "Failed".  
 - (bgServiceStatus) `ServiceStateNES=` Enum indicating the state of the NES Service. Shows more detail than the above quick reference data.  



## Shapes

### Sphere - Create

**URI**: `/NES/Geometry/Shape/Sphere/Create?`  
**Request**:  
*Required Params*:  

- (float) `Radius_nm=` The radius of the sphere in nanometers.
- (vec3) `Center_nm=` Position of the center of the sphere in nanometers. Given as a list containing the three x,y,z coordinates.

*Optional Params*:  

- (string) `Name=` Optional name of the sphere.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).


### Cylinder - Create

**URI**: `/NES/Geometry/Shape/Cylinder/Create?`  
**Request**:  
*Required Params*:  

- (float) `Point1Radius_nm=` The radius at the cylinder's first point.
- (vec3) `Point1Position_nm=` First endpoint of the cylinder's axis.
- (float) `Point2Radius_nm=` The radius at the cylinder's second point.
- (vec3) `Point2Position_nm=` Second endpoint of the cylinder's axis.

*Optional Params*:  

- (string) `Name=` Optional name of the cylinder.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).


### Box - Create

**URI**: `/NES/Geometry/Shape/Box/Create?`  
**Request**:  
*Required Params*:  

- (vec3) `CenterPosition_nm=` Position of the center of the box in world space coordinates.
- (vec3) `Dimensions_nm=` (X,Y,Z) dimensions of the box in nm.
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
- (bgConnectionID) `ConnectionID=` ID of the resulting connection created here (if status indicates success, otherwise this is -1).


### Receptor - Create

**URI** `/NES/Connection/Receptor/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `SourceCompartmentID=` ID of the compartment with presynaptic spike.
- (bgCompartmentID) `DestinationCompartmentID=` ID of the compartment receiving postsynaptic activity.
- (float) `Conductance_nS=` Conductance from source to destination in nanoSiemens.
- (float) `TimeConstant_ms=` Postsynaptic potential time constant in milliseconds.
- (vec3) `ReceptorLocation_nm=` (X,Y,Z) World space location of the receptor's base (where it intersects the compartment).

*Optional Params*:  

- (string) `Name=` Optional name of the connection.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (bgConnectionID) `ConnectionID=` ID of the resulting connection created here (if status indicates success, otherwise this is -1).



## Tools

### DAC - Create

**URI** `/NES/Tool/PatchClampDAC/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `DestinationCompartmentID=` ID of the compartment receiving DAC output.
- (vec3) `ClampLocation_nm=` (X,Y,Z) World space location of the DAC's connection in nanometers.

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
- (float list) `DACVoltages_mV=` List of voltages to be played back by the DAC.
- (float) `Timestep_ms=` Simulation time for which each step in the DACVoltages is replayed. 

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.


### ADC - Create

**URI** `/NES/Tool/PatchClampADC/Create?`  
**Request**:  
*Required Params*:  

- (bgCompartmentID) `SourceCompartmentID=` ID of the compartment being read by the ADC output.
- (vec3) `ClampLocation_nm=` (X,Y,Z) World space location of the ADC's connection in nanometers.

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
- (float) `Timestep_ms=` Sets the sample timestep rate for the ADC in milliseconds. 

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.


### ADC - Get Recorded Data

**URI** `/NES/Tool/PatchClampADC/GetRecordedData?`  
**Request**:  
*Required Params*:  

- (bgPatchClampADCID) `TargetADC=` ID of the ADC being read.

**Response**:  

- (bgStatus) `StatusCode=` Enum indicating the status of this API call.
- (float list) `RecordedData_mV=` List of voltages recorded by the ADC.
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
