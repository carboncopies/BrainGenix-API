# NES Internal API Version 2023.06.25

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



## Shapes


### Sphere - Create
**Route**: `Geometry/Shape/Sphere/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this sphere is being created.  
- (float) `Radius_nm` The radius of the sphere in nanometers.  
- (float) `CenterPosX_nm` Position of the center's X coord of the sphere in nanometers.  
- (float) `CenterPosY_nm` Position of the center's Y coord of the sphere in nanometers.  
- (float) `CenterPosZ_nm` Position of the center's Z coord of the sphere in nanometers.  
- (string) `Name` Name of the sphere, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### Cylinder - Create
**Route**: `Geometry/Shape/Cylinder/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this cylinder is being created.  
- (float) `Point1Radius_nm` The radius of the first point in nanometers.  
- (float) `Point1PosX_nm` Position of the first point center's X coord of the sphere in nanometers.  
- (float) `Point1PosY_nm` Position of the first point center's Y coord of the sphere in nanometers.  
- (float) `Point1PosZ_nm` Position of the first point center's Z coord of the sphere in nanometers.  
- (float) `Point2Radius_nm` The radius of the second point in nanometers.  
- (float) `Point2PosX_nm` Position of the second point center's X coord of the sphere in nanometers.  
- (float) `Point2PosY_nm` Position of the second point center's Y coord of the sphere in nanometers.  
- (float) `Point2PosZ_nm` Position of the second point center's Z coord of the sphere in nanometers.  
- (string) `Name` Name of the sphere, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `ShapeID` ID of the resulting shape created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### Box - Create
**Route**: `Geometry/Shape/Box/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this box is being created.  
- (float) `CenterPosX_nm` Position of the center x coord.
- (float) `CenterPosY_nm` Position of the center y coord.
- (float) `CenterPosZ_nm` Position of the center z coord.
- (float) `ScaleX_nm` Scale of the x axis of the box.
- (float) `ScaleY_nm` Scale of the y axis of the box.
- (float) `ScaleZ_nm` Scale of the z axis of the box.
- (float) `RotationX_rad` Euler angle of the X axis in radians.
- (float) `RotationY_rad` Euler angle of the Y axis in radians.
- (float) `RotationZ_rad` Euler angle of the Z axis in radians.
- (string) `Name` Name of the Box, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `ShapeID` ID of the resulting shape created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.



## Compartments


### BS - Create
**Route**: `Compartment/BS/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.  
- (float) `ShapeID` ID of the shape which sets the size of this shape.
- (float) `MembranePotential_mV` Membrane potential of the compartment in millivolts.
- (float) `SpikeThreshold_mV` Action potential spike threshold of the compartment in millivolts.
- (float) `DecayTime_ms` (tau_ahp) After hyperpolarization time constant in milliseconds.
- (string) `Name` Name of the BallStick compartment, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `CompartmentID` ID of the resulting compartment created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.



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
- (bgShapeID) `ConnectionID` ID of the resulting connection created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### Receptor - Create
**Route**: `Connection/Receptor/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.  
- (bgCompartmentID) `SourceCompartmentID` ID of the compartment whos data will be copied to the destination.
- (bgCompartmentID) `DestinationCompartmentID` ID of the compartment whos data will be overwritten with the source.
- (float) `Conductance_nS` Conductance from source to destination in nanoSiemens.
- (float) `TimeConstant_ms` Postsynaptic potential time constant in milliseconds.
- (float) `ReceptorPosX_nm` X world space coordinate in nanometers.
- (float) `ReceptorPosY_nm` Y world space coordinate in nanometers.
- (float) `ReceptorPosZ_nm` Z world space coordinate in nanometers.
- (string) `Name` Name of the Receptor, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `ConnectionID` ID of the resulting connection created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.



## Tools

### DAC - Create
**Route**: `Tool/PatchClampDAC/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being created.  
- (bgCompartmentID) `DestinationCompartmentID` ID of the compartment receiving DAC output.
- (float) `ClampPosX_nm` X world space coordinate in nanometers.
- (float) `ClampPosY_nm` Y world space coordinate in nanometers.
- (float) `ClampPosZ_nm` Z world space coordinate in nanometers.
- (string) `Name` Name of the DAC, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `PatchClampDACID` ID of the resulting DAC created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### DAC - Set Output List
**Route**: `Tool/PatchClampDAC/SetOutputList`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being modified.  
- (int) `PatchClampDACID` ID of the target dac to be modified.
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
- (float) `ClampPosX_nm` X world space coordinate in nanometers.
- (float) `ClampPosY_nm` Y world space coordinate in nanometers.
- (float) `ClampPosZ_nm` Z world space coordinate in nanometers.
- (string) `Name` Name of the DAC, defaults to 'undefined'.

**Response**:  
- (bgShapeID) `PatchClampADCID` ID of the resulting ADC created here (-1 on fail).  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### ADC - Set Sample Rate
**Route**: `Tool/PatchClampADC/SetSampleRate`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being modified.  
- (bgPatchClampADCID) `TargetADC` ID of the ADC being configured.
- (float) `Timestep_m=` Sets the sample timestep rate for the ADC in milliseconds. 

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.


### ADC - Get Recorded Data
**Route**: `Tool/PatchClampADC/GetRecordedData`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this is being modified.  
- (bgPatchClampADCID) `TargetADC` ID of the ADC being configured.

**Response**:  
- (bgStatus) `StatusCode` Numeric status code, helping the gateway determine what went wrong.
- (string) `RecordedData_mV` JSON formatted list of voltages recorded by the ADC.
- (float) `Timestep_ms` Gets the sample timestep rate for the ADC in milliseconds. 