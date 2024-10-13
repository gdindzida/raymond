# raymond
raytracer in a weekend*

What next?

Random generation optimized, memory moved to static except materials & textures. What next to optimize? Move also materials & 
textures to static memory?

Created basic gui window. What next? Some kind of controls?

Triangles — Most cool models are in triangle form. The model I/O is the worst and almost everybody tries to get somebody else’s code to do this. This also includes efficiently handling large meshes of triangles, which present their own challenges.

Used OpenMP for multithreading. Speed up is around 50%. More than 6 threads doesn't yield benefits (My computer has 6 cores). What next? CUDA? OpenGL?

Shadow Rays — When firing rays at light sources, you can determine exactly how a particular point is shadowed. With this, you can render crisp or soft shadows, adding another degreee of realism to your scenes.
