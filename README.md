# RTv1


### Objective
Implement Raytracing protocol to render a scene from a specific angle and position. The scene is composed of simple geometric shapes and is illuminated by the lighting system defined in the scene file.

<img src="https://github.com/wehard/rtv1/blob/master/assets/rtv1_banner.png?raw=true"/>

### Features
- Shapes
	- Plane, Sphere, Cylinder, Cone
	- Translation, rotation and scaling of shapes
- Camera
	- Translation rotation and fov
- Light management
	- Directional eg. sunlight
		- Direction
		- Color
	- Point
		- Position
		- Color, attenuation etc.
	- Ambient (color, brightness)
	- Shadows
- Scene defined in file (see below for scene definition syntax)
	- Supports hot-reloading


### Scene definition

All sections are need to end with a closing ```#```
```
HEADER
#
LIGHT
#
OBJECT
#
```

#### Header
Camera is defined in the header. [There can be only one!](https://www.youtube.com/watch?v=_J3VeogFUOs)
```
OBJECTS <integer> (number of objects, must match actual objects defined below)
LIGHTS <integer> (same thing, must match)
COLOR red green blue (ambient color)
CAMERA
fov 60.0
pos 0.0 30 160.0
look_at 0 0 0
```
#### Lights

Lights are defined as:

```
LIGHT
type <integer> (0 for DIRECTIONAL, 1 for POINT)
pos <x> <y> <z>
dir <x> <y> <z> (for point light this is ignored for obvious reasons)
col <r> <g> <b>
```

#### Objects

Objects are defined using their type and properties. Some objecs ignore some of the properties if they don't need them.

```
PLANE/SPHERE/CYLINDER/CONE
pos <x> <y> <z>
rot <x> <y> <z> (degrees)
sca <x> <y> <z>
rad <double>
col <x> <y> <z>
```

<img src="https://github.com/wehard/rtv1/blob/master/assets/rtv1.png?raw=true"/>
