# import Modules ----------------------------------
from re import S
from turtle import st
import numpy as np
from stl import mesh

import matplotlib.pyplot as plt

import timeit

# import stl file ---------------------------------
start = timeit.default_timer() #start timer
stl_mesh = mesh.Mesh.from_file("3DBenchy.stl")
stop = timeit.default_timer() #stop timer
print("File loaded in ",len(stl_mesh.vectors)," faces as vertices in ",stop-start," [s]")

z = np.linspace(np.min(stl_mesh.z),np.max(stl_mesh.z),10)

# doing four points from [[x1,x2,...],[y1,y2,...],[z1,z2,...]]
# creating pairs of points for each face in a long string
start = timeit.default_timer() #start timer
p1 = np.zeros([3,len(stl_mesh.vectors)*3])
p2 = np.zeros([3,len(stl_mesh.vectors)*3])

i = 0 # initilize index for points
bad_face = 0 # track bad faces
for face in range(len(stl_mesh.vectors)):
    p_temp0 = stl_mesh.vectors[face][0] # x points for faces
    p_temp1 = stl_mesh.vectors[face][1] # y points for faces
    p_temp2 = stl_mesh.vectors[face][2] # z points for faces
    if (np.all(p_temp0 == p_temp1)|np.all(p_temp0 == p_temp2)|np.all(p_temp1 == p_temp2)): # check if all three points are unique (no duplicates)
        bad_face += 1
    else: # save set of points
        # p1 and p2 are offset in vertices p1=[v1,v2,v3] p2=[v2,v3,v1]
        p1[:,0+i] = p_temp0
        p1[:,1+i] = p_temp1
        p1[:,2+i] = p_temp2
        p2[:,0+i] = p_temp1
        p2[:,1+i] = p_temp2
        p2[:,2+i] = p_temp0
        i += 3

# remove end of zeros due to bad faces
p1 = p1[:,:i] # contains [[x,y,z],for each vertices (each face has 3 unique vertices)]
p2 = p2[:,:i]

stop = timeit.default_timer() #stop timer
print("Converted vertices to sets of points in ",stop-start," [s]")

# test creating of pairs of points again for later
#z = np.linspace(0.1,0.9,5)
#p1 = np.array([[0,1,0],[1,0,0],[0,0.5,1]])
#p2 = np.array([[0,0,1],[0,1,0],[1,0,0.5]])

# create parameterized line in space between p1 and p2
# P = V*t + b where V=[a,b,c] "normal vector". B=[bx,by,bz] "offset vector"
start = timeit.default_timer() #start timer
V = p2 - p1 # size is [space dim , num of pairs of points aka lines]
B = p1

# find point [x,y,z] for each z layer
# deal with parrell lines (considered not to touch even if overlaping is on slice)
i = V[2] == 0 # indies that are parrellel
t = np.zeros([len(z),len(V[0])]) # dim (# of slices, number or vertices)
t[:,i] = np.nan # setting all lines that are parrell to slices to nan
t[:,~i] = np.add.outer(z, -B[2,~i])/V[2,~i] # calculating intercecting parameter for each slice and parametericed line. size is [num of z layers , num of pairs of points]

p = np.zeros([len(z),len(V[:,0]),len(V[0])]) # size is [num of z layers , space dim, num of pairs of points]

for layer in range(len(z)): # compute intercepting points for each layer of z
    p[layer] = V*t[layer,:] + B # p is a spacial point for each line and layer

stop = timeit.default_timer() #stop timer
print("computed all z intercept points in ",stop-start," [s]")

# remove points that do note lie within the bounded box
# points that dont line within stl face are removed, including vertices intercetions
start = timeit.default_timer() #start timer
# calculate magniute of intecetion vector and 
u = p - p1 # displacement vector for each intercetion points and its starting point
uM = np.sqrt(u[:,0,:]**2+u[:,1,:]**2+u[:,2,:]**2) # displacement magnitude
VM = np.sqrt(V[0,:]**2+V[1,:]**2+V[2,:]**2) # direction vector magnitude
# Duplate arrays to be used for each layer (same dimensions)
V = np.repeat(V[np.newaxis,:,:],len(u[:,0,0]),axis=0)
VM = np.repeat(VM[np.newaxis,:],len(uM[:,0]),axis=0)

# find points that have smaller magnitude and in same direction as 
i = (uM<VM) & (u[:,2,:]*V[:,2,:]>0)
i = np.repeat(i[:,np.newaxis,:],3,axis=1)

p[~i] = np.nan # remove all out of bound points

stop = timeit.default_timer() #stop timer
print("removed all points that are outside of bounds in ",stop - start," [s]")

# create line segment from set of intercecting points
start = timeit.default_timer() #start timer

line1 = p[:,:,0::3]
line2 = p[:,:,1::3]
line3 = p[:,:,2::3] 

# remove lines that dont intesect (make arrays smaller)
i1 = np.sum(~np.isnan(line1[:,0,:]),axis=0)
i2 = np.sum(~np.isnan(line2[:,0,:]),axis=0)

i = (i1 + i2) != 0 

line1 = line1[:,:,i]
line2 = line2[:,:,i]
line3 = line3[:,:,i]

#####
# create empty arrays of sets of points
longline1 = np.zeros([line1.shape[0]*line1.shape[2],3])
longline2 = np.zeros([line1.shape[0]*line1.shape[2],3])
longline3 = np.zeros([line1.shape[0]*line1.shape[2],3])

set1 = np.zeros([line1.shape[0]*line1.shape[2],3])
set2 = np.zeros([line1.shape[0]*line1.shape[2],3])

for line in range(line1.shape[2]):
    longline1[line*line1.shape[0]:(line+1)*line1.shape[0],:] = line1[:,:,line]
    longline2[line*line1.shape[0]:(line+1)*line1.shape[0],:] = line2[:,:,line]
    longline3[line*line1.shape[0]:(line+1)*line1.shape[0],:] = line3[:,:,line]

# find idex that have values (not nan)
i1 = ~np.isnan(longline1[:,0])
i2 = ~np.isnan(longline2[:,0])
i3 = ~np.isnan(longline3[:,0])

set1[(i1 + i2) == 2,:] = longline1[(i1 + i2) == 2,:]
set2[(i1 + i2) == 2,:] = longline2[(i1 + i2) == 2,:]

set1[(i1 + i3) == 2,:] = longline1[(i1 + i3) == 2,:]
set2[(i1 + i3) == 2,:] = longline3[(i1 + i3) == 2,:]

set1[(i2 + i3) == 2,:] = longline2[(i2 + i3) == 2,:]
set2[(i2 + i3) == 2,:] = longline3[(i2 + i3) == 2,:]

set1 = set1[(i1+i1+i3)>0,:]
set2 = set2[(i1+i1+i3)>0,:]

#####
# append points on line that are not nan (points that intercet triangular face)

for line in range(np.sum(i)):
    i1 = ~np.isnan(line1[:,0,line]) # pionts that are not nan on a line 
    i2 = ~np.isnan(line2[:,0,line])
    i3 = ~np.isnan(line3[:,0,line])

    case = np.argmax(np.array([np.sum(i1),np.sum(i2),np.sum(i3)]))
    point1 = np.zeros(np.shape(line1[:,:,line]))
    point2 = np.zeros(np.shape(line1[:,:,line]))

    if (case==0): # first line is primary line
        point1[i1,:] = line1[i1,:,line]
        point2[i2,:] = line2[i2,:,line]
        point2[i3,:] = line3[i3,:,line]
    if (case==1): # second line is primary line
        point1[i2,:] = line2[i2,:,line]
        point2[i3,:] = line3[i3,:,line]
        point2[i1,:] = line1[i1,:,line]
    if (case==2): # third line is primary line
        point1[i3,:] = line3[i3,:,line]
        point2[i1,:] = line1[i1,:,line]
        point2[i2,:] = line2[i2,:,line]

    # append non trivial values
    set1 = np.append(set1,point1[i1+12>0])
    set2 = np.append(set2,point2[i1+12>0])


stop = timeit.default_timer() #stop timer
print("create pair of interceting points ",stop - start," [s]")




for line in range(len(line1[0,0,:])):
    i1 = ~np.isnan(line1[:,0,line])
    i2 = ~np.isnan(line2[:,0,line])
    if (np.sum(i1)==0):
        S
    elif (np.sum(i2)==0):
        S
    else:
        P1 = line3
        P2 = line1 + line2

i = np.isnan(line1[:,])