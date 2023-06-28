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

**Notes**  
 - None.


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


### Cylinder - Create
**Route**: `Geometry/Shape/Cylinder/Create`  
**Request**:  
*Required Params*:  
- (int) `SimulationID` ID of the simulation where this sphere is being created.  
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
- (bgShapeID) `ShapeID=` ID of the resulting shape created here (-1 on fail).  
