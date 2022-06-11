# import Modules ----------------------------------
import numpy as np
import matplotlib.pyplot as plt

# Create Functions for Rays ----------------------
# rays are definied as ax+by+c=0
#math ref: https://www.cuemath.com/geometry/intersection-of-two-lines/

# generate parrellel rays
def Generate_Rays(Theta,Spacing,Num):
    # takes angle, spacing and number of rays 
    # return a matrix of a,b,c values for each ray
    Rays = np.zeros([3,Num]) # empty matrix with a,b and c values ax+by+c=0
    if (Theta == 0): #horizontal lines
        Rays[0,:] = 0
        Rays[1,:] = 1
        Rays[2,:] = np.arange(0, Spacing*Num,Spacing) - Num*Spacing/2 + Spacing/2
    elif (Theta == np.pi/2): # vertical lines
        Rays[0,:] = 1
        Rays[1,:] = 0
        Rays[2,:] = np.arange(0, Spacing*Num,Spacing) - Num*Spacing/2 + Spacing/2
    else:
        Rays[0,:] = -np.tan(Theta)
        Rays[1,:] = 1
        Rays[2,:] = (np.arange(0, Spacing*Num,Spacing) - Num*Spacing/2 + Spacing/2)/np.abs(np.cos(Theta))
    
    return Rays

# Generate rays from points
def Generator_Lines(P1,P2):
    # takes to points P = [[x],[y]]
    # returns vector of a,b,c for line
    # ax+by+c=0
    A =-(P2[1] - P1[1])
    B = P2[0] - P1[0]
    C = -P1[0]*A - P1[1]*B
    return np.array([A,B,C])

# find intercept points of rays
def Line_Intersection(Line1,Line2):
    # takes P = [[a],[b],[c]] where ax+by+C=0
    # returns points of intersection [x,y]
    # for all possible line combose

    # returns runtime error when lines are parrell (never intercept)
    X = (np.outer(Line1[1],Line2[2])-np.outer(Line1[2],Line2[1]))/(np.outer(Line1[0],Line2[1])-np.outer(Line1[1],Line2[0]))
    Y = (np.outer(Line1[2],Line2[0])-np.outer(Line1[0],Line2[2]))/(np.outer(Line1[0],Line2[1])-np.outer(Line1[1],Line2[0]))
    return X,Y

# Functions used for plotting results ------------------------
# mostly usuful for debugging

# Function used to plot layer
def Plot_Slice(P1,P2,color='b',width=None):
    # takes set of points
    X1 = P1[0]
    X2 = P2[0]
    Y1 = P1[1]
    Y2 = P2[1]
    plt.plot([X1,X2],[Y1,Y2],color,linewidth=width) 

# Plots rays within the x/y limits
def Plot_Rays(Rays,X_lim,Y_lim,color='y'):
    # takes ray parameters, x limit, and
    # y limit and plots the lines
    Y = np.zeros([2,len(Rays[0,:])])
    X = np.zeros([2,len(Rays[0,:])])

    M = -Rays[0,:]/Rays[1,:] # slope of lines
    idx_vert = (M >= 1) | (M < -1) # rays between 45-135 deg
    idx_line = idx_vert == 0 # all other rays

    # generate lines
    Y[0,idx_vert] += Y_lim[0]
    Y[1,idx_vert] += Y_lim[1]
    X[0,idx_vert] = -(Rays[1,idx_vert]*Y[0,idx_vert]+Rays[2,idx_vert])/Rays[0,idx_vert]
    X[1,idx_vert] = -(Rays[1,idx_vert]*Y[1,idx_vert]+Rays[2,idx_vert])/Rays[0,idx_vert]

    # generate lines
    X[0,idx_line] += X_lim[0]
    X[1,idx_line] += X_lim[1]
    Y[0,idx_line] = -(Rays[0,idx_line]*X[0,idx_line]+Rays[2,idx_line])/Rays[1,idx_line]
    Y[1,idx_line] = -(Rays[0,idx_line]*X[1,idx_line]+Rays[2,idx_line])/Rays[1,idx_line]

    # plot lines
    plt.plot(X,Y,color)
    plt.xlim(X_lim[0]*1.05,X_lim[1]*1.05)
    plt.ylim(Y_lim[0]*1.05,Y_lim[1]*1.05)
    return X,Y

# Running Program ------------------------------
P1 = np.load("Benchy_Set_1.npy")
P2 = np.load("Benchy_Set_2.npy")

layers = np.load("Benchy_Layers.npy")[30]
i = P1[2,:] == layers
P1 = P1[0:2,i]
P2 = P2[0:2,i]

#
lines = Generator_Lines(P1,P2) # generate lines from slice 100

rays = Generate_Rays(0*np.pi/180,0.25,100) # Generate rays

xp,yp = Line_Intersection(rays,lines) # Calculate intersection matriz reutrn x y points

xpf,ypf = In_Bound_Points(xp,yp,P1,P2) # filter out points that dont lie inbtween points

# plot results
# fit around outline
plt.close()
Plot_Slice(P1,p2_100)
x_lim = np.array([np.min(P1[0,:]),np.max(P1[0,:])])*1.05
y_lim = np.array([np.min(P1[1,:]),np.max(P1[1,:])])*1.05
Plot_Rays(rays,x_lim,y_lim)
Plot_Rays(rays[:,idx],x_lim,y_lim,'r')
plt.plot(xpf,ypf,'.g')
plt.plot(xp,yp,'.')

idx = 878 # slice(878,879)
Plot_Slice(p1_100[:,idx],p2_100[:,idx],'r')

plt.xlim([np.min(part_mesh.x),np.max(part_mesh.x)])
plt.ylim([np.min(part_mesh.y),np.max(part_mesh.y)])
plt.show()