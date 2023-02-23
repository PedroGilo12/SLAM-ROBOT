import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

'''
Parâmetros do robô:

raio da roda (r) = 61mm (6.1cm; 0.061m)
metade do eixo do robô (l) = 71mm (7.1cm; 0.071m)

velocidade maxima da roda direita: 30.5 rad/s
velocidade maxima da roda esquerad: 37.5 rad/s (255), 35 rad/s(207)

'''

fig, ax = plt.subplots()

P = np.array([[0], [0] ,[0]]) # Posição inicial do Robô

r = 0.061
l = 0.071

wl = 36                 # Velocidade angular da roda esquerda (rad/s)
wr = 36              # Velocidade angular da roda direita  (rad/s)

v = (r*(wr+wl))/2       # Velocidade linear do robô
w = (r/(2*l))*(wr-wl)   # Velocidade angular do robô

'''


'''

dt = 0.5                  # Tempo de amostragem

maxt = 10

ax.plot(P[0], P[1], marker = "o", color = "green")


for i in range(maxt):
    x_tail = float(P[0])
    y_tail = float(P[1])

    i = i + dt
    print(i)
    th = P[2]

    v = (r*(wr+wl))/2       # Velocidade linear do robô
    w = (r/(2*l))*(wr-wl)   # Velocidade angular do robô

    dP = np.array([[v*np.cos(float(th))],[v*np.sin(float(th))],[w]]) # Decomposição das velocidades em x,y,w

    P = P + dP *dt

    print(dP)

    ax.plot(P[0], P[1], marker = "o", color = "red")

    x_head = float(P[0])
    y_head = float(P[1])
    dx = x_head - x_tail
    dy = y_head - y_tail
    arrow = mpatches.FancyArrowPatch((x_tail, y_tail), (x_head, y_head),
                                    mutation_scale=10)
    ax.add_patch(arrow)



plt.show()
quit()
