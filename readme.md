# BlackHole Ray Tracer

![](image/BlackHole2.gif)

## Description
Simulation of Schwarzschild black hole using OpenGL ray marching.

## How Realistic?
- [x] gravitational light deflection
- [x] gravitational redshift of accretion disk
- [x] doppler effect and relativistic beaming of accretion disk
- [x] realistic temperature distribution
- [x] accretion disk(Innermost stable circular orbit, orbiting speed)


- [ ] doppler shift, redshift of background stars
- [ ] observer position, velocity

## Controls
- Mouse right click&drag to rotate camera
- H to disable HUD
- C to change camera mode(orbit <-> free, default orbit)
- WASD to move(free camera mode only)
- Shift/Control to move up/down(free camera mode only)
- Q/E to roll camera(free camera mode only)

## Possible Updates
- doppler shift, redshift of background stars
- observer position, velocity
- orbiting star

## External Libraries, Files Used
- OpenGL, GLFW, GLM, GLAD
- stb_image
- Dear ImGui
- realistic space skybox from [NASA Deep Star Maps 2020](https://svs.gsfc.nasa.gov/4851)
- Hack font from [source-foundry](https://github.com/source-foundry/Hack)
