#!/usr/bin/python
import numpy as np
import scipy.linalg
import matplotlib.pyplot as plt

def dlqr(A,B,Q,R):
    """
    Solve the discrete time lqr controller.
    x[k+1] = A x[k] + B u[k]
    cost = sum x[k].T*Q*x[k] + u[k].T*R*u[k]
    """
    # first, solve the ricatti equation
    P = np.matrix(scipy.linalg.solve_discrete_are(A, B, Q, R))
    # compute the LQR gain
    K = np.matrix(scipy.linalg.inv(B.T*P*B+R)*(B.T*P*A))
    return -K

l = .38 # rod length is 2l
m = .150
M = .175
dt = .02 # 20 ms
g = 9.8

A = np.matrix([[1, dt, 0, 0],[0,1, -(3*m*g*dt)/(4*M+m),0],[0,0,1,dt],[0,0,(6*g*(m+M)*dt)/(l*(4*M+m)),1]])
B = np.matrix([[0],[4*dt/(4*M+m)],[0],[-6*dt/(l*(4*M+m))]])

print A
print B

#Q = np.matrix("1 0 0 0; 0 .0001 0 0 ; 0 0 1 0; 0 0 0 .0001")
#R = np.matrix(".0005")

Q = np.matrix(".1 0 0 0; 0 .0001 0 0 ; 0 0 1 0; 0 0 0 .0001")
R = np.matrix(".005")


K = dlqr(A,B,Q,R)
print K
# x v a w
print "double c[] = {%f, %f, %f, %f};" % (K[0,0], K[0,1], K[0,2], K[0,3])

nsteps = 4 / .02
time = np.linspace(0, 4, nsteps, endpoint=True)
xk = np.matrix(".2 ; 0 ; .1 ; 0")

X = []
T = []
U = []

cost = 0
'''
for t in time:
    uk = 1*xk[0,0] + 1/1000*xk[1,0] + 45*xk[2,0] + 1/4000*xk[3,0]
    X.append(xk[0,0])
    T.append(xk[2,0])
    v = xk[1,0]
    force = uk
    accel = force/(M+m)
    u = ((1-.251)*v + dt*accel)/.047/10
    U.append(u)
    xk = A*xk + B*uk*.0002 /9999.
    cost += u*u*0.02
    #pid 0.23458108573
'''
for t in time:
    uk = K*xk
    X.append(xk[0,0])
    T.append(xk[2,0])
    v = xk[1,0]
    force = uk[0,0]
    accel = force/(M+m)
    u = ((1-.251)*v + dt*accel)/.047/10
    U.append(u)
    xk = A*xk + B*uk
    cost += u*u*0.02
    #lqr 0.23458108573'''

plt.plot(time, X, label="$x$ (m)")
plt.plot(time, T, label="$\\theta$ (rad)")
plt.plot(time, U, label='$f$ (daV)')
plt.xlabel("Tempo (s)")

plt.legend(loc='upper right')
plt.grid()
plt.show()

#double c[] = {30.789042, 20.549323, 67.716816, 12.689277};
print(cost)
