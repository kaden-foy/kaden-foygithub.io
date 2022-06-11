# import modules ----------------------------------------
from ast import Slice
import trimesh
from shapely.geometry import LineString
import numpy as np
import matplotlib.pyplot as plt
import timeit

# Load in STL file --------------------------------------
start = timeit.default_timer() #start timer

stl_mesh = trimesh.load_mesh("3DBenchy.stl")

stop = timeit.default_timer() #stop timer
print("File loaded in ",stop-start," [s]")

# Generte Intersection Rays -----------------------------
start = timeit.default_timer() #start timer

width_num = 150 # display width (pixels) [#]
width_dim = np.array([-32,32]) # display width (size) [mm]

height_num = 120 # display height (pixels) [#]
height_dim = np.array([-1,49]) # display heigth (size) [mm]

# get smallest cylinder that fits around stl file 
cylinder_rad = np.max(np.sqrt(stl_mesh.vertices[:,0]**2+stl_mesh.vertices[:,1]**2))
cylinder_height = np.array([np.min(stl_mesh.vertices[:,2]),np.max(stl_mesh.vertices[:,2])])

ray_origins = np.zeros([width_num,height_num,3])
ray_directions = np.zeros([width_num*height_num,3])

x_val = np.linspace(width_dim[0],width_dim[1],width_num) # x coordinates
y_val = cylinder_rad + 1 # y coordinates
z_val = np.linspace(height_dim[0],height_dim[1],height_num) # z coordinates

ray_origins[:,:,0] = np.meshgrid(np.ones(height_num),x_val)[1] # x coordinates
ray_origins[:,:,1] = cylinder_rad # y coordinates
ray_origins[:,:,2] = np.meshgrid(z_val,np.ones(width_num))[0] # z coordinates
ray_origins = ray_origins.reshape([-1,3])

ray_directions[:,0] = 0 # x component
ray_directions[:,1] = -1 # y component
ray_directions[:,2] = 0 # z component

stop = timeit.default_timer() #stop timer
print("Generate Intersection Rays in ",stop-start," [s]")

# rotate the rays to face stl angle at theta -----------
start = timeit.default_timer() #start timer

theta = np.deg2rad(0) # and of rays [deg]

def Rotate(V,t):
    V_rotated = V.copy()
    V_rotated[:,0] = V[:,0]*np.cos(theta) - V[:,1]*np.sin(theta)
    V_rotated[:,1] = V[:,0]*np.sin(theta) + V[:,1]*np.cos(theta)
    return V_rotated

ray_origins = Rotate(ray_origins,theta)
ray_directions = Rotate(ray_directions,theta)

######
# stack rays into line segments for visualization as Path3D
#ray_visualize = trimesh.load_path(np.hstack((ray_origins, ray_origins + ray_directions*5.0)).reshape(-1, 2, 3))

# create a visualization scene with rays, hits, and mesh
#scene = trimesh.Scene([stl_mesh, ray_visualize])# create a visualization scene with rays, hits, and mesh

# show the visualization
#scene.show() # Show rays with 3D render of STL model
######

stop = timeit.default_timer() #stop timer
print("Rotated Rays in ",stop-start," [s]")

# compute intersections ---------------------------------
start = timeit.default_timer() #start timer

locations, index_ray, index_tri = stl_mesh.ray.intersects_location( ray_origins=ray_origins, ray_directions=ray_directions) 

stop = timeit.default_timer() #stop timer
print("Compute intersections in ",stop-start," [s]")

# Generate Object function ------------------------------
start = timeit.default_timer() #start timer

obj_func = np.zeros([width_num,height_num]) # initize array
ray = 0
bad_ray_index = np.array([[],[]])

for w in range(width_num):
    for h in range(height_num):
        
        if (np.any(ray==index_ray)):
            dispacement = locations[index_ray==ray,:2][1:] - locations[index_ray==ray,:2][:-1]
            if (len(dispacement[:,0])%2==0):
                bad_ray_index = np.append(bad_ray_index,[[ray],[len(dispacement[:,0])]],axis=1)
            obj_func[w,h] = np.sqrt(np.sum(dispacement[::2,0]**2 + dispacement[::2,1]**2))
        else:
            obj_func[w,h] = -1
        ray += 1



stop = timeit.default_timer() #stop timer
print("Generate object functions in ",stop-start," [s]")

plt.imshow(obj_func)
plt.show()

#############################
# code used to plot slice
X = locations[:,0]
Y = locations[:,1]
Z = locations[:,2]

zz = np.unique (Z)

layer = 2
STLslice = stl_mesh.section(plane_origin=[0,0,zz[layer]], plane_normal=[0,0,1])
slice_2D, to_3D = STLslice.to_planar()

plt.plot(X[Z==zz[layer]],Y[Z==zz[layer]],"r.")
slice_2D.show()
plt.show()

print("complete")