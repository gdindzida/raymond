# raymond
raytracer in a weekend*

What next?

Solve todos.

Move exported image in gui window instead of saving it.

Triangles — Most cool models are in triangle form. The model I/O is the worst and almost everybody tries to get somebody else’s code to do this. This also includes efficiently handling large meshes of triangles, which present their own challenges.

Parallelism — Run N
 copies of your code on N
 cores with different random seeds. Average the N
 runs. This averaging can also be done hierarchically where N/2
 pairs can be averaged to get N/4
 images, and pairs of those can be averaged. That method of parallelism should extend well into the thousands of cores with very little coding.

Shadow Rays — When firing rays at light sources, you can determine exactly how a particular point is shadowed. With this, you can render crisp or soft shadows, adding another degreee of realism to your scenes.
