# API Design Spec



## Shapes

### Sphere - Create
URI: `/NES/Geometry/Shape/Sphere/Create?`
Request:
    Required Params:
        - (float) `Radius_nm=` The radius of the sphere in nanometers.
        - (vec3) `Center_nm=` Position of the center of the sphere in nanometers. Given as a list containing the three x,y,z coordinates.
    Optional Params:
        - (string) `Name=` Optional name of the sphere.
Response:
    - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
    - (bgShapeID) `ID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).

### Cylinder - Create
URI: `/NES/Geometry/Shape/Cylinder/Create?`
Request:
    Required Params:
        - (float) `Point1Radius_nm=` The radius at the cylinder's first point.
        - (vec3) `Point1Position_nm=` First endpoint of the cylinder's axis.
        - (float) `Point2Radius_nm=` The radius at the cylinder's second point.
        - (vec3) `Point2Position_nm=` Second endpoint of the cylinder's axis.
    Optional Params:
        - (string) `Name=` Optional name of the cylinder.
Response:
    - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
    - (bgShapeID) `ID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).

### Box - Create
URI: `/NES/Geometry/Shape/Box/Create?`
Request:
    Required Params:
        - (vec3) `CenterPosition_nm=` Position of the center of the box in world space coordinates.
        - (vec3) `Dimensions_nm=` (X,Y,Z) dimensions of the box in nm.
        - (vec3) `Rotation_rad=` (X,Y,Z) rotation of the box in radians.
    Optional Params:
        - (string) `Name=` Optional name of the box.
Response:
    - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
    - (bgShapeID) `ID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).



## Compartments
URI: `/NES/Compartment/BS/Create?`
Request:
    Required Params:
        - (bgShapeID) `ShapeID=` ID of an NES Shape.
        - (float) `MembranePotential_mV=` Membrane potential of the compartment in millivolts.
        - (float) `SpikeThreshold_mV=` Action potential spike threshold of the compartment in millivolts.
        - (float) `DecayTime_ms=` (tau_ahp) After hyperpolarization time constant in milliseconds.
    Optional Params:
        - (string) `Name=` Optional name of the compartment.
Response:
    - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
    - (bgCompartmentID) `ID=` ID of the resulting compartment created here (if status indicates success, otherwise this is -1).


## Connections
URI `/NES/Connection/Compartment/Staple/Create?`
Request:
    Required Params:
        - (bgCompartmentID) `SourceCompartmentID=` ID of the compartment whos data will be copied to the destination.
        - (bgCompartmentID) `DestinationCompartmentID=` ID of the compartment whos data will be overwritten with the source.
    Optional Params:
        - (string) `Name=` Optional name of the staple (clippy?).
Response:
    - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
    - (bgConnectionID) `ID=` ID of the resulting connection created here (if status indicates success, otherwise this is -1).
