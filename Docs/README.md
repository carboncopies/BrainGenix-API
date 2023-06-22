# API Design Spec


## Compartments
BGResult nesInitCompartment(
    nesShape _Shape,
    nesList _Params
)


URI: `/NES/Compartment/Create?`
Request:
    - `Shape=` ID of an NES Shape

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
    - (bgID) `ID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).

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
    - (bgID) `ID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).

### Box - Create
URI: `/NES/Geometry/Shape/Box/Create?`
Request:
    Required Params:
        - (vec3) `CenterPosition_nm=` Position of the center of the box in world space coordinates.
        - (vec3) `Dimensions_nm=` X,Y,Z dimensions of the box in nm.
        - (vec3) `Rotation_rad=` X,Y,Z rotation of the box in radians.
    Optional Params:
        - (string) `Name=` Optional name of the box.
Response:
    - (bgStatus) `StatusCode=` Enum indicating the status of this API call.
    - (bgID) `ID=` ID of the resulting shape created here (if status indicates success, otherwise this is -1).