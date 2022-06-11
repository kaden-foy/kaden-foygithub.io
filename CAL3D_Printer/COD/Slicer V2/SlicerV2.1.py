# import Modules ----------------------------------
import numpy as np
from stl import mesh

# import stl file ---------------------------------
stl_mesh = mesh.Mesh.from_file("3DBenchy.stl")
print("File loaded in as faces and vertices")

# Slice STL file with planes normal to X/Y ---------
num = 46 # number of layers
layers = np.linspace(np.min(stl_mesh.z)+0.0001,np.max(stl_mesh.z)-0.0001,num) # Z values for each layer

 # initilize points
 # this step is very slow because i append to these variables for each element
Set_1 = np.zeros([3,0])
Set_2 = np.zeros([3,0])

# save faces that dont would through error
Bad_Faces = np.array([])

l = len(stl_mesh.vectors[:])
#l = 50000*2
for face in range(l):
    # check if face is actually a line (bad stl file)
    v1 = stl_mesh.vectors[face][0,:] - stl_mesh.vectors[face][1,:]
    v2 = stl_mesh.vectors[face][1,:] - stl_mesh.vectors[face][2,:] 
    v3 = stl_mesh.vectors[face][2,:] - stl_mesh.vectors[face][0,:]
    # check that face intercept z point
    z_lim = np.array([stl_mesh.vectors[face][:,2].min(),stl_mesh.vectors[face][:,2].max()])
    inter = (z_lim[0]-layers)*(z_lim[1]-layers) < 0
    
    tol = 1e-7
    if np.any(np.all(abs(v1)<=tol) or np.all(abs(v2)<=tol) or np.all(abs(v3)<=tol) or np.all(inter)):
        Bad_Faces = np.append(Bad_Faces,face) # for debugging
        # just compute random task
    else:
        
        # code used to find slice lines
        t = np.zeros([3]) # initilaize line parameter

        ## solve for all z intercepts
        idx = (layers>z_lim[0]) & (layers<z_lim[1])

        if len(layers[idx]) == 0:
            continue

        # solve line intercept parameter value
        t = np.zeros([3,len(idx)])

        t[0,:] = np.transpose((layers[idx]-stl_mesh.vectors[face][0,2])/(stl_mesh.vectors[face][1,2]-stl_mesh.vectors[face][0,2]))
        t[1,:] = np.transpose((layers[idx]-stl_mesh.vectors[face][1,2])/(stl_mesh.vectors[face][2,2]-stl_mesh.vectors[face][1,2]))
        t[2,:] = np.transpose((layers[idx]-stl_mesh.vectors[face][2,2])/(stl_mesh.vectors[face][0,2]-stl_mesh.vectors[face][2,2]))

        II0 = np.array((stl_mesh.vectors[face][1,:]-stl_mesh.vectors[face][0,:]).reshape(-1,1)*t[0,:] + stl_mesh.vectors[face][0,:].reshape(-1,1))
        II1 = np.array((stl_mesh.vectors[face][2,:]-stl_mesh.vectors[face][1,:]).reshape(-1,1)*t[1,:] + stl_mesh.vectors[face][1,:].reshape(-1,1))
        II2 = np.array((stl_mesh.vectors[face][0,:]-stl_mesh.vectors[face][2,:]).reshape(-1,1)*t[2,:] + stl_mesh.vectors[face][2,:].reshape(-1,1))

        # remove points outside of bounds
        idx0 = ((II0[0,:]<stl_mesh.vectors[face][[0,1],0].min()) + (II0[0,:]>stl_mesh.vectors[face][[0,1],0].max()) 
            + (II0[1,:]<stl_mesh.vectors[face][[0,1],1].min()) + (II0[1,:]>stl_mesh.vectors[face][[0,1],1].max())
            + (II0[2,:]<stl_mesh.vectors[face][[0,1],2].min()) + (II0[2,:]>stl_mesh.vectors[face][[0,1],2].max()))
        idx1 = ((II1[0,:]<stl_mesh.vectors[face][[1,2],0].min()) + (II1[0,:]>stl_mesh.vectors[face][[1,2],0].max()) 
            + (II1[1,:]<stl_mesh.vectors[face][[1,2],1].min()) + (II1[1,:]>stl_mesh.vectors[face][[1,2],1].max())
            + (II1[2,:]<stl_mesh.vectors[face][[1,2],2].min()) + (II1[2,:]>stl_mesh.vectors[face][[1,2],2].max()))
        idx2 = ((II2[0,:]<stl_mesh.vectors[face][[0,2],0].min()) + (II2[0,:]>stl_mesh.vectors[face][[0,2],0].max()) 
            + (II2[1,:]<stl_mesh.vectors[face][[0,2],1].min()) + (II2[1,:]>stl_mesh.vectors[face][[0,2],1].max())
            + (II2[2,:]<stl_mesh.vectors[face][[0,2],2].min()) + (II2[2,:]>stl_mesh.vectors[face][[0,2],2].max()))
        
        # save points for every intercept for every z level...
        II0[:,idx0] = 0
        II1[:,idx1] = 0
        II2[:,idx2] = 0

        # combine points into pairs of points that can be stored easily
        # find the ONE line that has all points within bonds. This is the base line
        # the secondary points are the sum of the other two points becuase index above 
        # sets outof bounds points to zero
        if(sum(idx0)==0):
                P1 = II0
                P2 = II1 + II2
        elif(sum(idx1)==0):
                P1 = II1
                P2 = II0 + II2
        else:
                P1 = II2
                P2 = II0 + II1
        
        Set_1 = np.append(Set_1,P1,axis=1)
        Set_2 = np.append(Set_2,P2,axis=1)

print("Num of Bad Faces: ",len(Bad_Faces)," Total Num of Faces: ",len(stl_mesh.vectors[:]))
np.save("Set_1",Set_1)
np.save("Set_2",Set_2)
np.save("Layers",layers)