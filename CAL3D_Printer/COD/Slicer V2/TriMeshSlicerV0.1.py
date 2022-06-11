# import modules ----------------------------------------
import trimesh
from shapely.geometry import LineString
import numpy as np
import matplotlib.pyplot as plt

# Load in STL file --------------------------------------
stl_mesh = trimesh.load_mesh("3DBenchy.stl")

# Generte Intersection Rays -----------------------------
num = 1200 # number of rays

ray_origins = np.zeros([num,3])
ray_origins[:,0] = 30 # x coordinates
ray_origins[:,1] = np.linspace(-15,15,num) # y coordinates
ray_origins[:,2] = 20 # z coordinates

ray_directions = np.zeros([num,3])
ray_directions[:,0] = -1 # x component
ray_directions[:,1] = 0 # y component
ray_directions[:,2] = 0 # z component

# stack rays into line segments for visualization as Path3D
ray_visualize = trimesh.load_path(np.hstack((ray_origins, ray_origins + ray_directions*5.0)).reshape(-1, 2, 3))

# create a visualization scene with rays, hits, and mesh
scene = trimesh.Scene([stl_mesh, ray_visualize])# create a visualization scene with rays, hits, and mesh

# show the visualization
scene.show() # Show rays with 3D render of STL model

# create Cross section ------------------------------
stl_slice = stl_mesh.section(plane_origin=[0,0,20], plane_normal=[0,0,1])
stl_slice.show()

# Calculate intersection points ---------------------
locations, index_ray, index_tri = stl_mesh.ray.intersects_location( ray_origins=ray_origins, ray_directions=ray_directions)

# Plot cross section overlayed with intersections ---
for ray in range(num):
    plt.plot(locations[index_ray==ray,0],locations[index_ray==ray,1],"*")

plt.plot(stl_slice._vertices[:,0],stl_slice._vertices[:,1],".")
plt.show()

# Create Object Function --------------------------
obj_func = np.zeros(num)
check = np.zeros(num)
for ray in range(num):
    check[ray] = np.sum(index_ray==ray)
    obj_func[ray] = np.sqrt(np.sum(locations[index_ray==ray,0]**2 + locations[index_ray==ray,1]**2))

if (np.any(check%2==1)):
    print("intersetion is odd... fuck")

plt.plot(obj_func)
plt.show()

# stop code but keep param (debugging)
print("end")