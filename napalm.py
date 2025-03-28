import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation

# Configuração da grade
ncols = 100
nrows = 100
time_steps = 5

# Criação da malha
x = np.linspace(-5, 5, ncols)
y = np.linspace(-5, 5, nrows)
X, Y = np.meshgrid(x, y)

# Posição da fonte
source_x = 0
source_y = 0

# Configuração da figura
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')
cmap = plt.get_cmap('viridis')

# Criar o frame inicial
frame_initial = np.zeros((nrows, ncols))
surf = ax.plot_surface(X, Y, frame_initial, rstride=1, cstride=1, cmap=cmap, vmin=0, vmax=1.5)
fig.colorbar(surf, ax=ax, label='Densidade da névoa')
ax.set_xlabel('X(m)')
ax.set_ylabel('Y(m)')
ax.set_zlabel('Densidade')
plt.title('I love the smell of Napalm by morning')

# Pré-calcular todos os frames
frames = []
for t in range(time_steps):
    density = np.zeros((nrows, ncols))
    for i in range(nrows):
        for j in range(ncols):
            distance = np.sqrt((X[i, j] - source_x)**2 + (Y[i, j] - source_y)**2)
            density[i, j] = 1 / (distance**2 + 0.5) * (t+1)
    frames.append(density)

# Função de atualização para animação
def update(frame_num):
    ax.clear()
    surf = ax.plot_surface(X, Y, frames[frame_num], rstride=1, cstride=1, cmap=cmap, vmin=0, vmax=1.5)
    ax.set_xlabel('X(m)')
    ax.set_ylabel('Y(m)')
    ax.set_zlabel('Densidade')
    plt.title(f'I love the smell of Napalm by morning - Frame {frame_num+1}')
    return [surf]

# Criar animação
ani = animation.FuncAnimation(fig, update, frames=time_steps, interval=500, blit=False)

# Salvar cada frame como um arquivo PNG
for t in range(time_steps):
    update(t)
    plt.savefig(f'frame_{t+1}.png')

plt.show()